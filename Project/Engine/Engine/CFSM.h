#pragma once
#include "CComponent.h"
#include "CState.h"

class CFSM 
	: public CComponent
{
private:
	// [ STATE INFO ]
	map<wstring, CState*>	m_mapState;

	CState*					m_pCurState;

	wstring					m_sPrevStateType;
	wstring					m_sCurStateType;
	wstring					m_sNextStateType;

public:
	void DeleteState(wstring _eStateType);
	void AddState(wstring _sStateType, CState* _pState);
	void ChangeState(wstring _sStateType);
	
public:
	// [ GET PART ]
	CState* GetCurState()		{ return m_pCurState; }
	wstring GetPrevStateType()	{ return m_sPrevStateType; }
	wstring GetCurStateType()	{ return m_sCurStateType; }
	wstring GetNextStateType()	{ return m_sNextStateType; }

	// [ SET PART ]
	void SetCurState(CState* _pCurState) { m_pCurState = _pCurState; }

public:
	virtual void finalupdate() override;
	virtual void update() override;
	virtual void lateupdate() override;

public:
	friend class CEventMgr;
	CLONE_DISABLE(CFSM);
	CFSM();
	virtual ~CFSM();
};

//#define BIND_FN(func) std::bind(func,this,std::placeholders::_1)
