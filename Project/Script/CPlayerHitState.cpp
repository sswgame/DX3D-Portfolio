#include "pch.h"
#include "CPlayerHitState.h"

CPlayerHitState::CPlayerHitState()
	: CState(L"HIT")
{
}

CPlayerHitState::CPlayerHitState(const CPlayerHitState& _origin)
	:CState(_origin)
{
}


CPlayerHitState::~CPlayerHitState()
{
}


void CPlayerHitState::Enter()
{
	CState::ResetTimer();

}

void CPlayerHitState::Exit()
{

}

void CPlayerHitState::Update()
{
	CState::Update();

}

void CPlayerHitState::LateUpdate()
{
	CState::LateUpdate();


}