#include "pch.h"
#include "CNaviMap.h"

#include "CNaviMapData.h"

#include "CTransform.h"
#include"CMeshRender.h"

CNaviMap::CNaviMap()
	: CComponent{COMPONENT_TYPE::NAVIMAP}
	, m_pNeviMapData(nullptr)

{
}

CNaviMap::CNaviMap(const CNaviMap& _origin)
	: CComponent{COMPONENT_TYPE::NAVIMAP}
	, m_pNeviMapData(nullptr)
{
}

CNaviMap::~CNaviMap()
{
}


void CNaviMap::SetNaviMapData(CNaviMapData* _pData)
{
	m_pNeviMapData = _pData;

	m_pMesh     = _pData->GetNaviMesh();
	m_pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3D_DeferredMtrl.mtrl");

	if (nullptr == m_pDebugObj)
	{
		m_pDebugObj = new CGameObject;
		m_pDebugObj->SetName(L"NaviMap_debug");
		m_pDebugObj->AddComponent(new CTransform);
		m_pDebugObj->AddComponent(new CMeshRender);

		m_pDebugObj->MeshRender()->SetMesh(m_pMesh);
		m_pDebugObj->MeshRender()->SetSharedMaterial(m_pMaterial, 0);
	}

	m_pDebugObj->MeshRender()->SetMesh(m_pMesh);
	m_pDebugObj->MeshRender()->SetSharedMaterial(m_pMaterial, 0);
}

bool CNaviMap::CheckIntersect(int _iCellidx, Vec3 _vPos, Vec3 _vDir, float& _fDist)
{
	CNaviCell* pCell = m_pNeviMapData->GetCell(_iCellidx);
	Vec3       vV1   = pCell->GetVTX()[0].vPos;
	Vec3       vV2   = pCell->GetVTX()[1].vPos;
	Vec3       vV3   = pCell->GetVTX()[2].vPos;

	vV1 = XMVector3Transform(vV1, Transform()->GetWorldMat());
	vV2 = XMVector3Transform(vV2, Transform()->GetWorldMat());
	vV3 = XMVector3Transform(vV3, Transform()->GetWorldMat());

	return DirectX::TriangleTests::Intersects(_vPos, _vDir, vV1, vV2, vV3, _fDist);
}

void CNaviMap::UpdateData()
{
	Transform()->UpdateData();
}

void CNaviMap::finalupdate()
{
	UpdateData();
}

void CNaviMap::finalupdate_debug()
{
	if (nullptr != m_pDebugObj)
	{
		m_pDebugObj->Transform()->SetRelativePos(Transform()->GetWorldPos());
		m_pDebugObj->Transform()->SetRelativeScale(Transform()->GetRelativeScale());
		m_pDebugObj->Transform()->SetRelativeRotation(DecomposeRotMat(Transform()->GetWorldRotation()));

		m_pDebugObj->finalupdate();
	}
}

void CNaviMap::render_debug()
{
	if (nullptr != m_pDebugObj)
	{
		m_pDebugObj->MeshRender()->render();
	}
}

void CNaviMap::Serialize(YAML::Emitter& emitter)
{
}

void CNaviMap::Deserialize(const YAML::Node& node)
{
}
