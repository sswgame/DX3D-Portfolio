#include "pch.h"
#include "MonsterGunScript.h"

#include <Engine/CFSM.h>

#include "CAttack_deuxiemie.h"
#include "CDeath_deuxiemie.h"
#include "CIdle_deuxiemie.h"
#include "CRun_deuxiemie.h"
#include "CStun_deuxiemie.h"
#include "CWalk_deuxiemie.h"

void MonsterGunScript::start()
{
	FSM()->AddState(L"IDLE", new CIdle_deuxiemie{});
	FSM()->AddState(L"WALK", new CWalk_deuxiemie{});
	FSM()->AddState(L"RUN", new CRun_deuxiemie{});
	FSM()->AddState(L"DEATH", new CDeath_deuxiemie{});
	FSM()->AddState(L"STUN", new CStun_deuxiemie{});
	FSM()->AddState(L"ATTACK", new CAttack_deuxiemie{});

	FSM()->ChangeState(L"IDLE");
}

MonsterGunScript::MonsterGunScript()
	: CScript{(int)SCRIPT_TYPE::MONSTERGUNSCRIPT}
{
}

MonsterGunScript::~MonsterGunScript()
{
}
