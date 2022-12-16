#include "pch.h"
#include "ObjMgrState_IDLE.h"

#include <Engine/CFSM.h>
#include <Engine/CGameObject.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

#include "CObjectManager.h"

ObjMgrState_IDLE::ObjMgrState_IDLE()
	: CState{L"IDLE"}
	, m_bGameStart{false}
	, m_bGameEnd{false} {}

ObjMgrState_IDLE::~ObjMgrState_IDLE() = default;

void ObjMgrState_IDLE::Enter()
{
	const MAP_TYPE type       = CObjectManager::GetInst()->GetCurrentMapType();
	const Vec3     startPoint = CObjectManager::GetInst()->GetStartingPoint(type);

	CGameObject* pPlayer = CObjectManager::GetInst()->GetPlayer();
	if (pPlayer)
	{
		pPlayer->Transform()->SetRelativePos(startPoint);
	}
}

void ObjMgrState_IDLE::LateUpdate()
{
	CheckAllMonsterDead();
}

void ObjMgrState_IDLE::CheckAllMonsterDead()
{
	if (MAP_TYPE::_01 != CObjectManager::GetInst()->GetCurrentMapType())
	{
		return;
	}
	CLayer*     pMonsterLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER");
	const auto& vecMonster    = pMonsterLayer->GetRootObjects();

	const size_t count = std::count_if(vecMonster.begin(),
	                                   vecMonster.end(),
	                                   [](CGameObject* pMonster)
	                                   {
		                                   return pMonster->GetName() != L"JUG" && pMonster->IsDead();
	                                   });

	//보스만 남았거나, 존재는 하지만 모두 죽은 경우
	if (vecMonster.size() == 1 || vecMonster.size() - 1 == count)
	{
		GetOwner()->FSM()->ChangeState(L"LOADING");
	}
}

void ObjMgrState_IDLE::Exit() {}
