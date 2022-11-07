#include "pch.h"
#include "BossJugCombatMgrScript.h"

// [ EMGINE PART ]
#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>
#include <Engine/CFSM.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>

#include <Engine/CSceneMgr.h>

// [ SCRIPT PART ]
#include "BossJugScript.h"
#include "BossJugHandScript.h"

#include "JugPhase_None.h"
#include "JugPhase_Intro.h"
#include "JugPhase_1.h"
#include "JugPhase_2.h"
#include "JugPhase_Dead.h"

BossJugCombatMgrScript::BossJugCombatMgrScript()
	: CScript((int)SCRIPT_TYPE::BOSSJUGCOMBATMGRSCRIPT)
	, m_pPhaseFSM(nullptr)
	, m_pJug(nullptr)
	, m_pJugHand(nullptr)
{
}

BossJugCombatMgrScript::~BossJugCombatMgrScript()
{
}

void BossJugCombatMgrScript::InitState()
{
	m_pPhaseFSM = GetOwner()->FSM();

	if (nullptr == m_pPhaseFSM)
	{
		m_pPhaseFSM = new CFSM;
		GetOwner()->AddComponent(m_pPhaseFSM);
	}

	m_pPhaseFSM->AddState(L"JUG_PHASE_NONE", new JugPhase_None);
	m_pPhaseFSM->AddState(L"JUG_PHASE_INTRO", new JugPhase_Intro);
	m_pPhaseFSM->AddState(L"JUG_PHASE_1", new JugPhase_1);
	m_pPhaseFSM->AddState(L"JUG_PHASE_2", new JugPhase_2);
	m_pPhaseFSM->AddState(L"JUG_PHASE_DEAD", new JugPhase_Dead);

	for (auto pState : m_pPhaseFSM->GetAllStates())
	{
		pState.second->Init();
	}

	m_pPhaseFSM->SetCurState(L"JUG_PHASE_NONE");
	m_pPhaseFSM->ChangeState(L"JUG_PHASE_INTRO");
}

void BossJugCombatMgrScript::SpawnStage()
{
	/* 스테이지 생성 */
	Ptr<CPrefab> pStagePref = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BossJugStage.pref", L"prefab\\BOSS_STAGE.pref");
	CGameObject* pStageObj = pStagePref->Instantiate();
	CSceneMgr::GetInst()->GetCurScene()->AddObject(pStageObj, GetOwner()->GetLayerIndex());
	GetOwner()->AddChild(pStageObj);

	/* 보스 생성 */
	// 오브젝트 생성
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\boss_jug_01.mdat", L"meshdata\\boss_jug_01.mdat");

	m_pJug = pMeshData->Instantiate();
	m_pJug->SetName(L"Jug");
	m_pJug->AddComponent(new BossJugScript);

	//CPrefab* BossPrefab = dynamic_cast<CPrefab*>(CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BossJug.pref").Get());
	//m_pJug = BossPrefab->Instantiate();
	//m_pJug->GetScript<BossJugScript>()->Init();

	CSceneMgr::GetInst()->GetCurScene()->AddObject(m_pJug, GetOwner()->GetLayerIndex());
	GetOwner()->AddChild(m_pJug);
}

void BossJugCombatMgrScript::CheckPhase() const
{
	// [ Phase 1 ]
	if(L"JUG_PHASE_INTRO" == m_pPhaseFSM->GetCurState()->GetStateType()
		&& ANIMATION_STATE::FINISH == m_pJug->Animator3D()->GetCurAnim()->GetState())
	{
		m_pPhaseFSM->ChangeState(L"JUG_PHASE_1");
		return;
	}

	// [ Phase 2 ]
	if (L"JUG_PHASE_1" == m_pPhaseFSM->GetCurState()->GetStateType()
		&& m_pJug->GetScript<BossJugScript>()->GetHP() < (m_pJug->GetScript<BossJugScript>()->GetMaxHP() / 2))
	{
		m_pPhaseFSM->ChangeState(L"JUG_PHASE_2");
		return;
	}
	// [ Dead ]
	if (L"JUG_PHASE_2" == m_pPhaseFSM->GetCurState()->GetStateType()
		&& 0 >= m_pJug->GetScript<BossJugScript>()->GetHP())
	{
		m_pPhaseFSM->ChangeState(L"JUG_PHASE_DEAD");
		return;
	}
}

void BossJugCombatMgrScript::start()
{
	InitState();
	SpawnStage();
}

void BossJugCombatMgrScript::update()
{
	CScript::update();

	/* Phase 전환 체크 */
	CheckPhase();

}

void BossJugCombatMgrScript::lateupdate()
{
	CScript::lateupdate();
}