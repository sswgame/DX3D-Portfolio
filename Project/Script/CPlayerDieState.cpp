#include "pch.h"
#include "CPlayerDieState.h"


CPlayerDieState::CPlayerDieState()
	: CState(L"DIE")
{
}

CPlayerDieState::CPlayerDieState(const CPlayerDieState& _origin)
	:CState(_origin)

{
}

CPlayerDieState::~CPlayerDieState()
{
}


void CPlayerDieState::Enter()
{
	CState::ResetTimer();


}

void CPlayerDieState::Exit()
{

}

void CPlayerDieState::Update()
{
	CState::Update();

}

void CPlayerDieState::LateUpdate()
{
	CState::LateUpdate();

}
