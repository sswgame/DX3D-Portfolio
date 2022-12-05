#include "pch.h"
#include "FogTool.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CSceneMgr.h>

FogTool::FogTool()
	:UI("##FogTool")
	, m_pFog(nullptr)
	, m_bFogOn(true)
{
}

FogTool::~FogTool()
{
}

void FogTool::SetData()
{
}

void FogTool::update()
{

	if (nullptr == m_pFog)
	{
		m_pFog = new CGameObject;
		m_pFog->SetName(L"Depth of Field");
		m_pFog->AddComponent(new CTransform);
		m_pFog->AddComponent(new CMeshRender);

		CMesh* pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
		CMaterial* pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\FogPostProcessMtrl.mtrl").Get();
		Ptr<CTexture> pPostProcTarget = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex").Get();
		Ptr<CTexture> pPositionTarget = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex").Get();

		pMtrl->SetTexParam(TEX_PARAM::TEX_0, pPostProcTarget);
		pMtrl->SetTexParam(TEX_PARAM::TEX_1, pPositionTarget);

		m_pFog->MeshRender()->SetMesh(pMesh);
		m_pFog->MeshRender()->SetSharedMaterial(pMtrl, 0);
		CSceneMgr::GetInst()->SpawnObject(m_pFog, L"BG");

	}
	else
	{
		if (m_pFog->IsDead())
			return;

		if (m_bFogOn)
		{
			if (m_pFog->IsActive() == false)
				m_pFog->Activate();
		}
		else
		{
			if (m_pFog->IsActive() == true)
				m_pFog->Deactivate();
		}
	}


}

void FogTool::render_update()
{
	SetData();

	ImGui::Text("Turn On DoF : "); ImGui::SameLine();
	ImGui::Checkbox("##DOF_OnOff", &m_bFogOn);
}
