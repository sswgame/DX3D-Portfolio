#include "pch.h"
#include "CSceneMgr.h"

#include "CCamera.h"
#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

#include "CScene.h"
#include "CLayer.h"
#include "CLight3D.h"
#include "CRenderMgr.h"
#include "CSkyBox.h"


CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr) {}

CSceneMgr::~CSceneMgr()
{
	//SAFE_DELETE(m_pCurScene);

	Safe_Del_Vec(m_vecScene);
}


void CSceneMgr::init()
{
	MakeToolScene();
}

void CSceneMgr::progress()
{
	if (nullptr == m_pCurScene)
	{
		return;
	}

	SCENE_STATE eState = m_pCurScene->GetSceneState();

	if (SCENE_STATE::PLAY == eState)
	{
		m_pCurScene->update();
		m_pCurScene->lateupdate();
	}
	ClearLayer();
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();

	// Collision Check
	if (SCENE_STATE::PLAY == eState)
	{
		CCollisionMgr::GetInst()->update();
	}
}


void CSceneMgr::MakeToolScene()
{
	m_pToolScene = new CScene{};
	ChangeScene(m_pToolScene);

	m_pToolScene->SetLayerName(0, L"BG");
	m_pToolScene->SetLayerName(1, L"BG_OBJ");
	m_pToolScene->SetLayerName(2, L"ITEM");
	m_pToolScene->SetLayerName(3, L"PLAYER");
	m_pToolScene->SetLayerName(4, L"PLAYER_ATTACK");
	m_pToolScene->SetLayerName(5, L"MONSTER");
	m_pToolScene->SetLayerName(6, L"MONSTER_NON-PARRING-ATTACK");
	m_pToolScene->SetLayerName(7, L"MONSTER_PARRING-ATTACK");
	m_pToolScene->SetLayerName(8, L"OBJECT-MGR");
	m_pToolScene->SetLayerName(9, L"CAMERA");
	m_pToolScene->SetLayerName(30, L"UI_STATIC");
	m_pToolScene->SetLayerName(31, L"UI_INTERACTIVE");

	const auto pDirectionalLight = new CGameObject;
	pDirectionalLight->SetName(L"Directional Light");
	pDirectionalLight->AddComponent(new CTransform);
	pDirectionalLight->AddComponent(new CLight3D);
	pDirectionalLight->Transform()->SetRelativePos(Vec3(-1000.f, 1000.f, 0.f));
	pDirectionalLight->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pDirectionalLight->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pDirectionalLight->Light3D()->SetRange(1000.f);
	pDirectionalLight->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pDirectionalLight->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pDirectionalLight->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	m_pToolScene->AddObject(pDirectionalLight, L"BG_OBJ");

	const Ptr<CTexture> pSkyTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds",
	                                                                 L"texture\\skybox\\SkyDawn.dds");
	const auto pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->SkyBox()->SetSkyboxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_CUBE_0, pSkyTex);
	pSkyBox->SkyBox()->SetFrustumCulling(false);
	m_pToolScene->AddObject(pSkyBox, L"BG");

	CGameObject* pToolObj = new CGameObject;
	pToolObj->SetName(L"UICamera");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCamera);

	pToolObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pToolObj->Camera()->SetFar(100000);
	pToolObj->Camera()->CheckLayerMask(L"BG");
	pToolObj->Camera()->CheckLayerMask(L"BG_OBJ");
	pToolObj->Camera()->CheckLayerMask(L"UI_STATIC");
	pToolObj->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
	m_pToolScene->AddObject(pToolObj, L"CAMERA");
	CRenderMgr::GetInst()->SetToolCamera(pToolObj->Camera(), TOOL_SCENE::UI);

	pToolObj = new CGameObject;
	pToolObj->SetName(L"TOOL_PARTICLE_CAMERA");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCamera);
	pToolObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pToolObj->Camera()->SetFar(100000);
	pToolObj->Camera()->CheckLayerMaskAll();
	pToolObj->Camera()->CheckLayerMask(L"UI_STATIC");
	pToolObj->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
	m_pToolScene->AddObject(pToolObj, L"CAMERA");
	CRenderMgr::GetInst()->SetToolCamera(pToolObj->Camera(), TOOL_SCENE::PARTICLE);

	pToolObj = new CGameObject;
	pToolObj->SetName(L"TOOL_NAV_CAMERA");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCamera);
	pToolObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pToolObj->Camera()->SetFar(100000);
	pToolObj->Camera()->CheckLayerMaskAll();
	pToolObj->Camera()->CheckLayerMask(L"UI_STATIC");
	pToolObj->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
	m_pToolScene->AddObject(pToolObj, L"CAMERA");
	CRenderMgr::GetInst()->SetToolCamera(pToolObj->Camera(), TOOL_SCENE::NAV_MESH);

	pToolObj = new CGameObject;
	pToolObj->SetName(L"TOOL_EFFECT_CAMERA");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCamera);
	pToolObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pToolObj->Camera()->SetFar(100000);
	pToolObj->Camera()->CheckLayerMaskAll();
	pToolObj->Camera()->CheckLayerMask(L"UI_STATIC");
	pToolObj->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
	m_pToolScene->AddObject(pToolObj, L"CAMERA");
	CRenderMgr::GetInst()->SetToolCamera(pToolObj->Camera(), TOOL_SCENE::RENDER_EFFECT);
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	/*if (nullptr != m_pCurScene)
		delete m_pCurScene;*/

	if (nullptr != m_pCurScene && m_pCurScene == _pNextScene)
	{
		return;
	}

	auto iter = std::find(m_vecScene.begin(), m_vecScene.end(), _pNextScene);
	if (iter == m_vecScene.end())
	{
		m_vecScene.push_back(_pNextScene);
	}
	m_pCurScene = _pNextScene;
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::CREATE_OBJ;
	info.lParam     = (DWORD_PTR)_pSpawnObject;
	info.wParam     = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetRelativePos(_vWorldPos);
	_pSpawnObject->SetName(_strName);

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::CREATE_OBJ;
	info.lParam     = (DWORD_PTR)_pSpawnObject;
	info.wParam     = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, wstring _strLayerName)
{
	UINT iLayerIdx = m_pCurScene->GetLayerIdxFromName(_strLayerName);

	tEventInfo info = {};
	info.eType      = EVENT_TYPE::CREATE_OBJ;
	info.lParam     = (DWORD_PTR)_pSpawnObject;
	info.wParam     = (DWORD_PTR)iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::ADD_CHILD;
	info.lParam     = (DWORD_PTR)_pParentObject;
	info.wParam     = (DWORD_PTR)_pChildObject;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::DisconnectParent(CGameObject* _pObject)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::DISCONNECT_PARENT;
	info.lParam     = (DWORD_PTR)_pObject;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::ChangeObjectLayerIndex(CGameObject* _pObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::CHANGE_OBJ_LAYER_INDEX;
	info.lParam     = (DWORD_PTR)_pObject;
	info.wParam     = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SwapLayer(UINT _iLayerIdx_1, UINT _iLayerIdx_2)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::SWAP_LAYER;
	info.lParam     = (DWORD_PTR)_iLayerIdx_1;
	info.wParam     = (DWORD_PTR)_iLayerIdx_2;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}

void CSceneMgr::DeleteObject(CGameObject* _pDestroyObject)
{
	tEventInfo info = {};
	info.eType      = EVENT_TYPE::DELETE_OBJ;
	info.lParam     = (DWORD_PTR)_pDestroyObject;

	CEventMgr::GetInst()->AddEvent(info);
}

CGameObject* CSceneMgr::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer*                     pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				return vecObj[j];
			}
		}
	}

	return nullptr;
}

void CSceneMgr::FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer*                     pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				_vecOut.push_back(vecObj[j]);
			}
		}
	}
}
