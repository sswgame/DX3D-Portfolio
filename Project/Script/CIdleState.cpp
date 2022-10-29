#include "pch.h"
#include "CIdleState.h"

CIdleState::CIdleState()
	: CState(L"IDLE")
{
}

CIdleState::CIdleState(const CIdleState& _origin)
	:CState(_origin)

{
}

CIdleState::~CIdleState()
{
}


void CIdleState::Enter()
{
	CState::ResetTimer();

}

void CIdleState::Exit()
{

}

void CIdleState::Update()
{
	CState::Update();

}

void CIdleState::LateUpdate()
{
	CState::LateUpdate();


}

