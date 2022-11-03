#include "pch.h"
#include "CPlayerSprintState.h"

#include <Engine/CFSM.h>


CPlayerSprintState::CPlayerSprintState()
	: CState(L"SPRINT")
{
}

CPlayerSprintState::CPlayerSprintState(const CPlayerSprintState& _origin)
	:CState(_origin)

{
}

CPlayerSprintState::~CPlayerSprintState()
{
}


void CPlayerSprintState::Enter()
{
	CState::ResetTimer();

}

void CPlayerSprintState::Exit()
{

}

void CPlayerSprintState::Update()
{
	CState::Update();
	if (CState::m_fTimer >= 5.f)
		GetOwner()->FSM()->ChangeState(L"IDLE");



}

void CPlayerSprintState::LateUpdate()
{
	CState::LateUpdate();


}
