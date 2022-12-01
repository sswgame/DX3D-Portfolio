#include "pch.h"
#include "CRenderComponent.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_pMesh(nullptr)
	, m_vecMtrls{}
	, m_bDynamicShadow(false)
	, m_bFrustumCulling(false)
{
	m_vecMtrls.resize(1);
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	: CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)
	, m_bDynamicShadow(_origin.m_bDynamicShadow)
	, m_bFrustumCulling(_origin.m_bFrustumCulling)
{
	if (false == _origin.m_vecMtrls.empty())
	{
		m_vecMtrls.resize(_origin.m_vecMtrls.size());
		for (size_t i = 0; i < _origin.m_vecMtrls.size(); ++i)
		{
			SetSharedMaterial(_origin.m_vecMtrls[i].pSharedMtrl, static_cast<UINT>(i));
		}
	}
}

CRenderComponent::~CRenderComponent()
{
	for (size_t i = 0; i < m_vecMtrls.size(); ++i)
	{
		if (nullptr != m_vecMtrls[i].pDynamicMtrl)
			delete m_vecMtrls[i].pDynamicMtrl.Get();
	}
}

void CRenderComponent::SetMesh(Ptr<CMesh> _pMesh)
{
	m_pMesh = _pMesh;

	if (!m_vecMtrls.empty())
	{
		for (size_t i = 0; i < m_vecMtrls.size(); ++i)
		{
			if (nullptr != m_vecMtrls[i].pDynamicMtrl)
				delete m_vecMtrls[i].pDynamicMtrl.Get();
		}

		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	m_vecMtrls.resize(m_pMesh->GetSubsetCount());
}

void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl, UINT _iIdx)
{
	m_vecMtrls[_iIdx].pSharedMtrl = _pMtrl;
	m_vecMtrls[_iIdx].pMtrl       = _pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _iIdx)
{
	if (nullptr == m_vecMtrls[_iIdx].pMtrl)
	{
		m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pSharedMtrl;
	}

	return m_vecMtrls[_iIdx].pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _iIdx)
{
	m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pSharedMtrl;

	return m_vecMtrls[_iIdx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _iIdx)
{
	// Play 모드에서만 동작가능
	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() != SCENE_STATE::PLAY)
	{
		return nullptr;
	}

	if (nullptr != m_vecMtrls[_iIdx].pDynamicMtrl)
	{
		CMaterial* pMtrl = m_vecMtrls[_iIdx].pDynamicMtrl.Get();
		delete(pMtrl);

		m_vecMtrls[_iIdx].pDynamicMtrl = nullptr;
	}

	if (nullptr == m_vecMtrls[_iIdx].pDynamicMtrl)
	{
		m_vecMtrls[_iIdx].pDynamicMtrl = m_vecMtrls[_iIdx].pSharedMtrl->GetMtrlInst();
	}

	m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pDynamicMtrl;

	return m_vecMtrls[_iIdx].pMtrl;
}

bool CRenderComponent::IsUsingDynamicMaterial(UINT _index) const
{
	assert(_index < m_vecMtrls.size());
	return m_vecMtrls[_index].pMtrl.Get() == m_vecMtrls[_index].pDynamicMtrl.Get();
}


void CRenderComponent::render_shadowmap()
{
	Transform()->UpdateData();
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ShadowMap.mtrl");
	pMtrl->UpdateData();

	m_pMesh->render(0);
}

void CRenderComponent::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	SaveResPtr(m_pMesh, _pFile);

	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveResPtr(m_vecMtrls[i].pSharedMtrl, _pFile);
	}


	fwrite(&m_bDynamicShadow, 1, 1, _pFile);
	fwrite(&m_bFrustumCulling, 1, 1, _pFile);
}

void CRenderComponent::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	LoadResPtr(m_pMesh, _pFile);

	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> pMtrl;
		LoadResPtr(pMtrl, _pFile);
		SetSharedMaterial(pMtrl, i);
	}

	fread(&m_bDynamicShadow, 1, 1, _pFile);
	fread(&m_bFrustumCulling, 1, 1, _pFile);
}

void CRenderComponent::Serialize(YAML::Emitter& emitter)
{
	if (nullptr != m_pMesh)
	{
		CRes& mesh = *m_pMesh.Get();
		emitter << YAML::Key << "MESH" << YAML::Value << mesh;
	}
	else
	{
		emitter << YAML::Key << "MESH" << YAML::Value << "";
	}

	int materialCount = GetMtrlCount();
	emitter << YAML::Key << "MATERIAL COUNT" << YAML::Value << materialCount;
	for (int i = 0; i < materialCount; ++i)
	{
		CRes& material = *m_vecMtrls[i].pSharedMtrl.Get();
		emitter << YAML::Key << i << YAML::Value << material;
	}
	emitter << YAML::Key << NAME_OF(m_bDynamicShadow) << YAML::Value << m_bDynamicShadow;
	emitter << YAML::Key << NAME_OF(m_bFrustumCulling) << YAML::Value << m_bFrustumCulling;
}

void CRenderComponent::Deserialize(const YAML::Node& node)
{
	m_pMesh           = LoadAs<CMesh>(node["MESH"]);
	int materialCount = node["MATERIAL COUNT"].as<int>();

	m_vecMtrls.resize(materialCount);

	for (int i = 0; i < materialCount; ++i)
	{
		Ptr<CMaterial> pMaterial = LoadAs<CMaterial>(node[i]);
		SetSharedMaterial(pMaterial, i);
	}
	m_bDynamicShadow  = node[NAME_OF(m_bDynamicShadow)].as<bool>();
	m_bFrustumCulling = node[NAME_OF(m_bFrustumCulling)].as<bool>();
}
