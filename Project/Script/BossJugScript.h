#pragma once
#include <Engine\CScript.h>

class CAnimatior3D;
class CState;


class BossJugScript :
    public CScript
{
private:
	CFSM*  m_pBossFSM;
	string m_strCurState;

	CAnimator3D* m_pBossAnimator;
	string       m_strCurAnimName;
		
	float m_fHP;
	float m_fMaxHP;


public:
	// [ GET ]
	CFSM*        GetBossFSM()	{ return m_pBossFSM; }
	CAnimator3D* GetBossAnimator() { return m_pBossAnimator; }

	float GetHP() { return m_fHP; }
	float GetMaxHP()	{ return m_fMaxHP; }

	// [ Set ]
	void SetHP(float _hp) { m_fHP = _hp; }
	void SetMaxHP(float _hp) { m_fMaxHP = _hp; }

	// [ INIT ]
	void Init();
	void InitState();

	// [INTERACT]


	// [FLOW]
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

	CLONE(BossJugScript);

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	BossJugScript();
	BossJugScript(const BossJugScript& _origin);
	virtual ~BossJugScript();

};

