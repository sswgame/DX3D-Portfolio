#include "pch.h"
#include "ObjMgrState_IDLE.h"

#include <Engine/CFSM.h>
#include <Engine/CGameObject.h>

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
	if (CObjectManager::GetInst()->CheckAllMonsterDead())
	{
		//GetOwner()->FSM()->ChangeState(L"LOADING");
	}
}


void ObjMgrState_IDLE::Exit() {}
