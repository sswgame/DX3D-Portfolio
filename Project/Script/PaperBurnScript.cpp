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
	int MtrlCnt = GetOwner()->MeshRender()->GetMtrlCount();
	for (int i = 0; i < MtrlCnt; ++i)
	{
		GetOwner()->MeshRender()->SetUseDynamicMaterial(i, true);
		GetOwner()->MeshRender()->GetDynamicMaterial(i)->CreateDynamicShader();

	}

}

void PaperBurnScript::update()
{
	if (KEY_TAP(KEY::O))
	{
		m_bApply = !m_bApply;
		if (m_bApply)
		{
			m_fStrength = 0.f;
			int MtrlCnt = GetOwner()->MeshRender()->GetMtrlCount();
			for (int i = 0; i < MtrlCnt; ++i)
			{
				GetOwner()->MeshRender()->SetUseDynamicMaterial(i, true);
				GetOwner()->MeshRender()->GetDynamicMaterial(i)->SetUseDynamicShader(true);
				GetOwner()->MeshRender()->GetDynamicMaterial(i)->GetDynamicShader()->SetBSType(BS_TYPE::NO_ALPHA_COVERAGE);

			}
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
		int MtrlCnt = GetOwner()->MeshRender()->GetMtrlCount();
		for (int i = 0; i < MtrlCnt; ++i)
		{
			pMeshRender->GetMaterial(i)->SetScalarParam(SCALAR_PARAM::INT_0, &m_bApply);
			pMeshRender->GetMaterial(i)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fStrength);
			pMeshRender->GetMaterial(i)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);
		}


		if (m_fStrength >= 3.f)
		{
			m_bApply = false;
			m_fStrength = 0.f;

			int MtrlCnt = GetOwner()->MeshRender()->GetMtrlCount();
			for (int i = 0; i < MtrlCnt; ++i)
			{
				pMeshRender->GetMaterial(i)->SetScalarParam(SCALAR_PARAM::INT_0, &m_bApply);
				GetOwner()->MeshRender()->GetDynamicMaterial(i)->GetDynamicShader()->SetBSType(BS_TYPE::DEFAULT);
				GetOwner()->MeshRender()->GetDynamicMaterial(i)->SetUseDynamicShader(false);
				GetOwner()->MeshRender()->SetUseDynamicMaterial(i, false);

			}

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


