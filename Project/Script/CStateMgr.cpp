#include "pch.h"
#include "CStateMgr.h"

// [ EMGINE PART ] 
#include <Engine/CFSM.h>


// [ SCRIPT PART ]
#include "CAttackState.h"
#include "CIdleState.h"
#include "CDieState.h"
#include "CHitState.h"

#include "CScriptMgr.h"
#include "PlayerScript.h"
#include "CPlayerMoveState.h"



CStateMgr::CStateMgr()
	: m_pOwnerObj(nullptr)
{
}

CStateMgr::~CStateMgr()
{
}

void CStateMgr::Update()
{
	m_sPrevState = L"";
	m_sPrevState = m_sCurstate;
	m_sCurstate = ChangeStateByKeyInfo();
		
	if (m_sPrevState == m_sCurstate)
		return;

	// 키 입력에 따른 ChangeState 
	ChangeState(m_sCurstate);

}

void CStateMgr::LateUpdate()
{

}

void CStateMgr::InitState(CGameObject* _pOwner)
{
	m_pOwnerObj = _pOwner;
	CFSM* pFSM = m_pOwnerObj->FSM();
	if (pFSM == nullptr)
		return;


	pFSM->AddState(L"ATTACK", new CAttackState);
	pFSM->AddState(L"DIE"	, new CDieState);
	pFSM->AddState(L"HIT"	, new CHitState);
	pFSM->AddState(L"IDLE"	, new CIdleState);
	pFSM->AddState(L"MOVE"	, new CPlayerMoveState);


	pFSM->ChangeState(L"IDLE");

}



void CStateMgr::SetKeyInfo(KEY_STATE _eKeyState, KEY _eKey, KeyOptionFlags _eFlags)
{
	m_tCurKeyInfo.eKeyState		= _eKeyState;
	m_tCurKeyInfo .eKey			= _eKey;
	m_tCurKeyInfo .iKeyFlags	= _eFlags;

}

wstring CStateMgr::ChangeStateByKeyInfo()
{
	wstring StateByKey = L"";

	// 방향키 설정 
	if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::FORWARD ||
		m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::BACKWARD ||
		m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::LEFT ||
		m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::RIGHT)
	{

		if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::TAP)
		{
			//StateByKey = L"MOVE";

		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PRESSED)
		{
			StateByKey = L"MOVE";

		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::AWAY)
		{
			StateByKey = L"IDLE";

		}
	
	}

	if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::LIGHT_ATTACK ||
		m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::HEAVY_ATTACK)
	{

		if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::TAP)
		{
			StateByKey = L"ATTACK";
		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PRESSED)
		{
			StateByKey = L"ATTACK";

		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::AWAY)
		{

		}

	}

	if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::JUMP ||
		m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::SPRINT)
	{

		if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::TAP)
		{

		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PRESSED)
		{

		}
		else if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::AWAY)
		{

		}

	}

	return StateByKey;
}

void CStateMgr::ChangeState(wstring _sStateName)
{
	if (m_pOwnerObj == nullptr)
		return;

	CFSM* pFSM = m_pOwnerObj->FSM();
	if (pFSM == nullptr)
		return;

	pFSM->ChangeState(_sStateName);
}

