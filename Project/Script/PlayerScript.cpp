#include "pch.h"
#include "PlayerScript.h"

//#include "CState.h" // .. 각 상태별 헤더들은 CState.h 에 정의했습니다. 

#include "CStateMgr.h"
#include "CPlayerStat.h"
#include "CObjKeyMgr.h"

PlayerScript::PlayerScript() :
	CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pKeyMgr(nullptr)
	, m_pStateMgr(nullptr)
	, m_pStat(nullptr)
{
}

PlayerScript::~PlayerScript() 
{
	SAFE_DELETE(m_pKeyMgr);
	SAFE_DELETE(m_pStateMgr);
	SAFE_DELETE(m_pStat);
}

void PlayerScript::start()
{

	// 키 정보 관리 초기화
	if (nullptr == m_pKeyMgr)
	{
		m_pKeyMgr = new CObjKeyMgr;
		m_pKeyMgr->SetOwnerScript(this);
	}


	// 상태 정보 관리 초기화
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CStateMgr;
		m_pStateMgr->InitState(GetOwner());
	}


	// 스탯 정보 관리 초기화 
	if (nullptr == m_pStat)
	{
		m_pStat = new CPlayerStat;
		m_pStat->SetOwnerScript(this);
		m_pStat->start();
	}


	// 플레이어 방향 정보 초기화 
	m_vDirection = Vec3(0.f, 0.f, 1.f);

}

void PlayerScript::update()
{

	// 1. 키 상태 업데이트 
	m_pKeyMgr->update();
	tKey_Zip tCurKeyInfo = m_pKeyMgr->GetCurKeyInfo();
	tKey_Zip tPrevKeyInfo = m_pKeyMgr->GetPrevKeyInfo();

	// 2. 플레이어 방향 정보 업데이트 
	
	
	//UpdateDirection(tCurKeyInfo);


	// 3. 플레이어 상태 업데이트 
	m_pStateMgr->SetCurKeyInfo(tCurKeyInfo);
	m_pStateMgr->SetPrevKeyInfo(tPrevKeyInfo);
	m_pStateMgr->Update();

	// 4. 플레이어 스탯 업데이트 
	m_pStat->update();

}

void PlayerScript::lateupdate() 
{

	m_pKeyMgr->lateupdate();

	m_pStateMgr->LateUpdate();

	m_pStat->lateupdate();

}


void PlayerScript::UpdateDirection(tKey_Zip _keyInfo)
{
	// 방향 체크 
	// 여기서 PLAYER_KEY_OPTION 은 키에 반응한 방향을 의미한다.
	// NONE 은 움직이지 않음을 의미
	PLAYER_KEY_OPTION eDir;
	KEY eKey = KEY::KEY_END;
	(_keyInfo.iKeyFlags & PLAYER_KEY_OPTION::FORWARD) ?  eDir = PLAYER_KEY_OPTION::FORWARD	: eDir = PLAYER_KEY_OPTION::NONE;
	(_keyInfo.iKeyFlags & PLAYER_KEY_OPTION::BACKWARD) ? eDir = PLAYER_KEY_OPTION::BACKWARD : eDir = PLAYER_KEY_OPTION::NONE;
	(_keyInfo.iKeyFlags & PLAYER_KEY_OPTION::LEFT) ?	 eDir = PLAYER_KEY_OPTION::LEFT		: eDir = PLAYER_KEY_OPTION::NONE;
	(_keyInfo.iKeyFlags & PLAYER_KEY_OPTION::RIGHT) ?	 eDir = PLAYER_KEY_OPTION::RIGHT	: eDir = PLAYER_KEY_OPTION::NONE;
	(_keyInfo.iKeyFlags & PLAYER_KEY_OPTION::JUMP) ?	 eDir = PLAYER_KEY_OPTION::JUMP		: eDir = PLAYER_KEY_OPTION::NONE;

	switch (eDir)
	{
	case FORWARD:
	{
		
	}
		break;
	case BACKWARD:
	{
	
	}
		break;
	case LEFT:
	{
		
	}
		break;
	case RIGHT:
	{
		
	}
		break;
	case JUMP:
	{

	}
		break;
	}

	// 단위 행렬로 업데이트
	m_vDirection.Normalize();

}

void PlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void PlayerScript::OnCollision(CGameObject* _OtherObject)
{
}

void PlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{
}


void PlayerScript::SaveToScene(FILE* _pFile)
{
}

void PlayerScript::LoadFromScene(FILE* _pFile)
{
}
