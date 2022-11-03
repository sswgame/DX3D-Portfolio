#pragma once
#include <Engine\CScript.h>

class BossJugHandScript :
    public CScript
{
private:

public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	CLONE(BossJugHandScript)

public:
	BossJugHandScript();
	virtual ~BossJugHandScript();

};

