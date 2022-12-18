#include "pch.h"
#include "BossJugCombatMgrScript.h"

// [ EMGINE PART ]
#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>
#include <Engine/CFSM.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CNaviMapData.h>
#include <Engine/CNaviMap.h>

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>

#include <Engine/CSceneMgr.h>

// [ SCRIPT PART ]
#include <Engine/CCollider3D.h>

#include "CObjectManager.h"
#include "BossJugScript.h"
#include "HandStateMgrScript.h"

#include "JugPhase_None.h"
#include "JugPhase_Intro.h"
#include "JugPhase_1.h"
#include "JugPhase_2.h"
#include "JugPhase_Dead.h"
#include "TotemScript.h"

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


		CGameObject* pNMesh = new CGameObject;
		pNMesh->SetName(L"NaviMesh");
		pNMesh->AddComponent(new CTransform);
		pNMesh->AddComponent(new CNaviMap);

		CNaviMapData* pNaviMap = CResMgr::GetInst()->Load<CNaviMapData>(L"navimap\\boss_stage.map",
			L"navimap\\boss_stage.map").
			Get();
		pNMesh->NaviMap()->SetNaviMapData(pNaviMap);
		pNMesh->Transform()->SetRelativeScale(2.5f, 2.5f, 2.5f);
		pNMesh->Transform()->SetRelativePos(0.f, -345.f, 0.f);

		m_pStage->AddChild(pNMesh);
		//CObjectManager::GetInst()->SetSceneObject(m_pStage, MAP_TYPE::_02);
		//m_pStage->Deactivate();

		// totem 1
		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Jug_Totem.pref",
			L"prefab\\Jug_Totem.pref").Get();
		CGameObject* pTotem1 = pPrefab->Instantiate();
		pTotem1->SetName(L"Jug_Totem_01");
		pTotem1->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1087.f));
		pTotem1->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		pTotem1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pTotem1->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 150.f));
		pTotem1->Collider3D()->SetOffsetPos(Vec3(0.f, 85.f, 0.f));
		CSceneMgr::GetInst()->SpawnObject(pTotem1, GAME::LAYER::ITEM);


		// totem 2
		Vec3 rotateValue2 = Vec3(0.f, -70.f, 0.f);
		rotateValue2.ToRadian();
		CGameObject* pTotem2 = pPrefab->Instantiate();
		pTotem2->SetName(L"Jug_Totem_02");
		pTotem2->Transform()->SetRelativePos(Vec3(1083.f, 0.f, -376.f));
		pTotem2->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		pTotem2->Transform()->SetRelativeRotation(rotateValue2);
		pTotem2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pTotem2->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 150.f));
		pTotem2->Collider3D()->SetOffsetPos(Vec3(0.f, 85.f, 0.f));
		CSceneMgr::GetInst()->SpawnObject(pTotem2, GAME::LAYER::ITEM);

		Vec3 rotateValue3 = Vec3(0.f, 70.f, 0.f);
		rotateValue3.ToRadian();

		// totem 3
		CGameObject* pTotem3 = pPrefab->Instantiate();
		pTotem3->SetName(L"Jug_Totem_03");
		pTotem3->Transform()->SetRelativePos(Vec3(-1083.f, 0.f, -390.f));
		pTotem3->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		pTotem3->Transform()->SetRelativeRotation(rotateValue3);
		pTotem3->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pTotem3->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 150.f));
		pTotem3->Collider3D()->SetOffsetPos(Vec3(Vec3(0.f, 85.f, 0.f)));
		CSceneMgr::GetInst()->SpawnObject(pTotem3, GAME::LAYER::ITEM);
	}

	/* 보스 생성 */
	if (nullptr == m_pJug)
	{
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\jugulus+hammer_final0.mdat",
			L"meshdata\\jugulus+hammer_final0.mdat");
		m_pJug = pMeshData->Instantiate();
		m_pJug->SetName(L"JUG");
		m_pJug->AddComponent(new BossJugScript);

		m_pJug->Transform()->SetRelativePos(-150.f, 0.f, 400.f);
		m_pJug->Transform()->SetRelativeScale(1.3f, 1.3f, 1.3f);

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


			m_pJug->AddChild(m_pHammer);
			//m_pHammer->Deactivate();
		}


		// 보스 초기화
		m_pJug->GetScript<BossJugScript>()->Init();

		//CObjectManager::GetInst()->SetSceneObject(m_pJug, MAP_TYPE::_02);
		//m_pJug->Deactivate();
	}

	/* 보스 손 생성 */
	if (nullptr == m_pJugHandMgr)
	{
		m_pJugHandMgr = new CGameObject;
		m_pJugHandMgr->SetName(L"HAND MGR");
		m_pJugHandMgr->AddComponent(new CTransform);
		m_pJugHandMgr->AddComponent(new HandStateMgrScript);

		CSceneMgr::GetInst()->SpawnObject(m_pJugHandMgr, GAME::LAYER::OBJECT_MGR);
		m_pJugHandMgr->GetScript<HandStateMgrScript>()->init();

		//CObjectManager::GetInst()->SetSceneObject(m_pJugHandMgr, MAP_TYPE::_02);
		//m_pJugHandMgr->Deactivate();
	}

	/* Pot 생성 */
	if (nullptr == m_pPot)
	{
		Ptr<CPrefab> pStagePref = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Pot.pref",
			L"prefab\\Pot.pref");

		m_pPot = pStagePref->Instantiate();
		m_pPot->Animator3D()->SetPlayWithChild(true);
		m_pPot->Animator3D()->MakeAnimationFromTXT("PotAnimInfo.txt");
		CSceneMgr::GetInst()->SpawnObject(m_pPot, GAME::LAYER::ITEM);
	}
}

void BossJugCombatMgrScript::InitState()
{
	m_pPhaseFSM = GetOwner()->FSM();

	if (nullptr == m_pPhaseFSM)
	{
		m_pPhaseFSM = new CFSM;
		GetOwner()->AddComponent(m_pPhaseFSM);

		m_pPhaseFSM->AddState(GAME::BOSS::PHASE::NONE, new JugPhase_None);
		m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_INTRO, new JugPhase_Intro);
		m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_1, new JugPhase_1);
		m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_2, new JugPhase_2);
		m_pPhaseFSM->AddState(GAME::BOSS::PHASE::JUG_PHASE_DEAD, new JugPhase_Dead);

		for (auto pState : m_pPhaseFSM->GetAllStates())
		{
			pState.second->Init();
		}

		m_pPhaseFSM->SetCurState(GAME::BOSS::PHASE::NONE);
	}
}

void BossJugCombatMgrScript::CheckPhase() const
{
	// [ Phase Intro]
	if (L"JUG_PHASE_NONE" == m_pPhaseFSM->GetCurState()->GetStateType() && m_bStartCombat)
	{
		m_pPhaseFSM->ChangeState(GAME::BOSS::PHASE::JUG_PHASE_INTRO);
		return;
	}
	// [ Phase 1 ]
	if (L"JUG_PHASE_INTRO" == m_pPhaseFSM->GetCurState()->GetStateType()
		&& 4.f < m_pPhaseFSM->GetCurState()->GetTimer())
	{
		m_pPhaseFSM->ChangeState(GAME::BOSS::PHASE::JUG_PHASE_1);
		return;
	}

	// [ Phase 2 ]
	if (L"JUG_PHASE_1" == m_pPhaseFSM->GetCurState()->GetStateType()
		&& m_pJug->GetScript<BossJugScript>()->GetHP() < (m_pJug->GetScript<BossJugScript>()->GetMaxHP() / 2))
	{
		m_pPhaseFSM->ChangeState(GAME::BOSS::PHASE::JUG_PHASE_2);
		return;
	}
	// [ Dead ]
	if (0 >= m_pJug->GetScript<BossJugScript>()->GetHP())
	{
		m_pPhaseFSM->ChangeState(GAME::BOSS::PHASE::JUG_PHASE_DEAD);
		return;
	}
}

void BossJugCombatMgrScript::start()
{
	SpawnStage();
	InitState();

	// Object Mgr에 등록
	CObjectManager::GetInst()->SetBossCombatMgr(GetOwner());

	//최초 시작 이후 맵 전환 전까지 동작하지 않아야하므로
	//GetOwner()->Deactivate();
}

void BossJugCombatMgrScript::update()
{
	// 배틀 시작
	if (!m_bStartCombat)
		m_bStartCombat = true;

	if (nullptr == m_pPlayer)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER");
		m_pPlayer = pLayer->FindRootObject(L"player");
	}
	else
	{
		if (m_pPlayer->IsDead())
			m_pPlayer = nullptr;
	}

	// 현재 타입 이름
	m_strCurState = ToString(m_pPhaseFSM->GetCurState()->GetStateType());

	if (GAME::BOSS::PHASE::JUG_PHASE_DEAD != ToWString(m_strCurState))
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
