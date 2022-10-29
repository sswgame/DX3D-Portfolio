#include "pch.h"
#include "CObjKeyMgr.h"


// 



CObjKeyMgr::CObjKeyMgr()
{
	Init();

}

CObjKeyMgr::CObjKeyMgr(const CObjKeyMgr& _origin)
{
	Init();

}

CObjKeyMgr::~CObjKeyMgr()
{
}

void CObjKeyMgr::Init()
{
	// DEFAULT KEY SETTING

	m_tMovingKey.eForward		= KEY::W;
	m_tMovingKey.eBackWard		= KEY::S;
	m_tMovingKey.eLeft			= KEY::A;
	m_tMovingKey.eRight			= KEY::D;

	m_tMovingKey.eJump			= KEY::SPACE;
	m_tMovingKey.eInteract		= KEY::E;
	m_tMovingKey.eSprint		= KEY::LSHFT;
	m_tMovingKey.eWalk			= KEY::LALT;


	m_tCombatKey.eLightAttack	= KEY::LBTN;
	m_tCombatKey.eHeavyAttack	= KEY::RBTN;
	m_tCombatKey.eParry			= KEY::Q;
	m_tCombatKey.eCameraLock	= KEY::MBTN;

}


void CObjKeyMgr::update()
{
	m_tPrevKeyInfo = m_tCurKeyInfo;
	m_tCurKeyInfo.iKeyFlags = 0;

	CheckKeyTap();
	CheckKeyPressed();
	CheckKeyAway();

}

void CObjKeyMgr::lateupdate()
{

}


void CObjKeyMgr::CheckKeyTap()
{
	// KEY TAP
	if (KEY_TAP(m_tMovingKey.eForward))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::FORWARD;

		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eForward;


	}
	if (KEY_TAP(m_tMovingKey.eBackWard))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::BACKWARD;

		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eBackWard;

	}
	if (KEY_TAP(m_tMovingKey.eLeft))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::LEFT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eLeft;
	}
	if (KEY_TAP(m_tMovingKey.eRight))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::RIGHT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eRight;
	}


	if (KEY_TAP(m_tMovingKey.eJump))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::JUMP;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eJump;
	}
	if (KEY_TAP(m_tMovingKey.eInteract))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::INTERACT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eInteract;
	}
	if (KEY_TAP(m_tMovingKey.eSprint))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::SPRINT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eSprint;
	}
	if (KEY_TAP(m_tMovingKey.eWalk))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::TAP;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;
		m_tCurKeyInfo.eKeyState = KEY_STATE::TAP;
		m_tCurKeyInfo.eKey = m_tMovingKey.eWalk;
	}

}

void CObjKeyMgr::CheckKeyPressed()
{
	// KEY PRESSED
	if (KEY_PRESSED(m_tMovingKey.eForward))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::FORWARD;

		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eForward;

		if (KEY_TAP(m_tMovingKey.eWalk)
			|| KEY_PRESSED(m_tMovingKey.eWalk))
		{
			m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;
		}

	}
	if (KEY_PRESSED(m_tMovingKey.eBackWard))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::BACKWARD;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eBackWard;

		if (KEY_TAP(m_tMovingKey.eWalk)
			|| KEY_PRESSED(m_tMovingKey.eWalk))
		{
			m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;

		}
	}
	if (KEY_PRESSED(m_tMovingKey.eLeft))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::LEFT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eLeft;

		if (KEY_TAP(m_tMovingKey.eWalk)
			|| KEY_PRESSED(m_tMovingKey.eWalk))
		{
			m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;

		}

	}
	if (KEY_PRESSED(m_tMovingKey.eRight))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::RIGHT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eRight;

		if (KEY_TAP(m_tMovingKey.eWalk)
			|| KEY_PRESSED(m_tMovingKey.eWalk))
		{
			m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;

		}

	}


	if (KEY_PRESSED(m_tMovingKey.eJump))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::JUMP;

		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eJump;

		if (KEY_TAP(m_tMovingKey.eJump) ||
			KEY_PRESSED(m_tMovingKey.eJump))
		{
			m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::DOUBLE_JUMP;
		}
	}

	if (KEY_PRESSED(m_tMovingKey.eInteract))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::INTERACT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eInteract;
	}

	if (KEY_PRESSED(m_tMovingKey.eSprint))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::SPRINT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eSprint;
	}

	if (KEY_PRESSED(m_tMovingKey.eWalk))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::PRESSED;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;
		m_tCurKeyInfo.eKeyState = KEY_STATE::PRESSED;
		m_tCurKeyInfo.eKey = m_tMovingKey.eWalk;
	}


}

void CObjKeyMgr::CheckKeyAway()
{
	// KEY AWAY
	if (KEY_AWAY(m_tMovingKey.eForward))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::FORWARD;

		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eForward;
	}
	if (KEY_AWAY(m_tMovingKey.eBackWard))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::BACKWARD;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eBackWard;
	}
	if (KEY_AWAY(m_tMovingKey.eLeft))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::LEFT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eLeft;
	}
	if (KEY_AWAY(m_tMovingKey.eRight))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::RIGHT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eRight;
	}


	if (KEY_AWAY(m_tMovingKey.eJump))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::JUMP;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eJump;
	}
	if (KEY_AWAY(m_tMovingKey.eInteract))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::INTERACT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eInteract;
	}
	if (KEY_AWAY(m_tMovingKey.eSprint))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::SPRINT;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eSprint;
	}
	if (KEY_AWAY(m_tMovingKey.eWalk))
	{
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::AWAY;
		m_tCurKeyInfo.iKeyFlags |= PLAYER_KEY_OPTION::WALK;
		m_tCurKeyInfo.eKeyState = KEY_STATE::AWAY;
		m_tCurKeyInfo.eKey = m_tMovingKey.eWalk;
	}

}


KEY_STATE CObjKeyMgr::GetCurKeyStateByFlags()
{
	// 1. Key State 해석 
	KEY_STATE eKeyState = KEY_STATE::NONE;
	eKeyState = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::TAP)		? eKeyState = KEY_STATE::TAP		: eKeyState = eKeyState;
	eKeyState = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PRESSED)	? eKeyState = KEY_STATE::PRESSED	: eKeyState = eKeyState;
	eKeyState = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::AWAY)		? eKeyState = KEY_STATE::AWAY		: eKeyState = eKeyState;
	return eKeyState;

}

KEY CObjKeyMgr::GetCurKeyByFlags()
{
	// 2. KEY 해석
	KEY eKey = KEY::KEY_END;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::FORWARD)		? eKey = m_tMovingKey.eForward		: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::BACKWARD)		? eKey = m_tMovingKey.eBackWard		: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::LEFT)			? eKey = m_tMovingKey.eLeft			: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::RIGHT)			? eKey = m_tMovingKey.eRight		: eKey = eKey;
	
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::JUMP)			? eKey = m_tMovingKey.eJump			: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::INTERACT)		? eKey = m_tMovingKey.eInteract		: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::SPRINT)		? eKey = m_tMovingKey.eSprint		: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::WALK)			? eKey = m_tMovingKey.eWalk			: eKey = eKey;
	
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::LIGHT_ATTACK)	? eKey = m_tCombatKey.eLightAttack	: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::HEAVY_ATTACK)	? eKey = m_tCombatKey.eHeavyAttack	: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PARRY)			? eKey = m_tCombatKey.eParry		: eKey = eKey;
	eKey = (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::CAMERA_LOCK)	? eKey = m_tCombatKey.eCameraLock	: eKey = eKey;

	return eKey;

}