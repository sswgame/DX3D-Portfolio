
#include "pch.h"

#include "CScriptMgr.h"
#include "BossJugCombatMgrScript.h"
#include "BossJugHandScript.h"
#include "BossJugScript.h"
#include "CameraMoveScript.h"

#include "GravityScript.h"
#include "MissileScript.h"
#include "MonsterGunScript.h"
#include "MonsterShieldScript.h"
#include "MonsterSwordScript.h"
#include "PlayerCamScript.h"
#include "PlayerScript.h"
#include "Hand_StateMgr.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"BossJugCombatMgrScript");
	_vec.push_back(L"BossJugHandScript");
	_vec.push_back(L"BossJugScript");
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"GravityScript");
	_vec.push_back(L"MissileScript");
	_vec.push_back(L"MonsterGunScript");
	_vec.push_back(L"MonsterShieldScript");
	_vec.push_back(L"MonsterSwordScript");
	_vec.push_back(L"PlayerCamScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"Hand_StateMgr");
}


CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"BossJugCombatMgrScript"== _strScriptName)
		return new BossJugCombatMgrScript;
	if (L"BossJugHandScript"== _strScriptName)
		return new BossJugHandScript;
	if (L"BossJugScript"== _strScriptName)
		return new BossJugScript;
	if (L"CameraMoveScript"== _strScriptName)
		return new CameraMoveScript;
	if (L"GravityScript"== _strScriptName)
		return new GravityScript;
	if (L"MissileScript"== _strScriptName)
		return new MissileScript;
	if (L"MonsterGunScript"== _strScriptName)
		return new MonsterGunScript;
	if (L"MonsterShieldScript"== _strScriptName)
		return new MonsterShieldScript;
	if (L"MonsterSwordScript"== _strScriptName)
		return new MonsterSwordScript;
	if (L"PlayerCamScript"== _strScriptName)
		return new PlayerCamScript;
	if (L"PlayerScript"== _strScriptName)
		return new PlayerScript;
	if (L"Hand_StateMgr" == _strScriptName)
		return new Hand_StateMgr;
	return nullptr;
}


CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
    switch((SCRIPT_TYPE)_iScriptType)
    {
	case SCRIPT_TYPE::BOSSJUGCOMBATMGRSCRIPT:
		return new BossJugCombatMgrScript;
	case SCRIPT_TYPE::BOSSJUGHANDSCRIPT:
		return new BossJugHandScript;
	case SCRIPT_TYPE::BOSSJUGSCRIPT:
		return new BossJugScript;
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CameraMoveScript;
	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return new GravityScript;
	case SCRIPT_TYPE::MISSILESCRIPT:
		return new MissileScript;
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return new MonsterGunScript;
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return new MonsterShieldScript;
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return new MonsterSwordScript;
	case SCRIPT_TYPE::PLAYERCAMSCRIPT:
		return new PlayerCamScript;
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return new PlayerScript;
	case SCRIPT_TYPE::HAND_STATEMGR:
		return new Hand_StateMgr;
	}
	return nullptr;
}


const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
    switch((SCRIPT_TYPE)_pScript->GetScriptType())
    {
	case SCRIPT_TYPE::BOSSJUGCOMBATMGRSCRIPT:
		return L"BossJugCombatMgrScript";
	case SCRIPT_TYPE::BOSSJUGHANDSCRIPT:
		return L"BossJugHandScript";
	case SCRIPT_TYPE::BOSSJUGSCRIPT:
		return L"BossJugScript";
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CameraMoveScript";
	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"GravityScript";
	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"MissileScript";
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return L"MonsterGunScript";
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return L"MonsterShieldScript";
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return L"MonsterSwordScript";
	case SCRIPT_TYPE::PLAYERCAMSCRIPT:
		return L"PlayerCamScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
	case SCRIPT_TYPE::HAND_STATEMGR:
		return L"Hand_StateMgr";
		}
	return nullptr;
}

