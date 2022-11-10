#pragma once
#include <Engine/CScript.h>

class Hand_StateMgr :
	public CScript
{
private:
	wstring					m_sPrevState;
	wstring					m_sCurstate;
	wstring					m_sNextState;

	CGameObject*			m_pHand1;
	CGameObject*			m_pHand2;
	CGameObject*			m_pHand3;


	int					m_iCurAttackHandNumber;
	int					m_iSaveMidFrame;

	bool				m_bGenDone;
	bool				m_bAllAttackDone;
	bool				m_bVanishDone;

	bool				m_bAttack1Done;
	bool				m_bAttack2Done;
	bool				m_bAttack3Done;


public:
	void	Update();
	void	LateUpdate();
	void	init();


public:
	void InitState();


public:
	 // [ SET ]
	void SetNextState(wstring _state) { m_sNextState = _state; }
	void SetGenDone(bool _TorF) { m_bGenDone = _TorF; }
	void SetAllAttackDone(bool _TorF) { m_bAllAttackDone = _TorF; }
	void SetVanishDone(bool _TorF) { m_bVanishDone = _TorF; }
	void SetCurAttackNumber(int _num) { m_iCurAttackHandNumber = _num; }
	void Set1stAttackDone(bool _TorF) { m_bAttack1Done = _TorF; }
	void Set2ndAttackDone(bool _TorF) { m_bAttack2Done = _TorF; }
	void Set3rdAttackDone(bool _TorF) { m_bAttack3Done = _TorF; }
	void SetSaveMidFrame(int _frame) { m_iSaveMidFrame = _frame; }


public:
	// [ GET ]
	int GetSavedMidFrame() { return m_iSaveMidFrame; }
	bool GetGenDone() { return m_bGenDone; }
	bool GetAllAttackDone() { return m_bAllAttackDone; }
	bool GetVanishDone() { return m_bVanishDone; }
	bool GetCurAttackHandNUM() { return m_iCurAttackHandNumber; }
	bool Get1stAttackDone() { return m_bAttack1Done; }
	bool Get2ndAttackDone() { return m_bAttack2Done; }
	bool Get3rdAttackDone() { return m_bAttack3Done; }

	CGameObject* GetHandObject(int _handNum) 
	{ 
		switch (_handNum)
		{
		case 1:
			return m_pHand1;
		case 2:
			return m_pHand2;
		case 3:
			return m_pHand3;
		}
	}

	wstring GetCurState() { return m_sCurstate; }

private:
	void ChangeState(wstring _sStateName);



	CLONE(Hand_StateMgr)

public:
	Hand_StateMgr();
	virtual ~Hand_StateMgr();
};

