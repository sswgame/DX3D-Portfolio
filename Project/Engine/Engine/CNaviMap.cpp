#include "pch.h"
#include "CNaviMap.h"

#include "CNaviMapData.h"

#include "CTransform.h"
#include"CMeshRender.h"
#include "CSerializer.h"

CNaviMap::CNaviMap()
	: CComponent{COMPONENT_TYPE::NAVIMAP}
	, m_pNeviMapData(nullptr)
	, m_vDebugLineColor(0.f, 0.f, 0.f, 1.f)
	, m_vDebugMeshColor(0.1f, 0.1, 0.1f, 0.6f)

{
}

CNaviMap::CNaviMap(const CNaviMap& _origin)
	: CComponent{COMPONENT_TYPE::NAVIMAP}
	, m_pNeviMapData(nullptr)
	, m_vDebugLineColor(_origin.m_vDebugLineColor)
	, m_vDebugMeshColor(_origin.m_vDebugMeshColor)
{
}

CNaviMap::~CNaviMap()
{
}


void CNaviMap::SetNaviMapData(CNaviMapData* _pData)
{
	m_pNeviMapData = _pData;

	Ptr<CMesh>     p_mesh   = _pData->GetNaviMesh();
	Ptr<CMaterial> p_mtrl_1 = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DAlphaMtrl.mtrl");
	Ptr<CMaterial> p_mtrl_2 = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DWireShader.mtrl");

	if (nullptr == m_pDebugObj)
	{
		m_pDebugObj = new CGameObject;
		m_pDebugObj->SetName(L"NaviMap_debug");
		m_pDebugObj->AddComponent(new CTransform);
		m_pDebugObj->AddComponent(new CMeshRender);

		m_pDebugObj->MeshRender()->SetMesh(p_mesh);
		m_pDebugObj->MeshRender()->SetSharedMaterial(p_mtrl_1, 0);


		CGameObject* ChildDebug = new CGameObject;
		ChildDebug->SetName(L"NaviMap_debug_wire");
		ChildDebug->AddComponent(new CTransform);
		ChildDebug->AddComponent(new CMeshRender);

		ChildDebug->MeshRender()->SetMesh(p_mesh);
		ChildDebug->MeshRender()->SetSharedMaterial(p_mtrl_2, 0);
		m_pDebugObj->AddChild(ChildDebug);
	}
	else
	{
		m_pDebugObj->MeshRender()->SetMesh(p_mesh);
		m_pDebugObj->MeshRender()->SetSharedMaterial(p_mtrl_1, 0);

		CGameObject* ChildDebug = m_pDebugObj->GetChild(L"NaviMap_debug_wire");
		if (nullptr == ChildDebug)
		{
			ChildDebug = new CGameObject;
			ChildDebug->SetName(L"NaviMap_debug_wire");
			ChildDebug->AddComponent(new CTransform);
			ChildDebug->AddComponent(new CMeshRender);
				
			m_pDebugObj->AddChild(ChildDebug);
		}
		ChildDebug->MeshRender()->SetMesh(p_mesh);
		ChildDebug->MeshRender()->SetSharedMaterial(p_mtrl_2, 0);
	}
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
		if (nullptr != m_pDebugObj->MeshRender()->GetDynamicMaterial(0))
			m_pDebugObj->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vDebugMeshColor);


		CGameObject* ChildDebug = m_pDebugObj->GetChild(L"NaviMap_debug_wire");
		if (nullptr != ChildDebug->MeshRender()->GetDynamicMaterial(0))
			ChildDebug->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vDebugLineColor);

		m_pDebugObj->finalupdate();
	}
}

void CNaviMap::render_debug()
{
	if (nullptr != m_pDebugObj)
	{
		m_pDebugObj->MeshRender()->render();

		CGameObject* ChildDebug = m_pDebugObj->GetChild(L"NaviMap_debug_wire");
		ChildDebug->MeshRender()->render();
	}
}

void CNaviMap::Serialize(YAML::Emitter& emitter)
{
}

void CNaviMap::Deserialize(const YAML::Node& node)
{
}
