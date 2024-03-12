#include "pch.h"
#include "CMonsterScript.h"
#include "CMonsterAttackScript.h"
#include "CAttackScript.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CRigidbody.h>
#include <Engine\CMonsterFSM.h>

#include <Engine\CMonsterMove.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CMonsterHit.h>
#include <Engine\CTransform.h>
#include <Engine\CCollider3D.h>

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

void CMonsterScript::BeginOverlap(CCollider3D* _Other)
{

}

void CMonsterScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CAttackScript* pAttack = pObj->GetScript<CAttackScript>();
	if (pAttack)
	{
		bool bOn = pAttack->IsAttackOn(GetOwner()->GetID());
		if (bOn)
		{
			CMonsterHit* pHit = dynamic_cast<CMonsterHit*>(m_pFSM->FindState(MONSTER_STATE_TYPE::HIT));
			pHit->SetHitInfo(m_tHitInfo);
			ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::HIT);
			
		}
	}
}

void CMonsterScript::EndOverlap(CCollider3D* _Other)
{

}


void CMonsterScript::begin()
{
	CMonsterState* pState = m_pFSM->FindState(MONSTER_STATE_TYPE::IDLE);
	if (pState == nullptr)
		assert(nullptr);

	m_pFSM->SetState(MONSTER_STATE_TYPE::IDLE);

	ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::HIT);
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

	//Transform()->SetRelativeScale(2.f, 2.f, 2.f);
	Transform()->SetRelativeRot(-XM_PI / 2.f, 0.f, 0.f);

	m_pFSM = new CMonsterFSM();
	m_pFSM->SetOwner(pMonster);
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

void CMonsterScript::AddMonsterState(MONSTER_STATE_TYPE _eType,CMonsterState* _pState,
									 const wstring& _strAnimName, int _iStartFrame, int _iEndFrame)
{
	if (!m_pFSM)
		assert(nullptr);

	m_pFSM->AddState(_eType, _pState);
	_pState->SetName(_strAnimName);

	const vector<CGameObject*>& vecGameObj = GetOwner()->GetChild();
	UINT iSize = vecGameObj.size();
	for (UINT i = 0; i < iSize; ++i)
	{
		vecGameObj[i]->Animator3D()->CreateAnimationF(_strAnimName, _iStartFrame, _iEndFrame);
	}
}

void CMonsterScript::AddMonsterState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState, const wstring& _strAnimName)
{
	if (!m_pFSM)
		assert(nullptr);

	m_pFSM->AddState(_eType, _pState);
	_pState->SetName(_strAnimName);
}

void CMonsterScript::AddMonsterAttack(int _iAttackNum, float _fForce, float _fRotate, float _fTime, int _iStartFrame, int _iEndFrame,
									Vec3 _vAttackScale, float _fOffsetPos, Vec3 _vAttackRot)
{
	tAttackInfo tAttackInfo = {};
	tAttackInfo.iAttackNum = _iAttackNum;
	tAttackInfo.fForce = _fForce;
	tAttackInfo.fRotate = _fRotate;
	tAttackInfo.fMoveTime = _fTime;

	tAttackInfo.iStartFrame = _iStartFrame;
	tAttackInfo.iEndFrame = _iEndFrame;

	tAttackInfo.vAttackScale = _vAttackScale;
	tAttackInfo.fOffsetPos = _fOffsetPos;
	tAttackInfo.vAttackRot = _vAttackRot;

	//고정 데미지
	tAttackInfo.tAttackValue.bDown = false;
	tAttackInfo.tAttackValue.fAttRcnt = 5.f;
	tAttackInfo.tAttackValue.fDamage = 5.f;

	CGameObject* pAttackObj = new CGameObject();
	CMonsterAttackScript* pAttack = new CMonsterAttackScript();
	pAttack->SetAttackValue(tAttackInfo.tAttackValue);
	pAttackObj->AddComponent(pAttack);
	m_pFSM->AddMonsterAttack(tAttackInfo, pAttackObj);
}
