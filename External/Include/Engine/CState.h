#pragma once
#include "CEntity.h"
/*
	* 각 상태는 wstring 으로 저장합니다. 

	* 상태별 타이머는 개별 상태가 시작된 후 
	* 진행시간을 위해 생성됐습니다.

	* m_bTimePlay 로 키고 끌 수 있습니다. 
*/
class CState :
    public CEntity
{
protected:
	CGameObject*	m_pOwner;		

	float			m_fTimer;		// 상태별 타이머 
	wstring			m_sStateType;	// 상태 타입 이름 

	bool			m_bTimePlay;	// 타이머 토글 

public:
	virtual void Init()		{};
	virtual void Enter()	= 0;
	virtual void Exit()		= 0;

	virtual void Update();
	virtual void LateUpdate();

public:
	// [ GET PART ]
	wstring		 GetStateType()	{ return m_sStateType; }
	float		 GetTimer()		{ return m_fTimer; }
	CGameObject* GetOwner()		{ return m_pOwner; }

	// [ SET PART ]
	void SetStateType(wstring _sState) { m_sStateType = _sState; }
	void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }


public:
	// [ TIMER INFO ]
	void PlayTimer();
	void StopTimer();
	void ResetTimer();

	
public:
	CState* Clone() = 0;
	CState(wstring _sState);
	virtual ~CState();


public:
	friend class CFSM;
	friend class CEventMgr;
};
