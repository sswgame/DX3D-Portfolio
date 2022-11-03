#include "pch.h"
#include "CPlayerLightAttackState.h"

CPlayerLightAttackState::CPlayerLightAttackState()
	: CState(L"LIGHT_ATTACK")
{
}

CPlayerLightAttackState::CPlayerLightAttackState(const CPlayerLightAttackState& _origin)
	:CState(_origin)

{
}

CPlayerLightAttackState::~CPlayerLightAttackState()
{
}


void CPlayerLightAttackState::Enter()
{
	CState::ResetTimer();

}

void CPlayerLightAttackState::Exit()
{

}

void CPlayerLightAttackState::Update()
{
	CState::Update();

}

void CPlayerLightAttackState::LateUpdate()
{
	CState::LateUpdate();


}
