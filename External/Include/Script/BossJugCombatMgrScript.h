#pragma once
#include <Engine\CScript.h>

#include "BossJugScript.h"
#include "BossJugHandScript.h"

class BossJugCombatMgrScript :
    public CScript
{
private:
	CFSM*				m_pPhaseState;

	BossJugScript*		m_pJugScript;
	BossJugHandScript*	m_pJugHandScript;

public:
	void InitState();

	virtual void start();
	virtual void update();
	virtual void lateupdate();

	CLONE(BossJugCombatMgrScript)

public:
	BossJugCombatMgrScript();
	virtual ~BossJugCombatMgrScript();

};

