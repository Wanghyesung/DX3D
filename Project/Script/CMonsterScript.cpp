#include "pch.h"
#include "CMonsterScript.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CRigidbody.h>
#include <Engine\CMonsterFSM.h>

#include <Engine\CMonsterMove.h>

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	m_pFSM(nullptr)
{
	
}

CMonsterScript::~CMonsterScript()
{
	if (m_pFSM != nullptr)
		delete m_pFSM;
}

void CMonsterScript::tick()
{
	m_pFSM->final_tick();
}

void CMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	
}

void CMonsterScript::Initialize(const wstring& _strFbxName)
{
	CGameObject* pMonster = GetOwner();
	// ============
	// FBX Loading
	// ============	

	vector<Ptr<CMeshData>> vecMeshData = {};
	CGameObject* pObj = nullptr;

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\house.mdat");
	//pObj = pMeshData->Instantiate();
	//pObj->SetName(L"House");

	//Artorias4.fbx
	vector<Ptr<CMeshData>> pVecMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\" + _strFbxName + L".fbx");

	for (int i = 0; i < pVecMeshData.size(); ++i)
	{
		wstring strNum = std::to_wstring(i);
		//Ptr<CMeshData> pMeshData =
		//	CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\Artorias4" + strNum + L".mdat");
		//Ptr<CMeshData> pMeshData = pVecMeshData[i];

		Ptr<CMeshData> pMeshData = pVecMeshData[i];
		if (pMeshData != nullptr)
		{
			pObj = pMeshData->Instantiate();
			pObj->SetName(_strFbxName + strNum);
			pMonster->AddChild(pObj);
		}
	}

	Transform()->SetRelativeScale(2.f, 2.f, 2.f);
	Transform()->SetRelativeRot(-XM_PI / 2.f, 0.f, 0.f);

	m_pFSM = new CMonsterFSM();
	m_pFSM->SetOwner(pMonster);

	CMonsterMove* pMove = new CMonsterMove();
	m_pFSM->AddState(MONSTER_STATE_TYPE::RUN, pMove);

	m_pFSM->SetState(MONSTER_STATE_TYPE::RUN);
	
	ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::RUN);
	(m_pFSM, STATE_TYPE::IDLE);
}

void CMonsterScript::AddAnimFrame(const wstring& _strAnimName, int _iStart, int _iEnd)
{
	const vector<CGameObject*>& vecGameObj = GetOwner()->GetChild();
	UINT iSize = vecGameObj.size();
	for (UINT i = 0; i < iSize; ++i)
	{
		vecGameObj[i]->Animator3D()->CreateAnimationF(_strAnimName, _iStart, _iEnd);
	}
}

