#include "pch.h"
#include "CPlayerIdleState.h"

CPlayerIdleState::CPlayerIdleState()
	: CState(L"IDLE")
{
}

CPlayerIdleState::CPlayerIdleState(const CPlayerIdleState& _origin)
	:CState(_origin)

{
}

CPlayerIdleState::~CPlayerIdleState()
{
}


void CPlayerIdleState::Enter()
{
	CState::ResetTimer();

}

void CPlayerIdleState::Exit()
{

}

void CPlayerIdleState::Update()
{
	CState::Update();

}

void CPlayerIdleState::LateUpdate()
{
	CState::LateUpdate();


}

