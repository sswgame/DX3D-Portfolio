#pragma once
#include <Engine/CEntity.h>
#include "CObjKeyMgr.h"

class CStateMgr
	: public CEntity
{
private:
	wstring					m_sPrevState;
	wstring					m_sCurstate;
	CGameObject*			m_pOwnerObj;

	tKey_Zip				m_tCurKeyInfo;
	tKey_Zip				m_tPrevKeyInfo;

public:
	void	Update();
	void	LateUpdate();


public:
	void InitState(CGameObject* _pOwner);


public:

	void SetCurKeyState(KEY_STATE _eKeyState)	{ m_tCurKeyInfo.eKeyState = _eKeyState; }
	void SetCurKey(KEY _eKey)					{ m_tCurKeyInfo.eKey = _eKey; }
	void SetKeyInfo(KEY_STATE _eKeyState, KEY _eKey, KeyOptionFlags _eFlags);
	void SetCurKeyInfo(tKey_Zip _tCurKeyInfo)		{ m_tCurKeyInfo = _tCurKeyInfo; }
	void SetPrevKeyInfo(tKey_Zip _tPrevKeyInfo)		{ m_tPrevKeyInfo = _tPrevKeyInfo; }


private:
	wstring ChangeStateByKeyInfo();
	void ChangeState(wstring _sStateName);


	CLONE(CStateMgr)

public:
	CStateMgr();
	virtual ~CStateMgr();

};

