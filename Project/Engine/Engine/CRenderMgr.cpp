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
	// Rendering 시작	
	ClearMRT();

	// TextureRegister 초기화
	ClearTextureRegister();

	// Light 업데이트
	UpdateLight2D();
	UpdateLight3D();

	// Global 상수 업데이트
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

	//카메라 시점으로 렌더링
	for (const auto& pCamera : m_vecCamera)
	{
		if (nullptr == pCamera)
		{
			continue;
		}

		pCamera->SortGameObject();
		// Directional Light ShadowMap 만들기
		render_shadowmap();

		g_transform.matView    = pCamera->GetViewMat();
		g_transform.matViewInv = pCamera->GetViewInvMat();
		g_transform.matProj    = pCamera->GetProjMat();

		// Deferred 물체 렌더링			
		m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED)]->OMSet();
		pCamera->render_deferred();


		m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED_DECAL)]->OMSet();
		pCamera->render_deferred_decal();

		// Particle 물체 렌더링	
		m_arrMRT[static_cast<UINT>(MRT_TYPE::PARTICLE)]->OMSet();
		pCamera->render_particle();


		// 광원 렌더링
		render_lights();

		// Merge
		m_arrMRT[static_cast<UINT>(MRT_TYPE::SWAPCHAIN)]->OMSet();
		Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pMergeMaterial->UpdateData();
		pRectMesh->render(0);

		//포워드 렌더링
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

	// Camera 가 찍는 Layer 의 오브젝트들을 Shader Domain 에 따라 분류해둠
	m_pEditorCamera->SortGameObject();

	// Directional Light ShadowMap 만들기
	render_shadowmap();

	g_transform.matView    = m_pEditorCamera->GetViewMat();
	g_transform.matViewInv = m_pEditorCamera->GetViewInvMat();
	g_transform.matProj    = m_pEditorCamera->GetProjMat();

	// Deferred 물체 렌더링			
	m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED)]->OMSet();
	m_pEditorCamera->render_deferred();

	m_arrMRT[static_cast<UINT>(MRT_TYPE::DEFERRED_DECAL)]->OMSet();
	m_pEditorCamera->render_deferred_decal();

	// Particle 물체 렌더링			
	m_arrMRT[static_cast<UINT>(MRT_TYPE::PARTICLE)]->OMSet();
	m_pEditorCamera->render_particle();

	// 광원 렌더링
	render_lights();

	// Merge
	m_arrMRT[static_cast<UINT>(MRT_TYPE::SWAPCHAIN)]->OMSet();
	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	m_pMergeMaterial->UpdateData();
	pRectMesh->render(0);

	//포워드 렌더링
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
	// 카메라가 RenderMgr에 최초 등록 되는 경우
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
	//구조화버퍼로 값을 넘기므로, 크기가 작다면 재 생성
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(static_cast<UINT>(sizeof(tLightInfo)),
		                         static_cast<UINT>(m_vecLight2D.size()),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	//구조화 버퍼에 값을 갱신
	static vector<tLightInfo> vecLight2DInfo{};
	vecLight2DInfo.clear();

	for (const auto& pLight2D : m_vecLight2D)
	{
		vecLight2DInfo.push_back(pLight2D->GetLightInfo());
	}
	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), static_cast<UINT>(vecLight2DInfo.size()));
	m_pLight2DBuffer->UpdateData(PS, 60);

	//글로벌 데이터 갱신
	g_global.Light2DCount = static_cast<int>(m_vecLight2D.size());
}

void CRenderMgr::UpdateLight3D() const
{
	//구조화버퍼로 값을 넘기므로, 크기가 작다면 재 생성
	if (m_pLight3DBuffer->GetElementCount() < m_vecLight3D.size())
	{
		m_pLight3DBuffer->Create(static_cast<UINT>(sizeof(tLightInfo)),
		                         static_cast<UINT>(m_vecLight3D.size()),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	//구조화버퍼에 값을 세팅
	static vector<tLightInfo> vecLight3DInfo{};
	vecLight3DInfo.clear();

	for (const auto& pLight3D : m_vecLight3D)
	{
		vecLight3DInfo.push_back(pLight3D->GetLightInfo());
	}
	m_pLight3DBuffer->SetData(vecLight3DInfo.data(), static_cast<UINT>(vecLight3DInfo.size()));
	m_pLight3DBuffer->UpdateData(PS, 61);

	//글로벌 데이터 갱신
	g_global.Light3DCount = static_cast<int>(m_vecLight3D.size());
}

// 현재 시점 카메라 가져오기
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
