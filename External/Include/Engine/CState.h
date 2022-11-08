#pragma once
#include "CEntity.h"
/*
	* �� ���´� wstring ���� �����մϴ�. 

	* ���º� Ÿ�̸Ӵ� ���� ���°� ���۵� �� 
	* ����ð��� ���� �����ƽ��ϴ�.

	* m_bTimePlay �� Ű�� �� �� �ֽ��ϴ�. 
*/
class CState :
    public CEntity
{
protected:
	CGameObject*	m_pOwner;		

	float			m_fTimer;		// ���º� Ÿ�̸� 
	wstring			m_sStateType;	// ���� Ÿ�� �̸� 

	bool			m_bTimePlay;	// Ÿ�̸� ��� 

public:
	virtual void Init() {};
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
