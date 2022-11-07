#include "pch.h"
#include "JugPhase_2.h"

JugPhase_2::JugPhase_2()
	: CState(L"JUG_PHASE_2")
{
}

JugPhase_2::JugPhase_2(const JugPhase_2& _origin)
	: CState(_origin.m_sStateType)
{
}

JugPhase_2::~JugPhase_2()
{
}


void JugPhase_2::Enter()
{
}

void JugPhase_2::Exit()
{
}

void JugPhase_2::Update()
{
	CState::Update();
}

void JugPhase_2::LateUpdate()
{
	CState::LateUpdate();
}
