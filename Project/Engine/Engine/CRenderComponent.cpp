#include "pch.h"
#include "CRenderComponent.h"

#include "CSceneMgr.h"
#include "CScene.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	:
	CComponent(_type)
  , m_pMesh(nullptr)
  , m_pMtrl(nullptr)
  , m_bDynamicShadow(false)
  , m_bFrustumCulling(false) {}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	:
	CComponent(_origin)
  , m_pMesh(_origin.m_pMesh)
  , m_pMtrl(nullptr)
  , m_pSharedMtrl(_origin.m_pMtrl)
  , m_pDynamicMtrl(nullptr)
  , m_bDynamicShadow(_origin.m_bDynamicShadow)
  , m_bFrustumCulling(_origin.m_bFrustumCulling)
{
	if (nullptr != _origin.m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);
	}
}

CRenderComponent::~CRenderComponent()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}

void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{
	m_pSharedMtrl = _pMtrl;
	m_pMtrl       = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pMtrl = m_pSharedMtrl;

	return m_pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// Play 모드에서만 동작가능
	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() != SCENE_STATE::PLAY)
		return nullptr;

	if (nullptr != m_pDynamicMtrl && m_pDynamicMtrl->GetMasterMtrl() != m_pSharedMtrl)
	{
		CMaterial* pMtrl = m_pDynamicMtrl.Get();
		m_pDynamicMtrl   = nullptr;
		delete pMtrl;
	}

	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->GetMtrlInst();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}


#include "CTransform.h"

void CRenderComponent::render_shadowmap()
{
	Transform()->UpdateData();
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ShadowMap.mtrl");
	pMtrl->UpdateData();

	m_pMesh->render();
}

void CRenderComponent::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	SaveResPtr(m_pMesh, _pFile);
	SaveResPtr(m_pMtrl, _pFile);
	fwrite(&m_bDynamicShadow, 1, 1, _pFile);
	fwrite(&m_bFrustumCulling, 1, 1, _pFile);
}

void CRenderComponent::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	LoadResPtr(m_pMesh, _pFile);

	Ptr<CMaterial> pMtrl;
	LoadResPtr(pMtrl, _pFile);
	SetSharedMaterial(pMtrl);

	fread(&m_bDynamicShadow, 1, 1, _pFile);
	fread(&m_bFrustumCulling, 1, 1, _pFile);
}
