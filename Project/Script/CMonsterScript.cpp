#include "pch.h"
#include "CMonsterScript.h"
#include "CMonsterAttackScript.h"
#include "CAttackScript.h"
#include "CMonsterHPScript.h"

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
	m_pFSM(nullptr),
	m_bBoss(false)
{
	
}

CMonsterScript::CMonsterScript(SCRIPT_TYPE _eType)
	: CScript(_eType),
	m_pFSM(nullptr),
	m_bBoss(false)
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
			//데미지
			CMonsterHit* pHit = dynamic_cast<CMonsterHit*>(m_pFSM->FindState(MONSTER_STATE_TYPE::HIT));
			pHit->SetHitInfo(m_tHitInfo);
			m_tMonsterInfo.fHP -= m_tHitInfo.fDamage;
			
			if (m_pFSM->GetCurStateType() == MONSTER_STATE_TYPE::HIT)
				return;

			ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::HIT);
		}

		if (m_tMonsterInfo.fHP <= 0.f)
		{
			if (m_bDead)
				return;

			ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::DEAD);
			m_bDead = true;

		}

		m_pHp->UpdateGage(m_tMonsterInfo.fMaxHP, m_tMonsterInfo.fHP);
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

	ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::IDLE);
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
	pMonster->SetName(_strFbxName);

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


	init_hp();
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

void CMonsterScript::Chanage_AnimDT(float _fDivDT)
{
	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
		vecChild[i]->Animator3D()->SetAnimDivDT(_fDivDT);
	}
}

void CMonsterScript::init_hp()
{
	m_tMonsterInfo.fHP = 600.f;
	m_tMonsterInfo.fMaxHP = 600.f;

	CGameObject* pHP = new CGameObject();
	m_pHp = new CMonsterHPScript();
	m_pHp->Initialize(L"texture\\GameTexture\\Monster",Vec3(360,35.f,-2.f),GetOwner()->GetName(),false);
	m_pHp->SetMonster(GetOwner());
	
	pHP->AddComponent(m_pHp);
	pHP->AddComponent(new CTransform());
	SpawnGameObject(pHP, Vec3::Zero, (int)LAYER_TYPE::Default);//3차원 공간
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
	tAttackInfo.vForce = Vec3::Zero;
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
	tAttackInfo.tAttackValue.fAttRcnt = 200.f;
	tAttackInfo.tAttackValue.fDamage = 30.f;
	tAttackInfo.tAttackValue.fAddForceTime = 0.5f;

	CGameObject* pAttackObj = new CGameObject();
	CMonsterAttackScript* pAttack = new CMonsterAttackScript();
	pAttack->SetAttackValue(tAttackInfo.tAttackValue);
	pAttackObj->AddComponent(pAttack);
	m_pFSM->AddMonsterAttack(tAttackInfo, pAttackObj);
}

void CMonsterScript::AddMonsterAttack(const tAttackInfo& _tAttackInfo)
{
	CGameObject* pAttackObj = new CGameObject();
	CMonsterAttackScript* pAttack = new CMonsterAttackScript();
	pAttack->SetAttackValue(_tAttackInfo.tAttackValue);
	pAttackObj->AddComponent(pAttack);
	m_pFSM->AddMonsterAttack(_tAttackInfo, pAttackObj);
}
