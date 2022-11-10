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
#include "CHand_StateMgr.h"
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
	, m_pHammer(nullptr)
	, m_pJugHandMgr(nullptr)
	, m_pStageInnerCollider(nullptr)
	, m_pStageOuterCollider(nullptr)
{
	AddScriptParam("PHASE INFO", SCRIPTPARAM_TYPE::TEXT, &m_strCurState);
}

BossJugCombatMgrScript::~BossJugCombatMgrScript()
{
}

void BossJugCombatMgrScript::SpawnStage()
{
	/* 스테이지 생성 */
	Ptr<CPrefab> pStagePref = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BOSS_STAGE.pref", L"prefab\\BOSS_STAGE.pref");
	CGameObject* pStageObj  = pStagePref->Instantiate();
	CSceneMgr::GetInst()->AddChild(GetOwner(), pStageObj);

	/* 보스 생성 */
	// 1페이즈 보스
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\jugulus+hammer_final0.mdat",
	                                                               L"meshdata\\jugulus+hammer_final0.mdat");

	m_pJug = pMeshData->Instantiate();
	m_pJug->SetName(L"JUG");
	m_pJug->AddComponent(new BossJugScript);
	m_pJug->GetScript<BossJugScript>()->Init();

	CSceneMgr::GetInst()->GetCurScene()->AddObject(m_pJug, GetOwner()->GetLayerIndex());
	GetOwner()->AddChild(m_pJug);

	// 2페이즈 보스 무기
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\jugulus+hammer_final1.mdat",
	                                                L"meshdata\\jugulus+hammer_final1.mdat");

	CGameObject* pHammer = pMeshData->Instantiate();
	pHammer->SetName(L"JUG_Hammer");

	CSceneMgr::GetInst()->GetCurScene()->AddObject(pHammer, GetOwner()->GetLayerIndex());
	m_pJug->AddChild(pHammer);


	/* 보스 손 생성 */
	//m_pJugHandMgr = new CGameObject;
	//m_pJugHandMgr->AddComponent(new CTransform);
	//m_pJugHandMgr->AddComponent(new CHand_StateMgr);
	//m_pJugHandMgr->Init();
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
}

void BossJugCombatMgrScript::CheckPhase() const
{
	// [ Phase Intro]
	if (L"JUG_PHASE_NONE" == m_pPhaseFSM->GetCurState()->GetStateType() && m_bStartCombat)
		m_pPhaseFSM->ChangeState(L"JUG_PHASE_INTRO");

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
	SpawnStage();
	InitState();
}

void BossJugCombatMgrScript::update()
{
	// 배틀 시작
	if (!m_bStartCombat)
		m_bStartCombat = true;

	// 현재 타입 이름
	m_strCurState = ToString(m_pPhaseFSM->GetCurState()->GetStateType());

	/* Phase 전환 체크 */
	CheckPhase();

}

void BossJugCombatMgrScript::lateupdate()
{
}

void BossJugCombatMgrScript::Serialize(YAML::Emitter& emitter)
{
	CScript::Serialize(emitter);
}

void BossJugCombatMgrScript::Deserialize(const YAML::Node& node)
{
	CScript::Deserialize(node);
}
