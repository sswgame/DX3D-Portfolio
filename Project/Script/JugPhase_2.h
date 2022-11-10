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

	UINT m_iAttackPattern;
	bool m_bCurAttackEnd;
private:
	void Attack_0();	// 빛 기둥 공격
	void Attack_1();	// 망치 공격
	void Attack_2();	// 번개 공격
	void Attack_3();	// 투사체 공격

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
