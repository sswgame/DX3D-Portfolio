#include "pch.h"
#include "BossJugScript.h"

BossJugScript::BossJugScript()
	:CScript((int)SCRIPT_TYPE::BOSSJUGSCRIPT)
{
}

BossJugScript::~BossJugScript()
{
}


void BossJugScript::start()
{
	CScript::start();
}

void BossJugScript::update()
{
	CScript::update();
}

void BossJugScript::lateupdate()
{
	CScript::lateupdate();
}
