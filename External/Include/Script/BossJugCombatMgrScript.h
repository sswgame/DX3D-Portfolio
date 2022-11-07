#pragma once
#include <Engine/CScript.h>

class CGameObject;

class BossJugCombatMgrScript :
    public CScript
{
private:
	CFSM*				m_pPhaseFSM;
	wstring				m_wstrCurState;

	CGameObject*		m_pJug;
	CGameObject*		m_pJugHand;

	CGameObject*		m_pStageInnerCollider;
	CGameObject*		m_pStageOuterCollider;

public:
	void SpawnStage();
	void InitState();

	void CheckPhase() const;

	// [ GET / SET ]
	CGameObject* GetJug() { return m_pJug;}
	CGameObject* GetJugHand() { return m_pJugHand;}

	void start() override;
	void update() override;
	void lateupdate() override;

	CLONE(BossJugCombatMgrScript)

public:
	BossJugCombatMgrScript();
	virtual ~BossJugCombatMgrScript();

};

