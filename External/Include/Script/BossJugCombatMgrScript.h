#pragma once
#include <Engine/CScript.h>

class CGameObject;

class BossJugCombatMgrScript : public CScript
{
private:
	CFSM*  m_pPhaseFSM;
	string m_strCurState;

	CGameObject* m_pStage;
	CGameObject* m_pJug;
	CGameObject* m_pHammer;
	CGameObject* m_pJugHandMgr;
	CGameObject* m_pPlayer;

	CGameObject* m_pStageInnerCollider;
	CGameObject* m_pStageOuterCollider;

	bool m_bStartCombat;

public:
	void SpawnStage();
	void InitState();

	void CheckPhase() const;

	// [ GET / SET ]
	CGameObject* GetJug() { return m_pJug; }
	CGameObject* GetHammer() { return m_pHammer; }
	CGameObject* GetJugHand() { return m_pJugHandMgr; }
	CGameObject* GetPlayer() { return m_pPlayer; }

	void start() override;
	void update() override;
	void lateupdate() override;

	CLONE(BossJugCombatMgrScript)

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	BossJugCombatMgrScript();
	virtual ~BossJugCombatMgrScript();

};

