#include "pch.h"
#include "JugPhase_Intro.h"

// [ ENGINE PART ]
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>

// [ SCRIPT PART ]
#include "BossJugCombatMgrScript.h"
#include "BossJugScript.h"

JugPhase_Intro::JugPhase_Intro()
	: CState(L"JUG_PHASE_INTRO")
{
}

JugPhase_Intro::JugPhase_Intro(const JugPhase_Intro& _origin)
	: CState(_origin.m_sStateType)
{
}

JugPhase_Intro::~JugPhase_Intro()
{
}

void JugPhase_Intro::Init()
{
	m_pCombatMgr = GetOwner()->GetScript<BossJugCombatMgrScript>();
	assert(m_pCombatMgr);
}

void JugPhase_Intro::Enter()
{
	// Jug 상태 초기화
	CGameObject* pBoss = m_pCombatMgr->GetJug();
	pBoss->GetScript< BossJugScript>()->Init();

	// Jug 상태를 Intro로 바꾼다.
	CFSM* pFSM = pBoss->GetScript<BossJugScript>()->GetBossFSM();
	pFSM->ChangeState(L"JUG_INTRO");
}

void JugPhase_Intro::Exit()
{
}

void JugPhase_Intro::Update()
{
	CState::Update();
}

void JugPhase_Intro::LateUpdate()
{
	CState::LateUpdate();
}

