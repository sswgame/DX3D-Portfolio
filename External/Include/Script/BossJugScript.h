#pragma once
#include <Engine\CScript.h>

class CAnimatior3D;
class CState;


class BossJugScript :
    public CScript
{
private:
	CFSM*			m_pBossFSM;
	CAnimator3D*	m_pBossAnimator;

	float m_fHP;
	float m_fMaxHP;

public:
	// [ GET / SET]
	CFSM* GetBossFSM()	{ return m_pBossFSM; }

	float GetHP()		{ return m_fHP; }
	float GetMaxHP()	{ return m_fMaxHP; }

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
	virtual ~BossJugScript();

};

