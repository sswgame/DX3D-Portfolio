#include "pch.h"
#include "ObjMgrState_IDLE.h"

#include <Engine/CCollider3D.h>
#include <Engine/CFSM.h>
#include <Engine/CGameObject.h>

#include "CObjectManager.h"
#include "DoorScript.h"

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
	return;
	CGameObject* pDoor = CObjectManager::GetInst()->GetGate()->GetChild(L"DOOR");
	if (CObjectManager::GetInst()->CheckAllMonsterDead() && false == pDoor->GetScript<DoorScript>()->IsDoorOpened())
	{
		pDoor->GetScript<DoorScript>()->SetOpen(true);
		pDoor->Collider3D()->Activate();
	}
}


void ObjMgrState_IDLE::Exit() {}
