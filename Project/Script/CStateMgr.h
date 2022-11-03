#pragma once
#include <Engine/CEntity.h>
#include "CObjKeyMgr.h"

class CStateMgr
	: public CEntity
{
private:
	CGameObject*			m_pOwnerObj;

	wstring					m_sPrevState;
	wstring					m_sCurstate;

	tKey_Zip				m_tCurKeyInfo;
	tKey_Zip				m_tPrevKeyInfo;

public:
	void	Update();
	void	LateUpdate();


public:
	void InitState(CGameObject* _pOwner);


public:
	// [ SET PART ]
	void SetCurKeyState(KEY_STATE _eKeyState)		{ m_tCurKeyInfo.eKeyState = _eKeyState; }
	void SetCurKey(KEY _eKey)						{ m_tCurKeyInfo.eKey = _eKey; }
	void SetKeyInfo(KEY_STATE _eKeyState, KEY _eKey, tKeyOptionFlags_Zip _tFlags);
	void SetCurKeyInfo(tKey_Zip _tCurKeyInfo)		{ m_tCurKeyInfo = _tCurKeyInfo; }
	void SetPrevKeyInfo(tKey_Zip _tPrevKeyInfo)		{ m_tPrevKeyInfo = _tPrevKeyInfo; }

	// [ GET PART ]
	wstring GetCurstate()							{ return m_sCurstate; }
	wstring GetPrevState()							{ return m_sPrevState; }


private:
	wstring ChangeStateByKeyInfo();
	void ChangeState(wstring _sStateName);

	wstring Check_Tap();
	wstring Check_Pressed();
	wstring Check_Away();



	CLONE(CStateMgr)

public:
	CStateMgr();
	virtual ~CStateMgr();

};

