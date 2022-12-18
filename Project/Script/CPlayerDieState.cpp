#include "pch.h"
#include "CPlayerDieState.h"
#include "TrailScript.h"



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
	TrailScript* pTrail = (TrailScript*)GetOwner()->GetScriptByName(L"TrailScript");
	if (pTrail)
	{
		pTrail->Off();
	}

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
