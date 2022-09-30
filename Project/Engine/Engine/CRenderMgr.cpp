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
	:
	m_pEditorCam(nullptr)
  , m_pLight2DBuffer(nullptr)
  , m_pLight3DBuffer(nullptr)
  , m_arrMRT{}
  , m_pMergeShader{nullptr}
  , m_pMergeMtrl{nullptr}
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
	SAFE_DELETE(m_pMergeMtrl);
}

void CRenderMgr::update() {}

void CRenderMgr::render()
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


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene->GetSceneState() == SCENE_STATE::PLAY)
	{
		render_play();
	}

	else
	{
		render_editor();
	}

	m_vecLight2D.clear();
	m_vecLight3D.clear();

	// todo - Render Frustum �κ� �߰� 
	if (m_pEditorCam != nullptr)
		m_pEditorCam->render_frustum();

	CCamera* pMainCam = m_vecCam[0];
	if (pMainCam != nullptr && pCurScene->GetSceneState() == SCENE_STATE::STOP)
		pMainCam->render_frustum();
}

void CRenderMgr::CameraRender(CCamera* _cam)
{
	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	_cam->SortGameObject();

	// Directional Light ShadowMap �����
	render_shadowmap();

	g_transform.matView    = _cam->GetViewMat();
	g_transform.matViewInv = _cam->GetViewInvMat();
	g_transform.matProj    = _cam->GetProjMat();

	// Deferred ��ü ������	
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	_cam->render_deferred();

	m_arrMRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->OMSet();
	_cam->render_deferred_decal();

	// ���� ������
	render_lights();

	// Merge
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	int        a         = 0;
	m_pMergeMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	m_pMergeMtrl->UpdateData();
	pRectMesh->render();

	// Foward ��ü ������	
	_cam->render_forward();

	// Foward Decal ������
	_cam->render_forward_decal();

	// Masked ��ü ������
	_cam->render_masked();

	// Alpha ��ü ������
	_cam->render_translucent();

	// PostProcess ��ü ������
	_cam->render_postprocess();
}

void CRenderMgr::render_play()
{
	if (m_vecCam.empty())
		return;

	// ���� ī�޶� �������� ������
	CCamera* pMainCam = m_vecCam[0];

	// �߰�
	CameraRender(pMainCam);

	// Sub ī�޶� �������� ������
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		// �߰�
		CameraRender(m_vecCam[i]);
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_pEditorCam)
		return;

	CameraRender(m_pEditorCam);
}

void CRenderMgr::render_shadowmap()
{
	m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		if (LIGHT_TYPE::DIRECTIONAL == m_vecLight3D[i]->GetLightType())
			m_vecLight3D[i]->render_shadowmap();
	}
}

void CRenderMgr::render_lights()
{
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->render();
	}
}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// ī�޶� RenderMgr�� ���� ��� �Ǵ� ���
	if (-1 == _pCam->m_iCamIdx)
	{
		m_vecCam.push_back(_pCam);
		int iIdx         = (int)m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}
	else
	{
		if (m_vecCam.size() <= _pCam->m_iCamIdx)
		{
			m_vecCam.resize((size_t)_pCam->m_iCamIdx + 1);
		}

		m_vecCam[_pCam->m_iCamIdx] = _pCam;
	}
}

void CRenderMgr::SwapCameraIndex(CCamera* _pCam, int _iChangeIdx)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (_pCam == m_vecCam[i])
		{
			if (nullptr != m_vecCam[_iChangeIdx])
			{
				m_vecCam[_iChangeIdx]->m_iCamIdx = (int)i;
				_pCam->m_iCamIdx                 = _iChangeIdx;

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

void CRenderMgr::UpdateLight2D()
{
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create((UINT)sizeof(tLightInfo),
		                         (UINT)m_vecLight2D.size(),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	static vector<tLightInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}
	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	m_pLight2DBuffer->UpdateData(PIPELINE_STAGE::PS, 60);

	g_global.Light2DCount = (int)m_vecLight2D.size();
}

void CRenderMgr::UpdateLight3D()
{
	if (m_pLight3DBuffer->GetElementCount() < m_vecLight3D.size())
	{
		m_pLight3DBuffer->Create((UINT)sizeof(tLightInfo),
		                         (UINT)m_vecLight3D.size(),
		                         SB_TYPE::READ_ONLY,
		                         true,
		                         nullptr);
	}

	static vector<tLightInfo> vecLight3DInfo;
	vecLight3DInfo.clear();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
	}
	m_pLight3DBuffer->SetData(vecLight3DInfo.data(), (UINT)vecLight3DInfo.size());
	m_pLight3DBuffer->UpdateData(PIPELINE_STAGE::PS, 61);

	g_global.Light3DCount = (int)m_vecLight3D.size();
}
