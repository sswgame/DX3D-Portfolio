
#include "pch.h"

#include "CScriptMgr.h"
#include "CameraMoveScript.h"
#include "MissileScript.h"
#include "MonsterGunScript.h"
#include "MonsterShieldScript.h"
#include "MonsterSwordScript.h"
#include "PlayerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"MissileScript");
	_vec.push_back(L"MonsterGunScript");
	_vec.push_back(L"MonsterShieldScript");
	_vec.push_back(L"MonsterSwordScript");
	_vec.push_back(L"PlayerScript");
}


CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CameraMoveScript"== _strScriptName)
		return new CameraMoveScript;
	if (L"MissileScript"== _strScriptName)
		return new MissileScript;
	if (L"MonsterGunScript"== _strScriptName)
		return new MonsterGunScript;
	if (L"MonsterShieldScript"== _strScriptName)
		return new MonsterShieldScript;
	if (L"MonsterSwordScript"== _strScriptName)
		return new MonsterSwordScript;
	if (L"PlayerScript"== _strScriptName)
		return new PlayerScript;
	return nullptr;
}


CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
    switch((SCRIPT_TYPE)_iScriptType)
    {
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CameraMoveScript;
	case SCRIPT_TYPE::MISSILESCRIPT:
		return new MissileScript;
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return new MonsterGunScript;
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return new MonsterShieldScript;
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return new MonsterSwordScript;
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return new PlayerScript;
	}
	return nullptr;
}


const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
    switch((SCRIPT_TYPE)_pScript->GetScriptType())
    {
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CameraMoveScript";
	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"MissileScript";
	case SCRIPT_TYPE::MONSTERGUNSCRIPT:
		return L"MonsterGunScript";
	case SCRIPT_TYPE::MONSTERSHIELDSCRIPT:
		return L"MonsterShieldScript";
	case SCRIPT_TYPE::MONSTERSWORDSCRIPT:
		return L"MonsterSwordScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		}
	return nullptr;
}

