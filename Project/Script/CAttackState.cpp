#include "pch.h"
#include "CAttackState.h"


CAttackState::CAttackState()
	: CState(L"ATTACK")
{
}

CAttackState::CAttackState(const CAttackState& _origin)
	:CState(_origin)

{
}

CAttackState::~CAttackState()
{
}


void CAttackState::Enter()
{
	CState::ResetTimer();

}

void CAttackState::Exit()
{
}

void CAttackState::Update()
{
	CState::Update();

}

void CAttackState::LateUpdate()
{
	CState::LateUpdate();

}
