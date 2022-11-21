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

	void Attack_1();	// 빛 기둥 공격
	void Attack_2();	// 망치 공격
	void Attack_3();	// 번개 공격
	void Attack_4();	// 투사체 공격

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
