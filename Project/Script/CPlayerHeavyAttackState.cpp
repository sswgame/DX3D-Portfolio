#include "pch.h"
#include "CPlayerHeavyAttackState.h"

CPlayerHeavyAttackState::CPlayerHeavyAttackState()
	: CState(L"HEAVY_ATTACK")
{
}

CPlayerHeavyAttackState::CPlayerHeavyAttackState(const CPlayerHeavyAttackState& _origin)
	:CState(_origin)

{
}

CPlayerHeavyAttackState::~CPlayerHeavyAttackState()
{
}


void CPlayerHeavyAttackState::Enter()
{
	CState::ResetTimer();

}

void CPlayerHeavyAttackState::Exit()
{

}

void CPlayerHeavyAttackState::Update()
{
	CState::Update();

}

void CPlayerHeavyAttackState::LateUpdate()
{
	CState::LateUpdate();


}
