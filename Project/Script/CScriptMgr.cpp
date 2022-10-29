
#include "pch.h"

#include "CScriptMgr.h"
#include "CameraMoveScript.h"
#include "MissileScript.h"
#include "PlayerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"MissileScript");
	_vec.push_back(L"PlayerScript");
}


CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CameraMoveScript"== _strScriptName)
		return new CameraMoveScript;
	if (L"MissileScript"== _strScriptName)
		return new MissileScript;
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
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		}
	return nullptr;
}

