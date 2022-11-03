#include "pch.h"
#include "CStateMgr.h"

// [ EMGINE PART ] 
#include <Engine/CFSM.h>


// [ SCRIPT PART ]
#include "CPlayerDieState.h"			// DIE 
#include "CPlayerHeavyAttackState.h"	// HEAVY_ATTACK
#include "CPlayerHitState.h"			// HIT
#include "CPlayerIdleState.h"			// IDLE
#include "CPlayerJumpState.h"			// JUMP	
#include "CPlayerLightAttackState.h"	// LIGHT_ATTACK
#include "CPlayerMoveState.h"			// MOVE
#include "CPlayerSprintState.h"			// SPRINT
#include "CPlayerParryState.h"			// PARRY 


#include "CScriptMgr.h"
#include "PlayerScript.h"
#include "CPlayerMoveState.h"



CStateMgr::CStateMgr()
	: m_pOwnerObj(nullptr)
	, m_sPrevState(L"")
	, m_sCurstate(L"")

{
}

CStateMgr::~CStateMgr()
{
}

void CStateMgr::Update()
{
	m_sPrevState = m_sCurstate;
	m_sCurstate = ChangeStateByKeyInfo();
	
	// 현재 상태와 다음 상태가 동일하다면 바꾸지 않는다. 
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

	pFSM->AddState(L"DIE"			, new CPlayerDieState);
	pFSM->AddState(L"HEAVY_ATTACK"	, new CPlayerHeavyAttackState);
	pFSM->AddState(L"HIT"			, new CPlayerHitState);
	pFSM->AddState(L"IDLE"			, new CPlayerIdleState);
	pFSM->AddState(L"JUMP"			, new CPlayerJumpState);
	pFSM->AddState(L"LIGHT_ATTACK"	, new CPlayerLightAttackState);
	pFSM->AddState(L"MOVE"			, new CPlayerMoveState);
	pFSM->AddState(L"SPRINT"		, new CPlayerSprintState);
	pFSM->AddState(L"PARRY"			, new CPlayerParryState);

	pFSM->ChangeState(L"IDLE");
}



void CStateMgr::SetKeyInfo(KEY_STATE _eKeyState, KEY _eKey, tKeyOptionFlags_Zip _tFlags)
{
	m_tCurKeyInfo.eKeyState		= _eKeyState;
	m_tCurKeyInfo .eKey			= _eKey;
	m_tCurKeyInfo .tKeyFlags_Zip	= _tFlags;

}

wstring CStateMgr::ChangeStateByKeyInfo()
{
	wstring StateByKey = L"";

	// 키 변화에 다른 상태 변화 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
	{
		StateByKey = Check_Tap();
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED)
	{
		StateByKey = Check_Pressed();
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::AWAY)
	{
		StateByKey = Check_Away();
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


wstring CStateMgr::Check_Tap()
{
	wstring StateByKey = L"";

	// [ TAP ]

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::FORWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::BACKWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LEFT ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::RIGHT)
	{
		StateByKey = L"MOVE";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::JUMP)
	{
		StateByKey = L"JUMP";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::SPRINT)
	{
		StateByKey = L"SPRINT";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::PARRY)
	{
		StateByKey = L"PARRY";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LIGHT_ATTACK)
	{
		StateByKey = L"LIGHT_ATTACK";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::HEAVY_ATTACK)
	{
		StateByKey = L"HEAVY_ATTACK";
	}

	return StateByKey;

}

wstring CStateMgr::Check_Pressed()
{
	wstring StateByKey = L"";

	// [ PRESSED ]

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT)
	{
		StateByKey = L"MOVE";
	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::JUMP)
	{

	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::SPRINT)
	{


	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PARRY)
	{

	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LIGHT_ATTACK)
	{

	}


	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::HEAVY_ATTACK)
	{

	}

	return StateByKey;

}

wstring CStateMgr::Check_Away()
{
	wstring StateByKey = L"";

	// [ AWAY ]

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::FORWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::BACKWARD ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::LEFT ||
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::RIGHT)
	{
		StateByKey = L"IDLE";

	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::JUMP)
	{

	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::SPRINT)
	{


	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::PARRY)
	{

	}

	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::LIGHT_ATTACK)
	{

	}


	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::HEAVY_ATTACK)
	{

	}

	return StateByKey;

}