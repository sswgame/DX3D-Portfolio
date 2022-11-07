#include "pch.h"
#include "JugPhase_Dead.h"

JugPhase_Dead::JugPhase_Dead()
	:CState(L"JUG_PHASE_DEAD")
{
}

JugPhase_Dead::JugPhase_Dead(const JugPhase_Dead& _origin)
	: CState(_origin.m_sStateType)
{
}

JugPhase_Dead::~JugPhase_Dead()
{
}


void JugPhase_Dead::Enter()
{
}

void JugPhase_Dead::Exit()
{
}

void JugPhase_Dead::Update()
{
	CState::Update();
}

void JugPhase_Dead::LateUpdate()
{
	CState::LateUpdate();
}