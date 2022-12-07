#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CResMgr.h"
#include "CLight2D.h"
#include "CLight3D.h"

#include "CStructuredBuffer.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_pEditorCamera(nullptr)
	, m_pUICamera{nullptr}
	, m_pLight2DBuffer(nullptr)
	, m_pLight3DBuffer(nullptr)
	, m_arrMRT{}
	, m_pMergeShader(nullptr)
	, m_pMergeMaterial(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::READ_ONLY, true, nullptr);

	m_pLight3DBuffer = new CStructuredBuffer;
	m_pLight3DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::READ_ONLY, true, nullptr);
}

CRenderMgr::~CRenderMgr()
{
	SAFE_DELETE(m_pLight2DBuffer);
	SAFE_DELETE(m_pLight3DBuffer);

	Safe_Del_Arr(m_arrMRT);

	SAFE_DELETE(m_pMergeShader);
	SAFE_DELETE(m_pMergeMaterial);
}

void CRenderMgr::update() {}

void CRenderMgr::render()
{
	RenderBegin();

	const CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->GetSceneState() == SCENE_STATE::PLAY ? render_play() : render_editor();

	RenderEnd();
}

void CRenderMgr::RenderBegin()
{
	// Rendering ����	
	ClearMRT();

	// TextureRegister �ʱ�ȭ
	ClearTextureRegister();

	// Light ������Ʈ
	UpdateLight2D();
	UpdateLight3D();

	// Global ��� ������Ʈ
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global, sizeof(tGlobal));
	pGlobalCB->UpdateData();
	pGlobalCB->UpdateData_CS();
}

void CRenderMgr::RenderEnd()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::render_play()
{
	if (m_vecCamera.empty())
	{
		return;
	}

	//ī�޶� �������� ������
	for (const auto& pCamera : m_vecCamera)
	{
		if (nullptr == pCamera)
		{
			continue;
		}

		pCamera->SortGameObject();
		// Directional Light ShadowMap �����
		render_shadowmap();

		g_transform.matView    = pCamera->GetViewMat();
		g_transform.matViewInv = pCamera->GetViewInvMat();
		g_transform.matProj    = pCamera->GetProjMat();

		// Deferred ��ü ������			
		m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED)]->OMSet();
		pCamera->render_deferred();


		m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED_DECAL)]->OMSet();
		pCamera->render_deferred_decal();

		// Particle ��ü ������	
		m_arrMRT[static_cast<UINT>(MRT_TYPE::PARTICLE)]->OMSet();
		pCamera->render_particle();


		// ���� ������
		render_lights();

		// Merge
		m_arrMRT[static_cast<UINT>(MRT_TYPE::SWAPCHAIN)]->OMSet();
		Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pMergeMaterial->UpdateData();
		pRectMesh->render(0);

		//������ ������
		pCamera->render_forward();
		pCamera->render_masked();
		pCamera->render_forward_decal();
		pCamera->render_translucent();
		pCamera->render_debug();
		pCamera->render_postprocess();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_pEditorCamera)
	{
		return;
	}

	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	m_pEditorCamera->SortGameObject();

	// Directional Light ShadowMap �����
	render_shadowmap();

	g_transform.matView    = m_pEditorCamera->GetViewMat();
	g_transform.matViewInv = m_pEditorCamera->GetViewInvMat();
	g_transform.matProj    = m_pEditorCamera->GetProjMat();

	// Deferred ��ü ������			
	m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED)]->OMSet();
	m_pEditorCamera->render_deferred();

	m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED_DECAL)]->OMSet();
	m_pEditorCamera->render_deferred_decal();

	// Particle ��ü ������			
	m_arrMRT[static_cast<UINT>(MRT_TYPE::PARTICLE)]->OMSet();
	m_pEditorCamera->render_particle();

	// ���� ������
	render_lights();

	// Merge
	m_arrMRT[static_cast<UINT>(MRT_TYPE::SWAPCHAIN)]->OMSet();
	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	m_pMergeMaterial->UpdateData();
	pRectMesh->render(0);

	//������ ������
	m_pEditorCamera->render_forward();
	m_pEditorCamera->render_masked();
	m_pEditorCamera->render_forward_decal();
	m_pEditorCamera->render_translucent();
	m_pEditorCamera->render_debug();
	m_pEditorCamera->render_postprocess();
}

void CRenderMgr::render_shadowmap() const
{
	m_arrMRT[static_cast<UINT>(MRT_TYPE::SHADOWMAP)]->OMSet();

	for (const auto& pLight3D : m_vecLight3D)
	{
		if (LIGHT_TYPE::DIRECTIONAL == pLight3D->GetLightType())
		{
			pLight3D->render_shadowmap();
		}
	}
}

void CRenderMgr::render_lights() const
{
	m_arrMRT[static_cast<UINT>(MRT_TYPE::LIGHT)]->OMSet();

	for (const auto& pLight3D : m_vecLight3D)
	{
		pLight3D->render();
	}
}

void CRenderMgr::RegisterCamera(CCamera* _pCamera)
{
	// ī�޶� RenderMgr�� ���� ��� �Ǵ� ���
	if (-1 == _pCamera->m_iCamIdx)
	{
		m_vecCamera.push_back(_pCamera);
		const int cameraIndex = static_cast<int>(m_vecCamera.size() - 1);
		_pCamera->m_iCamIdx   = cameraIndex;
	}
	else
	{
		if (m_vecCamera.size() <= _pCamera->m_iCamIdx)
		{
			m_vecCamera.resize(static_cast<size_t>(_pCamera->m_iCamIdx + 1));
		}

		m_vecCamera[_pCamera->m_iCamIdx] = _pCamera;
	}
}

void CRenderMgr::SwapCameraIndex(CCamera* _pTargetCamera, int _iCameraIndexToChange) const
{
	for (size_t cameraIndex = 0; cameraIndex < m_vecCamera.size(); ++cameraIndex)
	{
		if (_pTargetCamera == m_vecCamera[cameraIndex])
		{
			if (nullptr != m_vecCamera[_iCameraIndexToChange])
			{
				m_vecCamera[_iCameraIndexToChange]->m_iCamIdx = static_cast<int>(cameraIndex);
				_pTargetCamera->m_iCamIdx                     = _iCameraIndexToChange;

				return;
			}
		}
	}

	assert(nullptr);
}

void CRenderMgr::CopyTargetToPostProcess()
{
	Ptr<CTexture> pRenderTarget = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pPostProcess  = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");

	CONTEXT->CopyResource(pPostProcess->GetTex2D().Get(), pRenderTarget->GetTex2D().Get());
}

void CRenderMgr::UpdateLight2D() const
{
	//����ȭ���۷� ���� �ѱ�Ƿ�, ũ�Ⱑ �۴ٸ� �� ����
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(static_cast<UINT>(sizeof(tLightInfo)),
		                         static_cast<UINT>(m_vecLight2D.size()),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	//����ȭ ���ۿ� ���� ����
	static vector<tLightInfo> vecLight2DInfo{};
	vecLight2DInfo.clear();

	for (const auto& pLight2D : m_vecLight2D)
	{
		vecLight2DInfo.push_back(pLight2D->GetLightInfo());
	}
	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), static_cast<UINT>(vecLight2DInfo.size()));
	m_pLight2DBuffer->UpdateData(PS, 60);

	//�۷ι� ������ ����
	g_global.Light2DCount = static_cast<int>(m_vecLight2D.size());
}

void CRenderMgr::UpdateLight3D() const
{
	//����ȭ���۷� ���� �ѱ�Ƿ�, ũ�Ⱑ �۴ٸ� �� ����
	if (m_pLight3DBuffer->GetElementCount() < m_vecLight3D.size())
	{
		m_pLight3DBuffer->Create(static_cast<UINT>(sizeof(tLightInfo)),
		                         static_cast<UINT>(m_vecLight3D.size()),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	//����ȭ���ۿ� ���� ����
	static vector<tLightInfo> vecLight3DInfo{};
	vecLight3DInfo.clear();

	for (const auto& pLight3D : m_vecLight3D)
	{
		vecLight3DInfo.push_back(pLight3D->GetLightInfo());
	}
	m_pLight3DBuffer->SetData(vecLight3DInfo.data(), static_cast<UINT>(vecLight3DInfo.size()));
	m_pLight3DBuffer->UpdateData(PS, 61);

	//�۷ι� ������ ����
	g_global.Light3DCount = static_cast<int>(m_vecLight3D.size());
}

// ���� ���� ī�޶� ��������
CCamera* CRenderMgr::GetMainCam() const
{
	if (SCENE_STATE::PLAY == CSceneMgr::GetInst()->GetCurScene()->GetSceneState())
	{
		if (m_vecCamera.empty())
		{
			return nullptr;
		}
		return m_vecCamera[0];
	}

	return m_pEditorCamera;
}

CCamera* CRenderMgr::GetUICamera()
{
	if (nullptr == m_pUICamera)
	{
		const auto iter = std::find_if(m_vecCamera.begin(),
		                               m_vecCamera.end(),
		                               [](const CCamera* pCamera)
		                               {
			                               return pCamera->GetOwner()->GetName() == L"UICamera";
		                               });
		if (iter != m_vecCamera.end())
		{
			m_pUICamera = *iter;
			return m_pUICamera;
		}
		return nullptr;
	}
	return m_pUICamera;
}
