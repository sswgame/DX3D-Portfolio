
#pragma once

#include <vector>
#include <string>
using namespace std;

enum class SCRIPT_TYPE
{	BOSSJUGCOMBATMGRSCRIPT,
	BOSSJUGHANDSCRIPT,
	BOSSJUGSCRIPT,
	CAMERAMOVESCRIPT,
	CINEMACAMSCRIPT,
	COLUMNFLAMESCRIPT,
	DESTROYSCRIPT,
	DOORSCRIPT,
	EFFECTSCRIPT,
	ENERGYBALLSCRIPT,
	FIELDMONSTESCRIPT,
	HAMMERSCRIPT,
	HANDSTATEMGRSCRIPT,
	ITEMSCRIPT,
	MAGMASCRIPT,
	MAINMENUSCRIPT,
	MISSILESCRIPT,
	MONSTERGUNSCRIPT,
	MONSTERSEARCHSCRIPT,
	MONSTERSHIELDSCRIPT,
	MONSTERSWORDSCRIPT,
	M_ATTACKSCRIPT,
	PAPERBURNSCRIPT,
	PLAYERCAMSCRIPT,
	PLAYERSCRIPT,
	POTSCRIPT,
	SCREENSHAKESCRIPT,
	SOCKETCOLLIDERSCRIPT,
	SWORDTRAILSCRIPT,
	TESTDEADSCRIPT,
	TOTEMSCRIPT,
	TRAILSCRIPT,

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
