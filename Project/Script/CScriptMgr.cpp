
#include "pch.h"

#include "CScriptMgr.h"
#include "BossJugCombatMgrScript.h"
#include "BossJugHandScript.h"
#include "BossJugScript.h"
#include "CameraMoveScript.h"
#include "ColumnFlameScript.h"
#include "DestroyScript.h"
#include "GravityScript.h"
#include "HandStateMgrScript.h"
#include "MagmaScript.h"
#include "MissileScript.h"
#include "MonsterGunScript.h"
#include "MonsterSearchScript.h"
#include "MonsterShieldScript.h"
#include "MonsterSwordScript.h"
#include "PlayerCamScript.h"
#include "PlayerScript.h"
#include "RigidBodyScript.h"
#include "SwordTrailScript.h"
#include "TrailScript.h"
#include "UITestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"BossJugCombatMgrScript");
	_vec.push_back(L"BossJugHandScript");
	_vec.push_back(L"BossJugScript");
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"ColumnFlameScript");
	_vec.push_back(L"DestroyScript");
	_vec.push_back(L"GravityScript");
	_vec.push_back(L"HandStateMgrScript");
	_vec.push_back(L"MagmaScript");
	_vec.push_back(L"MissileScript");
	_vec.push_back(L"MonsterGunScript");
	_vec.push_back(L"MonsterSearchScript");
	_vec.push_back(L"MonsterShieldScript");
	_vec.push_back(L"MonsterSwordScript");
	_vec.push_back(L"PlayerCamScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"RigidBodyScript");
	_vec.push_back(L"SwordTrailScript");
	_vec.push_back(L"TrailScript");
	_vec.push_back(L"UITestScript");
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
	if (L"ColumnFlameScript"== _strScriptName)
		return new ColumnFlameScript;
	if (L"DestroyScript"== _strScriptName)
		return new DestroyScript;
	if (L"GravityScript"== _strScriptName)
		return new GravityScript;
	if (L"HandStateMgrScript"== _strScriptName)
		return new HandStateMgrScript;
	if (L"MagmaScript"== _strScriptName)
		return new MagmaScript;
	if (L"MissileScript"== _strScriptName)
		return new MissileScript;
	if (L"MonsterGunScript"== _strScriptName)
		return new MonsterGunScript;
	if (L"MonsterSearchScript"== _strScriptName)
		return new MonsterSearchScript;
	if (L"MonsterShieldScript"== _strScriptName)
		return new MonsterShieldScript;
	if (L"MonsterSwordScript"== _strScriptName)
		return new MonsterSwordScript;
	if (L"PlayerCamScript"== _strScriptName)
		return new PlayerCamScript;
	if (L"PlayerScript"== _strScriptName)
		return new PlayerScript;
	if (L"RigidBodyScript"== _strScriptName)
		return new RigidBodyScript;
	if (L"SwordTrailScript"== _strScriptName)
		return new SwordTrailScript;
	if (L"TrailScript"== _strScriptName)
		return new TrailScript;
	if (L"UITestScript"== _strScriptName)
		return new UITestScript;
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
	case SCRIPT_TYPE::COLUMNFLAMESCRIPT:
		return new ColumnFlameScript;
	case SCRIPT_TYPE::DESTROYSCRIPT:
		return new DestroyScript;
	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return new GravityScript;
	case SCRIPT_TYPE::HANDSTATEMGRSCRIPT:
		return new HandStateMgrScript;
	case SCRIPT_TYPE::MAGMASCRIPT:
		return new MagmaScript;
	case SCRIPT_TYPE::MISSILESCRIPT:
		return new MissileScript;
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return new MonsterGunScript;
	case SCRIPT_TYPE::MONSTERSEARCHSCRIPT:
		return new MonsterSearchScript;
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return new MonsterShieldScript;
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return new MonsterSwordScript;
	case SCRIPT_TYPE::PLAYERCAMSCRIPT:
		return new PlayerCamScript;
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return new PlayerScript;
	case SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return new RigidBodyScript;
	case SCRIPT_TYPE::SWORDTRAILSCRIPT:
		return new SwordTrailScript;
	case SCRIPT_TYPE::TRAILSCRIPT:
		return new TrailScript;
	case SCRIPT_TYPE::UITESTSCRIPT:
		return new UITestScript;
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
	case SCRIPT_TYPE::COLUMNFLAMESCRIPT:
		return L"ColumnFlameScript";
	case SCRIPT_TYPE::DESTROYSCRIPT:
		return L"DestroyScript";
	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"GravityScript";
	case SCRIPT_TYPE::HANDSTATEMGRSCRIPT:
		return L"HandStateMgrScript";
	case SCRIPT_TYPE::MAGMASCRIPT:
		return L"MagmaScript";
	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"MissileScript";
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return L"MonsterGunScript";
	case SCRIPT_TYPE::MONSTERSEARCHSCRIPT:
		return L"MonsterSearchScript";
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return L"MonsterShieldScript";
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return L"MonsterSwordScript";
	case SCRIPT_TYPE::PLAYERCAMSCRIPT:
		return L"PlayerCamScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
	case SCRIPT_TYPE::RIGIDBODYSCRIPT:
		return L"RigidBodyScript";
	case SCRIPT_TYPE::SWORDTRAILSCRIPT:
		return L"SwordTrailScript";
	case SCRIPT_TYPE::TRAILSCRIPT:
		return L"TrailScript";
	case SCRIPT_TYPE::UITESTSCRIPT:
		return L"UITestScript";
		}
	return nullptr;
}

