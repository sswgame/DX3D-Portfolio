#include "pch.h"
#include "CNaviAgent.h"

#define AGENT_TEST
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMeshRender.h"

#include "CNaviMap.h"
#include "CNaviMapData.h"
#include "CTransform.h"


CNaviAgent::CNaviAgent()
	: CComponent{COMPONENT_TYPE::NAVIAGENT}
	, m_pTargetNaviMap(nullptr)
	, m_iCurCellIndex(-1)
	, m_bGravity(false)
{
}

CNaviAgent::CNaviAgent(const CNaviAgent& _origin)
	: CComponent{COMPONENT_TYPE::NAVIAGENT}
	, m_pTargetNaviMap(_origin.m_pTargetNaviMap)
	, m_iCurCellIndex(-1)
	, m_bGravity(false)
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
	Vec3          vPos  = Transform()->GetWorldPos();

	vector<int> vecLinkedCell = pData->GetCell(m_iCurCellIndex)->GetLinkedCellIDX();
	for (size_t i = 0; i < vecLinkedCell.size(); ++i)
	{
		float tmp;
		if (m_pTargetNaviMap->CheckIntersect(vecLinkedCell[i], vPos, Vec3(0.f, -1.f, 0.f), tmp))
		{
			m_iCurCellIndex = vecLinkedCell[i];
			return true;
		}
	}

	return false;
}

void CNaviAgent::SetYPos()
{
}

void CNaviAgent::SetNaviMap(CNaviMap* _pNaviMap)
{
	m_pTargetNaviMap = _pNaviMap;
	m_iCurCellIndex  = -1;
}

void CNaviAgent::update()
{
	assert(m_pTargetNaviMap && "지정된 NaviMap이 없습니다");

	if (-1 == m_iCurCellIndex)
		FindCurIndex();

#ifdef AGENT_TEST
	if (nullptr == MeshRender())
	{
		GetOwner()->AddComponent(new CMeshRender);

		MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
		MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3D_DeferredMtrl.mtrl"),
		                                0);
		const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\Sky01.png");
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	}

	Vec3  vPos   = Transform()->GetRelativePos();
	float fSpeed = 90.f;

	if (KEY_PRESSED(KEY::UP))
	{
		Vec3 vFront = Transform()->GetWorldFrontDir();
		vPos += DT * vFront * fSpeed;
		int a = 5;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		Vec3 vFront = Transform()->GetWorldFrontDir();
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();
		vPos += DT * vRight * fSpeed;
	}

	Transform()->SetRelativePos(vPos);
#endif
}

void CNaviAgent::lateupdate()
{
	if (-1 != m_iCurCellIndex)
	{
		Vec3  vPos = Transform()->GetWorldPos();
		vPos.y += Transform()->GetWorldScale().y / 2;
		float fDist;
		if (!m_pTargetNaviMap->CheckIntersect(m_iCurCellIndex, vPos, Vec3(0.f, -1.f, 0.f), fDist))
		{
			if (!CheckLinkedCell())
			{
				Transform()->SetRelativePos(m_vPrevPos);
			}
		}
		else
		{
			if (Transform()->GetWorldScale().y < fDist)
				Transform()->SetMovePosition(0.f, -fDist, 0.f);

			m_vPrevPos = vPos;
		}
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
	CComponent::finalupdate_debug();
}

void CNaviAgent::Serialize(YAML::Emitter& emitter)
{
	CComponent::Serialize(emitter);
}

void CNaviAgent::Deserialize(const YAML::Node& node)
{
	CComponent::Deserialize(node);
}
