#include "pch.h"
#include "BossJugScript.h"

// [ ENGINE PART ]
#include <Engine/CState.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>
#include <Engine/CFSM.h>

// [ SCRIPT PART ] 
#include "Jug_None.h"
#include "Jug_Intro.h"
#include "Jug_Norm_Idle.h"
#include "Jug_Norm_Hit.h"
#include "Jug_Hammer_Idle.h"
#include "Jug_Hammer_Hit.h"
#include "Jug_Attack_0.h"
#include "Jug_Attack_1.h"
#include "Jug_SpawnHammer.h"
#include "Jug_WalkGround.h"
#include "Jug_Fly.h"
#include "Jug_Dead.h"

BossJugScript::BossJugScript()
	: CScript((int)SCRIPT_TYPE::BOSSJUGSCRIPT)
{
	AddScriptParam("HP", SCRIPTPARAM_TYPE::FLOAT, &m_fHP);
	AddScriptParam("MAX HP", SCRIPTPARAM_TYPE::FLOAT, &m_fMaxHP);

}

BossJugScript::~BossJugScript()
{
}


void BossJugScript::Init()
{
	// �ִϸ��̼� �ε�
	m_pBossAnimator = GetOwner()->Animator3D();
	if (nullptr == m_pBossAnimator)
	{
		GetOwner()->AddComponent(new CAnimator3D);
	}
	map<wstring, CAnimation3D*> mapAnim = m_pBossAnimator->GetAllAnim();
	if (1 <= mapAnim.size())
	{
		m_pBossAnimator->MakeAnimationFromTXT("BossJugAnimInfo.txt");
	}

	// ���� �ʱ�ȭ
	InitState();
}

void BossJugScript::InitState()
{
	{
		m_pBossFSM = GetOwner()->FSM();

		if (nullptr == m_pBossFSM)
		{
			m_pBossFSM = new CFSM;
			GetOwner()->AddComponent(m_pBossFSM);
		}

		m_pBossFSM->AddState(L"JUG_NONE", new Jug_None);
		m_pBossFSM->AddState(L"JUG_INTRO", new Jug_Intro);
		m_pBossFSM->AddState(L"JUG_NORM_IDLE", new Jug_Norm_Idle);
		m_pBossFSM->AddState(L"JUG_NORM_HIT", new Jug_Norm_Hit);
		m_pBossFSM->AddState(L"JUG_HAMMER_IDLE", new Jug_Hammer_Idle);
		m_pBossFSM->AddState(L"JUG_HAMMER_HIT", new Jug_Hammer_Hit);
		m_pBossFSM->AddState(L"JUG_ATTACK_0", new Jug_Attack_0);
		m_pBossFSM->AddState(L"JUG_ATTACK_1", new Jug_Attack_1);
		m_pBossFSM->AddState(L"JUG_SPAWNHAMMER", new Jug_SpawnHammer);
		m_pBossFSM->AddState(L"JUG_WALKGROUND", new Jug_WalkGround);
		m_pBossFSM->AddState(L"JUG_FLY", new Jug_Fly);
		m_pBossFSM->AddState(L"JUG_DEAD", new Jug_Dead);
		
		for( auto pState : m_pBossFSM->GetAllStates())
		{
			pState.second->Init();
		}

		m_pBossFSM->SetCurState(L"JUG_NONE");
	}
}
void BossJugScript::start()
{
	CScript::start();
}

void BossJugScript::update()
{
	CScript::update();
}

void BossJugScript::lateupdate()
{
	CScript::lateupdate();
}

void BossJugScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript::OnCollisionEnter(_OtherObject);
}

void BossJugScript::OnCollision(CGameObject* _OtherObject)
{
	CScript::OnCollision(_OtherObject);
}

void BossJugScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript::OnCollisionExit(_OtherObject);
}

void BossJugScript::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_fHP) << YAML::Value << m_fHP;
	emitter << YAML::Key << NAME_OF(m_fMaxHP) << YAML::Value << m_fMaxHP;

}

void BossJugScript::Deserialize(const YAML::Node& node)
{
	m_fHP = node[NAME_OF(m_fHP)].as<float>();
	m_fMaxHP = node[NAME_OF(m_fMaxHP)].as<float>();

	CScript::Deserialize(node);
}
