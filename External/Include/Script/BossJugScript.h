#pragma once
#include <Engine\CScript.h>

class BossJugScript :
    public CScript
{
private:

public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	CLONE(BossJugScript)

public:
	BossJugScript();
	virtual ~BossJugScript();

};

