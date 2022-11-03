#include "pch.h"
#include "CPlayerJumpState.h"

CPlayerJumpState::CPlayerJumpState()
	: CState(L"JUMP")
{
}

CPlayerJumpState::CPlayerJumpState(const CPlayerJumpState& _origin)
	:CState(_origin)

{
}

CPlayerJumpState::~CPlayerJumpState()
{
}


void CPlayerJumpState::Enter()
{
	CState::ResetTimer();

}

void CPlayerJumpState::Exit()
{

}

void CPlayerJumpState::Update()
{
	CState::Update();

}

void CPlayerJumpState::LateUpdate()
{
	CState::LateUpdate();


}
