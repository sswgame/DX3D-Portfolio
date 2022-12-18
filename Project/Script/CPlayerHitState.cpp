#include "pch.h"
#include "CPlayerHitState.h"
#include "TrailScript.h"

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

	TrailScript* pTrail = (TrailScript*)GetOwner()->GetScriptByName(L"TrailScript");
	if (pTrail)
	{
		pTrail->Off();
	}


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