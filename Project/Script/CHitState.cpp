#include "pch.h"
#include "CHitState.h"

CHitState::CHitState()
	: CState(L"HIT")
{
}

CHitState::CHitState(const CHitState& _origin)
	:CState(_origin)
{
}


CHitState::~CHitState()
{
}


void CHitState::Enter()
{
	CState::ResetTimer();

}

void CHitState::Exit()
{

}

void CHitState::Update()
{
	CState::Update();

}

void CHitState::LateUpdate()
{
	CState::LateUpdate();


}