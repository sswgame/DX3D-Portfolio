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
	: m_pEditorCam(nullptr)
	, m_pLight2DBuffer(nullptr)
	, m_pLight3DBuffer(nullptr)
	, m_arrMRT{}
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

void CRenderMgr::update()
{

}

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

}

void CRenderMgr::render_play()
{
	if (m_vecCam.empty())
		return;

	// ���� ī�޶� �������� ������
	CCamera* pMainCam = m_vecCam[0];

	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();


	// Deferred ��ü ������	
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	m_pEditorCam->render_deferred();


	// Merge
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	int a = 0;
	m_pMergeMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	m_pMergeMtrl->UpdateData();
	pRectMesh->render(0);


	// Foward ��ü ������	
	pMainCam->render_forward();

	// Masked ��ü ������
	pMainCam->render_masked();

	// Alpha ��ü ������
	pMainCam->render_translucent();

	// Debug Object ������ 
	pMainCam->render_debug();

	// PostProcess ��ü ������
	pMainCam->render_postprocess();


	// Sub ī�޶� �������� ������
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortGameObject();

		g_transform.matView = m_vecCam[i]->GetViewMat();
		g_transform.matProj = m_vecCam[i]->GetProjMat();

		// Foward ��ü ������
		m_vecCam[i]->render_forward();

		// Masked ��ü ������
		m_vecCam[i]->render_masked();

		// Alpha ��ü ������
		m_vecCam[i]->render_translucent();

		// Debug Object Render
		m_vecCam[i]->render_debug();

		// PostProcess ��ü ������
		pMainCam->render_postprocess();


	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_pEditorCam)
		return;

	// ������ ī�޶� �������� ������
	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	m_pEditorCam->SortGameObject();


	// Directional Light ShadowMap �����
	render_shadowmap();


	g_transform.matView = m_pEditorCam->GetViewMat();
	g_transform.matViewInv = m_pEditorCam->GetViewInvMat();
	g_transform.matProj = m_pEditorCam->GetProjMat();

	// Deferred ��ü ������			
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	m_pEditorCam->render_deferred();

	m_arrMRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->OMSet();
	m_pEditorCam->render_deferred_decal();

	// ���� ������
	render_lights();

	// Merge
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	int a = 0;
	m_pMergeMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	m_pMergeMtrl->UpdateData();
	pRectMesh->render(0);

	// Foward ��ü ������	
	m_pEditorCam->render_forward();

	// Masked ��ü ������
	m_pEditorCam->render_masked();

	// Foward Decal ������
	m_pEditorCam->render_forward_decal();

	// Alpha ��ü ������
	m_pEditorCam->render_translucent();

	// Debug Object ������ 
	m_pEditorCam->render_debug();

	// PostProcess ��ü ������
	m_pEditorCam->render_postprocess();
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
		int iIdx = (int)m_vecCam.size() - 1;
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
				_pCam->m_iCamIdx = _iChangeIdx;

				return;
			}
		}
	}

	assert(nullptr);
}

void CRenderMgr::CopyTargetToPostProcess()
{
	Ptr<CTexture> pRenderTarget = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pPostProcess = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");

	CONTEXT->CopyResource(pPostProcess->GetTex2D().Get(), pRenderTarget->GetTex2D().Get());
}

void CRenderMgr::UpdateLight2D()
{
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create((UINT)sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::READ_ONLY, true, nullptr);
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
		m_pLight3DBuffer->Create((UINT)sizeof(tLightInfo), (UINT)m_vecLight3D.size(), SB_TYPE::READ_ONLY, true, nullptr);
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

// ���� ���� ī�޶� ��������
CCamera* CRenderMgr::GetMainCam()
{
	if (SCENE_STATE::PLAY == CSceneMgr::GetInst()->GetCurScene()->GetSceneState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_pEditorCam;
	}
}
