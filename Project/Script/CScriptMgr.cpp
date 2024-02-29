#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackScript.h"
#include "CBossStageScript.h"
#include "CCameraMoveScript.h"
#include "CGravityScript.h"
#include "CLandScpaeScript.h"
#include "CMissileScript.h"
#include "CMonsterAttackScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CStairsScript.h"
#include "CTerrainScript.h"
#include "CTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackScript");
	_vec.push_back(L"CBossStageScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CEquipScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CLandFormScript");
	_vec.push_back(L"CLandScpaeScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterAttackScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStairsScript");
	_vec.push_back(L"CTerrainScript");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"StairsScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackScript" == _strScriptName)
		return new CAttackScript;
	if (L"CBossStageScript" == _strScriptName)
		return new CBossStageScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CLandScpaeScript" == _strScriptName)
		return new CLandScpaeScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterAttackScript" == _strScriptName)
		return new CMonsterAttackScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CStairsScript" == _strScriptName)
		return new CStairsScript;
	if (L"CTerrainScript" == _strScriptName)
		return new CTerrainScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTACKSCRIPT:
		return new CAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSTAGESCRIPT:
		return new CBossStageScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::LANDSCAPESCRIPT:
		return new CLandScpaeScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return new CMonsterAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::STAIRSSCRIPT:
		return new CStairsScript;
		break;
	case (UINT)SCRIPT_TYPE::TERRAINSCRIPT:
		return new CTerrainScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTACKSCRIPT:
		return L"CAttackScript";
		break;

	case SCRIPT_TYPE::BOSSSTAGESCRIPT:
		return L"CBossStageScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::EQUIPSCRIPT:
		return L"CEquipScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::LANDFORMSCRIPT:
		return L"CLandFormScript";
		break;

	case SCRIPT_TYPE::LANDSCAPESCRIPT:
		return L"CLandScpaeScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return L"CMonsterAttackScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::STAIRSSCRIPT:
		return L"CStairsScript";
		break;

	case SCRIPT_TYPE::TERRAINSCRIPT:
		return L"CTerrainScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::TAIRSSCRIPT:
		return L"StairsScript";
		break;

	}
	return nullptr;
}