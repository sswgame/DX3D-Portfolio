#pragma once
#include <Engine/CEntity.h>

class CHand_StateMgr :
	public CEntity
{
private:
	wstring					m_sPrevState;
	wstring					m_sCurstate;
	CGameObject* m_pOwnerObj;

	wstring					m_sNextState;

public:
	void	Update();
	void	LateUpdate();


public:
	void InitState(CGameObject* _pOwner);


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

