#include "pch.h"
#include "Hand_StateMgr.h"

// [ ENGINE PART ] 
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CMeshData.h>

// [ SCRIPT PART ]
#include "BossJugHandScript.h"

#include "JugHand_Attack.h"
#include "JugHand_Gen.h"
#include "JugHand_Idle.h"
#include "JugHand_None.h"
#include "JugHand_Vanish.h"
#include <random>

Hand_StateMgr::Hand_StateMgr()
	: CScript{ (int)SCRIPT_TYPE::HAND_STATEMGR }
	, m_sNextState(L"")
	, m_pHand1(nullptr)
	, m_pHand2(nullptr)
	, m_pHand3(nullptr)
	, m_iCurAttackHandNumber(-1)
	, m_iSaveMidFrame(-1)
	, m_bGenDone(false)
	, m_bAllAttackDone(false)
	, m_bVanishDone(false)
{
}

Hand_StateMgr::~Hand_StateMgr()
{
}

void Hand_StateMgr::Update()
{
	if (-1 == m_iCurAttackHandNumber)
	{
		// 공격이 끝난다면 1,2,3 중 랜덤값을
		// 공격 num으로 설정해준다.
		// m_bAttackStarted 는 Gen 에서 trigger on

		std::random_device rdX;
		std::mt19937 genX(rdX());
		std::uniform_int_distribution<int> disX(1, 3);
		m_iCurAttackHandNumber = disX(genX);
	}
	
	m_sPrevState = L"";
	m_sPrevState = m_sCurstate;
	if (L"" != m_sNextState)
	{
		m_sCurstate = m_sNextState;
		m_sNextState = L"";
		ChangeState(m_sCurstate);
	}
	else
	{
		if (m_sPrevState == m_sCurstate)
			return;
	}
}

void Hand_StateMgr::LateUpdate()
{
}

void Hand_StateMgr::init()
{
	wstring pMeshName = L"";

	// Create Hand Num 01
	pMeshName = L"meshdata//hand010.mdat";
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(	pMeshName.c_str(),
																	pMeshName.c_str());
	m_pHand1 = pMeshData->Instantiate();
	m_pHand1->SetName(L"Hand01");
	m_pHand1->AddComponent(new CTransform);
	m_pHand1->AddComponent(new BossJugHandScript);
	CSceneMgr::GetInst()->SpawnObject(m_pHand1, 5);


	// Create Hand Num 02
	pMeshName = L"meshdata//hand020.mdat";
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(pMeshName.c_str(),
													pMeshName.c_str());
	m_pHand2 = pMeshData->Instantiate();
	m_pHand2->SetName(L"Hand02");
	m_pHand2->AddComponent(new CTransform);
	m_pHand2->AddComponent(new BossJugHandScript);
	CSceneMgr::GetInst()->SpawnObject(m_pHand2, 5);


	// Create Hand Num 03
	pMeshName = L"meshdata//hand030.mdat";
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(pMeshName.c_str(),
													pMeshName.c_str());
	m_pHand3 = pMeshData->Instantiate();
	m_pHand3->SetName(L"Hand03");
	m_pHand3->AddComponent(new CTransform);
	m_pHand3->AddComponent(new BossJugHandScript);
	CSceneMgr::GetInst()->SpawnObject(m_pHand3, 5);

	InitState();
}

void Hand_StateMgr::InitState()
{
	//m_pOwnerObj = _pOwner;
	CFSM* pFSM01 = m_pHand1->FSM();
	CFSM* pFSM02 = m_pHand1->FSM();
	CFSM* pFSM03 = m_pHand1->FSM();

	if (pFSM01 == nullptr || pFSM02 == nullptr || pFSM03 == nullptr)
		return;

	pFSM01->AddState(L"ATTACK", new JugHand_Attack);
	pFSM01->AddState(L"IDLE", new JugHand_Idle);
	pFSM01->AddState(L"GEN", new JugHand_Gen);
	pFSM01->AddState(L"VANISH", new JugHand_Vanish);
	pFSM01->ChangeState(L"IDLE");


	pFSM02->AddState(L"ATTACK", new JugHand_Attack);
	pFSM02->AddState(L"IDLE", new JugHand_Idle);
	pFSM02->AddState(L"GEN", new JugHand_Gen);
	pFSM02->AddState(L"VANISH", new JugHand_Vanish);
	pFSM02->ChangeState(L"IDLE");


	pFSM03->AddState(L"ATTACK", new JugHand_Attack);
	pFSM03->AddState(L"IDLE", new JugHand_Idle);
	pFSM03->AddState(L"GEN", new JugHand_Gen);
	pFSM03->AddState(L"VANISH", new JugHand_Vanish);
	pFSM03->ChangeState(L"IDLE");


}

void Hand_StateMgr::ChangeState(wstring _sStateName)
{

	CFSM* pFSM01 = m_pHand1->FSM();
	CFSM* pFSM02 = m_pHand2->FSM();
	CFSM* pFSM03 = m_pHand3->FSM();

	if (pFSM01 == nullptr || pFSM02 == nullptr || pFSM03 == nullptr)
		return;

	if (-1 == m_iCurAttackHandNumber)
		return;
	else
	{
		switch (m_iCurAttackHandNumber)
		{
		case 1:
			return pFSM01->ChangeState(_sStateName);;
		case 2:
			return pFSM02->ChangeState(_sStateName);;
		case 3:
			return pFSM03->ChangeState(_sStateName);;
		}
	}
}
