#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ATTACKSCRIPT,
	BOSSSTAGESCRIPT,
	CAMERAMOVESCRIPT,
	DEMONSCRIPT,
	GRAVITYSCRIPT,
	JUMPATTACKSCRIPT,
	LANDSCAPESCRIPT,
	MISSILESCRIPT,
	MONSTERATTACKSCRIPT,
	MONSTERHPSCRIPT,
	MONSTERSCRIPT,
	OBJSTACLESCRIPT,
	PLAYERSCRIPT,
	STAIRSSCRIPT,
	TERRAINSCRIPT,
	TESTSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
