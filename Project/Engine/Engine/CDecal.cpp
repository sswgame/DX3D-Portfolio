#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
	:
	CRenderComponent(COMPONENT_TYPE::DECAL)
  , m_eType(DECAL_TYPE::CUBE)
{
	SetDecalType(m_eType);
}

CDecal::~CDecal() {}

void CDecal::SetDecalType(DECAL_TYPE _eType)
{
	m_eType = _eType;

	if (DECAL_TYPE::CUBE == m_eType)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	}
	else
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	}
}

void CDecal::SetDeferredLighting(bool _bLighting)
{
	m_bUseDeferredLighting = _bLighting;

	Ptr<CMaterial> pMtrl = nullptr;

	if (m_bUseDeferredLighting)
	{
		pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DeferredDecalMtrl.mtrl");
	}
	else
	{
		pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ForwardDecalMtrl.mtrl");
	}

	SetSharedMaterial(pMtrl, 0);
}

void CDecal::finalupdate() {}

void CDecal::UpdateData()
{
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, (int*)&m_eType);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1, m_pDecalTex);
}

void CDecal::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	UpdateData();

	Transform()->UpdateData();
	GetMaterial(0)->UpdateData();
	GetMesh()->render(0);
}

void CDecal::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	// SkyBox Type
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

void CDecal::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	// SkyBox Type
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetDecalType(m_eType);
}
