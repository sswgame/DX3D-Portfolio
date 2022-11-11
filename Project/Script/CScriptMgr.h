
#pragma once

#include <vector>
#include <string>
using namespace std;

enum class SCRIPT_TYPE
{	BOSSJUGCOMBATMGRSCRIPT,
	BOSSJUGHANDSCRIPT,
	BOSSJUGSCRIPT,
	CAMERAMOVESCRIPT,
	GRAVITYSCRIPT,
	HANDSTATEMGRSCRIPT,
	MISSILESCRIPT,
	MONSTERGUNSCRIPT,
	MONSTERSHIELDSCRIPT,
	MONSTERSWORDSCRIPT,
	PLAYERCAMSCRIPT,
	PLAYERSCRIPT,

    END
};

class CScript;

class CScriptMgr
{
public:
    static void GetScriptInfo(vector<wstring>& _vec);
    static CScript* GetScript(const wstring& _strScriptName);
    static CScript* GetScript(UINT _iScriptType);
    static const wchar_t* GetScriptName(CScript* _pScript);
};
