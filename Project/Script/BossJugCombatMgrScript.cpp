#include "pch.h"
#include "BossJugCombatMgrScript.h"

// [ EMGINE PART ]
#include <Engine/CFSM.h>
#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>

// [ SCRIPT PART ]

BossJugCombatMgrScript::BossJugCombatMgrScript()
	: CScript((int)SCRIPT_TYPE::BOSSJUGCOMBATMGRSCRIPT)
	, m_pPhaseState(nullptr)
	, m_pJugScript(nullptr)
	, m_pJugHandScript(nullptr)
{
}

BossJugCombatMgrScript::~BossJugCombatMgrScript()
{
}

void BossJugCombatMgrScript::start()
{
	// Phase 상태 초기화
	InitState();
}

void BossJugCombatMgrScript::update()
{
	CScript::update();
}

void BossJugCombatMgrScript::lateupdate()
{
	CScript::lateupdate();
}


void BossJugCombatMgrScript::InitState()
{
	m_pPhaseState = GetOwner()->FSM();
	
	if(nullptr == m_pPhaseState)
	{
		m_pPhaseState = new CFSM;
		GetOwner()->AddComponent(m_pPhaseState);
	}

	//m_pPhaseState->AddState(L"BOSS_JUG_PHASE_NONE",);
}
