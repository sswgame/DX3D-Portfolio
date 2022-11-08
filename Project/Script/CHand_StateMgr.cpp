#include "pch.h"
#include "CHand_StateMgr.h"

// [ ENGINE PART ] 
#include <Engine/CFSM.h>

// [ SCRIPT PART ]
#include "JugHand_Attack.h"
#include "JugHand_Gen.h"
#include "JugHand_Idle.h"
#include "JugHand_None.h"
#include "JugHand_Vanish.h"

CHand_StateMgr::CHand_StateMgr()
	: m_pOwnerObj(nullptr)
	, m_sNextState(L"")
{
}

CHand_StateMgr::~CHand_StateMgr()
{
}

void CHand_StateMgr::Update()
{
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

void CHand_StateMgr::LateUpdate()
{
}

void CHand_StateMgr::InitState(CGameObject* _pOwner)
{
	m_pOwnerObj = _pOwner;
	CFSM* pFSM = m_pOwnerObj->FSM();
	if (pFSM == nullptr)
		return;

	pFSM->AddState(L"ATTACK", new JugHand_Attack);
	//pFSM->AddState(L"DEATH", new CHand_Death);
	pFSM->AddState(L"IDLE", new JugHand_Idle);
	pFSM->AddState(L"GEN", new JugHand_Gen);
	//pFSM->AddState(L"NONE", new JugHand_None);
	pFSM->AddState(L"VANISH", new JugHand_Vanish);

	pFSM->ChangeState(L"IDLE");
}

void CHand_StateMgr::ChangeState(wstring _sStateName)
{
	if (m_pOwnerObj == nullptr)
		return;

	CFSM* pFSM = m_pOwnerObj->FSM();
	if (pFSM == nullptr)
		return;

	pFSM->ChangeState(_sStateName);
}
