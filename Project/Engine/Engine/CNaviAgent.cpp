#include "pch.h"
#include "CNaviAgent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CNaviMap.h"
#include "CNaviMapData.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CRigidBody.h"
#include "CSerializer.h"

CNaviAgent::CNaviAgent()
	: CComponent{COMPONENT_TYPE::NAVIAGENT}
	, m_pTargetNaviMap(nullptr)
	, m_pRigidBody(nullptr)
	, m_iCurCellIndex(-1)
	, m_bGravity(false)
	, m_vDebugAgentColor(0.6f, 0.8f, 1.f, 0.6f)
{
	// Debug Obj 추가
	if (nullptr == m_pDebugObj)
	{
		m_pDebugObj = new CGameObject;
		m_pDebugObj->SetName(L"NaviAgent_debug");
		m_pDebugObj->AddComponent(new CTransform);
		m_pDebugObj->AddComponent(new CMeshRender);

		Ptr<CMesh>     pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DWireShader.mtrl");

		m_pDebugObj->MeshRender()->SetMesh(pMesh);
		m_pDebugObj->MeshRender()->SetSharedMaterial(pMtrl, 0);
	}
}

CNaviAgent::CNaviAgent(const CNaviAgent& _origin)
	: CComponent{COMPONENT_TYPE::NAVIAGENT}
	, m_pTargetNaviMap(_origin.m_pTargetNaviMap)
	, m_pRigidBody(_origin.m_pRigidBody)
	, m_iCurCellIndex(-1)
	, m_bGravity(false)
	, m_vDebugAgentColor(_origin.m_vDebugAgentColor)
{
}

CNaviAgent::~CNaviAgent()
{
}

void CNaviAgent::FindCurIndex()
{
	// 맵의 모든 셀을 탐색하면서 현재 셀을 찾는다
	CNaviMapData* pData = m_pTargetNaviMap->GetNaviMapData();
	Vec3          vPos  = Transform()->GetWorldPos();

	for (size_t i = 0; i < pData->GetCellCount(); ++i)
	{
		float tmp;
		if (m_pTargetNaviMap->CheckIntersect(static_cast<int>(i), vPos, Vec3(0.f, -1.f, 0.f), tmp))
		{
			m_iCurCellIndex = static_cast<int>(i);
			return;
		}
	}
}

bool CNaviAgent::CheckLinkedCell()
{
	if (-1 == m_iCurCellIndex)
		assert(nullptr && "현재 충돌중인 Cell이 없습니다.");

	// 인접한 셀과 충돌 여부를 체크한다.
	CNaviMapData* pData = m_pTargetNaviMap->GetNaviMapData();
	Vec3          vPos  = Transform()->GetWorldPos() + m_vOffsetPos;

	vector<int> vecLinkedCell = pData->GetCell(m_iCurCellIndex)->GetLinkedCellIDX();
	for (size_t i = 0; i < vecLinkedCell.size(); ++i)
	{
		float fDist;
		if (m_pTargetNaviMap->CheckIntersect(vecLinkedCell[i], vPos, Vec3(0.f, -1.f, 0.f), fDist))
		{
			m_iCurCellIndex = vecLinkedCell[i];
			m_vPrevPos      = Transform()->GetWorldPos();
			return true;
		}
	}

	return false;
}

Vec3 CNaviAgent::SetYPos(float _fDist)
{
	Vec3  vPos   = Transform()->GetWorldPos();
	float fSpace = _fDist - m_vOffsetSize.y;

	if (fSpace < 0.f)
	{
		Transform()->SetMovePosition(0.f, -fSpace, 0.f);
		vPos.y -= fSpace;

		if (nullptr != m_pRigidBody)
			m_pRigidBody->SetOnGround(true);
	}
	else
	{
		if (nullptr != m_pRigidBody && 0 != fSpace)
			m_pRigidBody->SetOnGround(false);
	}

	//if (10.f < fSpace)
	//	m_iCurCellIndex = -1;

	return vPos;
}

void CNaviAgent::SetNaviMap(CNaviMap* _pNaviMap)
{
	m_pTargetNaviMap = _pNaviMap;
	m_iCurCellIndex  = -1;
}

void CNaviAgent::update()
{
	if (nullptr == m_pTargetNaviMap)
		return;

	if (nullptr == m_pRigidBody)
		m_pRigidBody = GetOwner()->RigidBody();

	if (-1 == m_iCurCellIndex)
		FindCurIndex();
}

void CNaviAgent::lateupdate()
{
	if (nullptr != m_pTargetNaviMap && -1 != m_iCurCellIndex)
	{
		Vec3 vPos = Transform()->GetWorldPos() + m_vOffsetPos;
		vPos.y += m_vOffsetSize.y / 2;
		float fDist;
		if (!m_pTargetNaviMap->CheckIntersect(m_iCurCellIndex, vPos, Vec3(0.f, -1.f, 0.f), fDist))
		{
			if (!CheckLinkedCell())
			{
				Transform()->SetRelativePos(m_vPrevPos);
			}
		}
		else
			m_vPrevPos = SetYPos(fDist);
	}
}

void CNaviAgent::UpdateData()
{
	CComponent::UpdateData();
}


void CNaviAgent::finalupdate()
{
}

void CNaviAgent::finalupdate_debug()
{
	if (nullptr != m_pDebugObj)
	{
		m_pDebugObj->Transform()->SetRelativePos(Transform()->GetWorldPos() + m_vOffsetPos);
		m_pDebugObj->Transform()->SetRelativeScale(m_vOffsetSize);
		m_pDebugObj->Transform()->SetRelativeRotation(DecomposeRotMat(Transform()->GetWorldRotation()));
		if (nullptr != m_pDebugObj->MeshRender()->GetDynamicMaterial(0))
			m_pDebugObj->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vDebugAgentColor);

		m_pDebugObj->finalupdate();
	}
}

void CNaviAgent::render_debug()
{
	if (nullptr != m_pDebugObj)
	{
		m_pDebugObj->MeshRender()->render();
	}
}

void CNaviAgent::Serialize(YAML::Emitter& emitter)
{
	CComponent::Serialize(emitter);
}

void CNaviAgent::Deserialize(const YAML::Node& node)
{
	CComponent::Deserialize(node);
}
