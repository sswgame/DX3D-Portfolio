#include "pch.h"
#include "PlayerScript.h"

//#include "CState.h" // .. �� ���º� ������� CState.h �� �����߽��ϴ�. 

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

	// Ű ���� ���� �ʱ�ȭ
	if (nullptr == m_pKeyMgr)
	{
		m_pKeyMgr = new CObjKeyMgr;
		m_pKeyMgr->SetOwnerScript(this);
	}


	// ���� ���� ���� �ʱ�ȭ
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CStateMgr;
		m_pStateMgr->InitState(GetOwner());
	}


	// ���� ���� ���� �ʱ�ȭ 
	if (nullptr == m_pStat)
	{
		m_pStat = new CPlayerStat;
		m_pStat->SetOwnerScript(this);
		m_pStat->start();
	}


	// �÷��̾� ���� ���� �ʱ�ȭ 
	m_vDirection = Vec3(0.f, 0.f, 1.f);

}

void PlayerScript::update()
{

	// 1. Ű ���� ������Ʈ 
	m_pKeyMgr->update();
	tKey_Zip tCurKeyInfo = m_pKeyMgr->GetCurKeyInfo();
	tKey_Zip tPrevKeyInfo = m_pKeyMgr->GetPrevKeyInfo();

	// 2. �÷��̾� ���� ���� ������Ʈ 
	
	
	//UpdateDirection(tCurKeyInfo);


	// 3. �÷��̾� ���� ������Ʈ 
	m_pStateMgr->SetCurKeyInfo(tCurKeyInfo);
	m_pStateMgr->SetPrevKeyInfo(tPrevKeyInfo);
	m_pStateMgr->Update();

	// 4. �÷��̾� ���� ������Ʈ 
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
	// ���� üũ 
	// ���⼭ PLAYER_KEY_OPTION �� Ű�� ������ ������ �ǹ��Ѵ�.
	// NONE �� �������� ������ �ǹ�
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

	// ���� ��ķ� ������Ʈ
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
