#pragma once
#include "CEntity.h"
/*
	* �� ���´� wstring ���� �����մϴ�. 

	* ���º� Ÿ�̸Ӵ� ���� ���°� ���۵� �� 
	* ����ð��� ���� �����ƽ��ϴ�.

	* m_bTimePlay �� Ű�� �� �� �ֽ��ϴ�. 
*/
class CState : public CEntity
{
protected:
	CGameObject* m_pOwner;

	float   m_fTimer;		// ���º� Ÿ�̸� 
	wstring m_sStateType;	// ���� Ÿ�� �̸� 

	bool m_bTimePlay;	// Ÿ�̸� ��� 

public:
	virtual void Init() {};
	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update();
	virtual void LateUpdate();

public:
	const std::wstring& GetStateType() const { return m_sStateType; }
	void                SetStateType(wstring _sState) { m_sStateType = _sState; }

	CGameObject* GetOwner() const { return m_pOwner; }
	void         SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }

public:
	float GetTimer() const { return m_fTimer; }
	void  PlayTimer();
	void  StopTimer();
	void  ResetTimer();

public:
	CState* Clone() override = 0;
	CState(wstring _sState);
	virtual ~CState();

	friend class CFSM;
	friend class CEventMgr;
};
