#pragma once
#include <Engine/CScript.h>

class CHand_StateMgr :
	public CScript
{
private:
	wstring					m_sPrevState;
	wstring					m_sCurstate;
	CGameObject*			m_pOwnerObj;

	CGameObject*			m_pHand1;
	CGameObject*			m_pHand2;
	CGameObject*			m_pHand3;


	wstring					m_sNextState;

public:
	void	Update();
	void	LateUpdate();
	void	init();


public:
	void InitState();


public:
	void SetNextState(wstring _state) { m_sNextState = _state; }

private:
	void ChangeState(wstring _sStateName);
	CGameObject* GetOwnerObject() { return m_pOwnerObj; }

	CLONE(CHand_StateMgr)

public:
	CHand_StateMgr();
	virtual ~CHand_StateMgr();
};

