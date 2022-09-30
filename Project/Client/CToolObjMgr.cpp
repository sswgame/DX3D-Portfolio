#include "pch.h"
#include "CToolObjMgr.h"

#include "CGameObjectEx.h"
#include "CCameraEx.h"

#include <Engine/CTransform.h>
#include <Engine/CRenderMgr.h>

#include "CEditorCamScript.h"
#include "CGridScript.h"

CToolObjMgr::CToolObjMgr() {}

CToolObjMgr::~CToolObjMgr()
{
	Safe_Del_Vec(m_vecObj);
}

void CToolObjMgr::init()
{
	// Editor Camera
	CGameObjectEx* pToolObj = new CGameObjectEx;
	pToolObj->SetName(L"EditorCamera");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCameraEx);
	pToolObj->AddComponent(new CEditorCamScript);

	pToolObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pToolObj->Camera()->SetFar(100000);
	pToolObj->Camera()->CheckLayerMaskAll();

	m_vecObj.push_back(pToolObj);

	CRenderMgr::GetInst()->RegisterEditorCamera(pToolObj->Camera());

	// Grid Object
	CGameObjectEx* pGridObj = new CGameObjectEx;

	pGridObj->SetName(L"Grid");
	pGridObj->AddComponent(new CTransform);
	pGridObj->AddComponent(new CMeshRender);
	pGridObj->AddComponent(new CGridScript);

	pGridObj->Transform()->SetRelativeScale(Vec3(100000.f, 100000.f, 1.f));
	pGridObj->Transform()->SetRelativeRotation(Vec3(XM_PI / 2.f, 0.f, 0.f));

	pGridObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGridObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\GridMtrl.mtrl"));

	pGridObj->GetScript<CGridScript>()->SetToolCamera(pToolObj);
	pGridObj->GetScript<CGridScript>()->SetGridColor(Vec3(0.2f, 0.2f, 0.7f));

	m_vecObj.push_back(pGridObj);
}

void CToolObjMgr::progress()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
		return;

	for (size_t i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->update();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->finalupdate();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)
	{
		if (nullptr == m_vecObj[i]->GetRenderComponent()
		    || nullptr == m_vecObj[i]->GetRenderComponent()->GetMesh()
		    || nullptr == m_vecObj[i]->GetRenderComponent()->GetMaterial()
		    || nullptr == m_vecObj[i]->GetRenderComponent()->GetMaterial()->GetShader())
			continue;

		m_vecObj[i]->render();
	}
}
