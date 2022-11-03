#include "pch.h"
#include "CPlayerParryState.h"

CPlayerParryState::CPlayerParryState()
	: CState(L"PARRY")
{
}

CPlayerParryState::CPlayerParryState(const CPlayerParryState& _origin)
	: CState(_origin)

{
}

CPlayerParryState::~CPlayerParryState()
{
}


void CPlayerParryState::Enter()
{
	CState::ResetTimer();

}

void CPlayerParryState::Exit()
{

}

void CPlayerParryState::Update()
{

}

void CPlayerParryState::LateUpdate()
{

}

