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
#include "HandStateMgrScript.h"

#include "JugPhase_None.h"
#include "JugPhase_Intro.h"
#include "JugPhase_1.h"
#include "JugPhase_2.h"
#include "JugPhase_Dead.h"

BossJugCombatMgrScript::BossJugCombatMgrScript()
	: CScript((int)SCRIPT_TYPE::BOSSJUGCOMBATMGRSCRIPT)
	, m_pPhaseFSM(nullptr)
	, m_pStage(nullptr)
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
	if (nullptr == m_pStage)
	{
		Ptr<CPrefab> pStagePref = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BOSS_STAGE.pref",
		                                                            L"prefab\\BOSS_STAGE.pref");
		m_pStage = pStagePref->Instantiate();
		CSceneMgr::GetInst()->SpawnObject(m_pStage, L"BG");
	}

	/* 보스 생성 */
	if (nullptr == m_pJug)
	{
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\jugulus+hammer_final0.mdat",
		                                                               L"meshdata\\jugulus+hammer_final0.mdat");
		m_pJug = pMeshData->Instantiate();
		m_pJug->SetName(L"JUG");
		m_pJug->AddComponent(new BossJugScript);

		m_pJug->Transform()->SetRelativePos(-150.f, -130.f, 250.f);
		m_pJug->Transform()->SetRelativeScale(1.5f, 1.5f, 1.5f);

		CSceneMgr::GetInst()->SpawnObject(m_pJug, L"MONSTER");

		/* 무기 생성*/
		if (nullptr == m_pHammer)
		{
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\jugulus+hammer_final1.mdat",
			                                                L"meshdata\\jugulus+hammer_final1.mdat");

			m_pHammer = pMeshData->Instantiate();
			m_pHammer->SetName(L"JUG_Hammer");
			m_pHammer->Transform()->SetRelativePos(-320.f, 0.f, -40.f);
			Vec3 Rot(-90.f, 0.f, 0.f);
			Rot.ToRadian();
			m_pHammer->Transform()->SetRelativeRotation(Rot);
			//CSceneMgr::GetInst()->SpawnObject(m_pHammer, L"MONSTER");
			m_pJug->AddChild(m_pHammer);
		}


		// 보스 초기화
		m_pJug->GetScript<BossJugScript>()->Init();
	}

	/* 보스 손 생성 */
	if (nullptr == m_pJugHandMgr)
	{
		m_pJugHandMgr = new CGameObject;
		m_pJugHandMgr->SetName(L"HANDS");
		m_pJugHandMgr->AddComponent(new CTransform);
		m_pJugHandMgr->AddComponent(new HandStateMgrScript);

		CSceneMgr::GetInst()->SpawnObject(m_pJugHandMgr, L"MONSTER");
		//m_pJug->AddChild(m_pJugHandMgr);

		//m_pJugHandMgr->GetScript<HandStateMgrScript>()->init();
	}
}

void BossJugCombatMgrScript::InitState()
{
	m_pPhaseFSM = GetOwner()->FSM();

	if (nullptr == m_pPhaseFSM)
	{
		m_pPhaseFSM = new CFSM;
		GetOwner()->AddComponent(m_pPhaseFSM);
	}

	m_pPhaseFSM->AddState(GAME::BOSS::PHASE::NONE, new JugPhase_None);
	m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_INTRO, new JugPhase_Intro);
	m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_1, new JugPhase_1);
	m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_2, new JugPhase_2);
	m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_DEAD, new JugPhase_Dead);

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
	{
		m_pPhaseFSM->ChangeState(L"JUG_PHASE_INTRO");
		return;
	}
	// [ Phase 1 ]
	if (L"JUG_PHASE_INTRO" == m_pPhaseFSM->GetCurState()->GetStateType()
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

	if (nullptr == m_pPlayer)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER");
		m_pPlayer      = pLayer->FindRootObject(L"player");
	}
	else
	{
		if (m_pPlayer->IsDead())
			m_pPlayer = nullptr;
	}

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
