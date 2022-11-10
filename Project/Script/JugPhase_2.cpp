#include "pch.h"
#include "JugPhase_2.h"

// [ ENGINE PART]
#include <Engine/CFSM.h>
#include <Engine/CState.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>

// [ SCRIPT PART ]
#include "BossJugCombatMgrScript.h"
#include "BossJugScript.h"


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

void JugPhase_2::Init()
{
	m_pCombatMgr = GetOwner()->GetScript<BossJugCombatMgrScript>();
	assert(m_pCombatMgr);
}

void JugPhase_2::Enter()
{
	m_pBossFSM = m_pCombatMgr->GetJug()->FSM();
	assert(m_pBossFSM);

	m_pBossAnimator = m_pCombatMgr->GetJug()->Animator3D();
	assert(m_pBossAnimator);

	// 자식과 함께 플레이
	m_pBossAnimator->SetPlayWithChild(true);

	// 현재 보스 상태를 JUG_SPAWNHAMMER로 바꾼다.
	m_pBossFSM->ChangeState(L"JUG_SPAWNHAMMER");
}

void JugPhase_2::Update()
{
	CState* pCurState = m_pBossFSM->GetCurState();

	// 만약 현재 상태가 SPAWNHAMMER이고 애니메이션이 종료된 상태라면 다시 IDLE 상태로 돌아온다
	if (L"JUG_SPAWNHAMMER" == pCurState->GetStateType()
	    && ANIMATION_STATE::FINISH == m_pBossAnimator->GetCurAnim()->GetState())
	{
		m_pBossFSM->ChangeState(L"JUG_HAMMER_IDLE");
	}


	switch (m_iAttackPattern)
	{
	case 0:
		Attack_0();
		break;
	case 1:
		Attack_1();
		break;
	case 2:
		Attack_2();
		break;
	case 3:
		Attack_3();
		break;
	}
}

void JugPhase_2::Exit()
{
}

void JugPhase_2::LateUpdate()
{
	CState::LateUpdate();
}


void JugPhase_2::Attack_0()
{
}

void JugPhase_2::Attack_1()
{
}

void JugPhase_2::Attack_3()
{
}

void JugPhase_2::Attack_2()
{
}
