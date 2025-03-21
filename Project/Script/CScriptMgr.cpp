#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackScript.h"
#include "CBossStageScript.h"
#include "CCameraMoveScript.h"
#include "CDemonScript.h"
#include "CGravityScript.h"
#include "CJumpAttackScript.h"
#include "CLandScpaeScript.h"
#include "CLightMoveScript.h"
#include "CMissileScript.h"
#include "CMonsterAttackScript.h"
#include "CMonsterHPScript.h"
#include "CMonsterScript.h"
#include "CObjstacleScript.h"
#include "CPlayerScript.h"
#include "CStairsScript.h"
#include "CTerrainScript.h"
#include "CTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackScript");
	_vec.push_back(L"CBossStageScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CDemonScript");
	_vec.push_back(L"CEquipScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CJumpAttackScript");
	_vec.push_back(L"CLandFormScript");
	_vec.push_back(L"CLandScpaeScript");
	_vec.push_back(L"CLightMoveScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterAttackScript");
	_vec.push_back(L"CMonsterHPScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CObjstacleScript");
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
	if (L"CDemonScript" == _strScriptName)
		return new CDemonScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CJumpAttackScript" == _strScriptName)
		return new CJumpAttackScript;
	if (L"CLandScpaeScript" == _strScriptName)
		return new CLandScpaeScript;
	if (L"CLightMoveScript" == _strScriptName)
		return new CLightMoveScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterAttackScript" == _strScriptName)
		return new CMonsterAttackScript;
	if (L"CMonsterHPScript" == _strScriptName)
		return new CMonsterHPScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CObjstacleScript" == _strScriptName)
		return new CObjstacleScript;
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
	case (UINT)SCRIPT_TYPE::DEMONSCRIPT:
		return new CDemonScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPATTACKSCRIPT:
		return new CJumpAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::LANDSCAPESCRIPT:
		return new CLandScpaeScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTMOVESCRIPT:
		return new CLightMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return new CMonsterAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERHPSCRIPT:
		return new CMonsterHPScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::OBJSTACLESCRIPT:
		return new CObjstacleScript;
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

	case SCRIPT_TYPE::DEMONSCRIPT:
		return L"CDemonScript";
		break;

	case SCRIPT_TYPE::EQUIPSCRIPT:
		return L"CEquipScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::JUMPATTACKSCRIPT:
		return L"CJumpAttackScript";
		break;

	case SCRIPT_TYPE::LANDFORMSCRIPT:
		return L"CLandFormScript";
		break;

	case SCRIPT_TYPE::LANDSCAPESCRIPT:
		return L"CLandScapeScript";
		break;

	case SCRIPT_TYPE::LIGHTMOVESCRIPT:
		return L"CLightMoveScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERATTACKSCRIPT:
		return L"CMonsterAttackScript";
		break;

	case SCRIPT_TYPE::MONSTERHPSCRIPT:
		return L"CMonsterHPScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::OBJSTACLESCRIPT:
		return L"CObjstacleScript";
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

	}
	return nullptr;
}