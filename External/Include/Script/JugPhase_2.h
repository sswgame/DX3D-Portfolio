#pragma once
#include <Engine\CState.h>

class BossJugCombatMgrScript;
class CAnimator3D;
class CFSM;

class JugPhase_2 : public CState
{
private:
	BossJugCombatMgrScript* m_pCombatMgr;
	CFSM*                   m_pBossFSM;
	CAnimator3D*            m_pBossAnimator;

	float m_fIdleTime;
	UINT  m_iPrevAttackPattern;
	UINT  m_iAttackPattern;
	
	bool m_bAttackProceeding;
	bool m_bRot;

private:
	void RotTowardPlayer();
	void ChangePattern();

	void Attack_1();	// �� ��� ����
	void Attack_2();	// ��ġ ����
	void Attack_3();	// ���� ����
	void Attack_4();	// ����ü ����

public:
	virtual void Init();
	virtual void Enter();
	virtual void Exit();

	virtual void Update();
	virtual void LateUpdate();

	CLONE(JugPhase_2)

public:
	JugPhase_2();
	JugPhase_2(const JugPhase_2& _origin);
	virtual ~JugPhase_2();
};
