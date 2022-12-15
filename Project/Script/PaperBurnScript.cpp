#include "pch.h"
#include "PaperBurnScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTexture.h>
#include <Engine/CKeyMgr.h>


PaperBurnScript::PaperBurnScript()
	: CScript((int)SCRIPT_TYPE::PAPERBURNSCRIPT)
	, m_bApply(false)
	, m_vColor(WHITE)
	, m_fStrength(0.f)

{
}

PaperBurnScript::PaperBurnScript(const PaperBurnScript& _origin)
	: CScript((int)SCRIPT_TYPE::PAPERBURNSCRIPT)

{
}


PaperBurnScript::~PaperBurnScript()
{
}

void PaperBurnScript::start()
{
	GetOwner()->MeshRender()->SetUseDynamicMaterial(0, true);
	GetOwner()->MeshRender()->GetDynamicMaterial(0)->CreateDynamicShader();

}

void PaperBurnScript::update()
{
	if (KEY_TAP(KEY::O))
	{
		m_bApply = !m_bApply;
		if (m_bApply)
		{
			m_fStrength = 0.f;

			GetOwner()->MeshRender()->SetUseDynamicMaterial(0, true);
			GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetUseDynamicShader(true);
			GetOwner()->MeshRender()->GetDynamicMaterial(0)->GetDynamicShader()->SetBSType(BS_TYPE::NO_ALPHA_COVERAGE);
		}
	}

	if (!m_bApply)
		return;


	m_fStrength += DT;
	if (!GetOwner()->GetRenderComponent())
		return;

	CMeshRender* pMeshRender = GetOwner()->MeshRender();
	if (pMeshRender)
	{

		pMeshRender->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_bApply);
		pMeshRender->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fStrength);
		pMeshRender->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);

		if (m_fStrength >= 3.f)
		{
			m_bApply = false;
			m_fStrength = 0.f;
			pMeshRender->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_bApply);
			GetOwner()->MeshRender()->GetDynamicMaterial(0)->GetDynamicShader()->SetBSType(BS_TYPE::DEFAULT);
			GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetUseDynamicShader(false);
			GetOwner()->MeshRender()->SetUseDynamicMaterial(0, false);
		}

	}

}

void PaperBurnScript::lateupdate()
{

}

void PaperBurnScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void PaperBurnScript::OnCollision(CGameObject* _OtherObject)
{
}

void PaperBurnScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void PaperBurnScript::SaveToScene(FILE* _pFile)
{
}

void PaperBurnScript::LoadFromScene(FILE* _pFile)
{
}


