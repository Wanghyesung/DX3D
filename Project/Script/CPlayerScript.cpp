#include "pch.h"
#include "CPlayerScript.h"
#include "CAttackScript.h"
#include "CMonsterAttackScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CRigidbody.h>
#include <Engine\CFSM.h>

#include <Engine\CIdleState.h>
#include <Engine\CWalkState.h>
#include <Engine\CRollState.h>
#include <Engine\CAttackState.h>
#include <Engine\CRunState.h>
#include <Engine\CJumpState.h>
#include <Engine\CJumpEnd.h>
#include <Engine\CPxRigidbody.h>
CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(100.f)
	, m_iActive(1)
	, m_pFSM(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iActive, "Player Active");
	
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_pFSM)
		delete m_pFSM;
}

void CPlayerScript::begin()
{
	//MeshRender()->GetDynamicMaterial(0);
}

void CPlayerScript::tick()
{
	if (m_iActive == 0)
		return;

	STATE_TYPE eType = m_pFSM->GetCurStateType();
	if (eType != STATE_TYPE::ATTACK &&
		eType != STATE_TYPE::ROLL)
	{
		rotate();
	}

	m_pFSM->final_tick();
}



void CPlayerScript::SetFSM(CFSM* _pFSM)
{
	m_pFSM = _pFSM;
	m_pFSM->SetOwner(GetOwner());
}

void CPlayerScript::Initialize()
{
	CGameObject* pArtorias = GetOwner();
	// ============
	// FBX Loading
	// ============	

	vector<Ptr<CMeshData>> vecMeshData = {};
	CGameObject* pObj = nullptr;

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\house.mdat");
	//pObj = pMeshData->Instantiate();
	//pObj->SetName(L"House");

	vector<Ptr<CMeshData>> pVecMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Artorias4.fbx");

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
			pObj->SetName(L"Artorias4" + strNum);
			pArtorias->AddChild(pObj);

			pObj->Animator3D()->CreateAnimationF(L"Idle", 0, 69);
			pObj->Animator3D()->CreateAnimationF(L"Walk_Front", 71, 130);
			pObj->Animator3D()->CreateAnimationF(L"Walk_Back", 133, 191);
			pObj->Animator3D()->CreateAnimationF(L"Walk_Left", 193, 252);
			pObj->Animator3D()->CreateAnimationF(L"Walk_Right", 254, 313);
			pObj->Animator3D()->CreateAnimationF(L"Run_Front", 315, 334);
			pObj->Animator3D()->CreateAnimationF(L"Roll_Front", 336, 376);
			pObj->Animator3D()->CreateAnimationF(L"Roll_Back", 377, 417);
			pObj->Animator3D()->CreateAnimationF(L"Roll_Left", 419, 458);
			pObj->Animator3D()->CreateAnimationF(L"Roll_Right", 460, 499);
			pObj->Animator3D()->CreateAnimationF(L"Attack0", 501, 599);
			pObj->Animator3D()->CreateAnimationF(L"Attack__1", 601, 690);
			pObj->Animator3D()->CreateAnimationF(L"Attack2", 693, 792);
			pObj->Animator3D()->CreateAnimationF(L"Attack1", 794, 879);
			//pObj->Animator3D()->CreateAnimationF(L"Attack4", 882, 981);
			pObj->Animator3D()->CreateAnimationF(L"Attack5", 982, 1097);

			pObj->Animator3D()->CreateAnimationF(L"Jump", 923, 953);
			pObj->Animator3D()->CreateAnimationF(L"Jump_Attack", 954, 981); //1135
			pObj->Animator3D()->CreateAnimationF(L"Jump_End", 1142, 1212);

			pObj->Animator3D()->CreateAnimationF(L"Attack6", 1214, 1287);
			pObj->Animator3D()->CreateAnimationF(L"Attack7", 1290, 1363);
			pObj->Animator3D()->CreateAnimationF(L"Dead", 1465, 1685);

			pObj->Animator3D()->Play(L"Idle", true);
			//pObj->Animator3D()->StartEvent() = std::bind(std::bind(&SkillLuck::create_luck, this)
		}
	}
	
	//SpawnGameObject(pAritorias, Vec3(0.f, 0.f, 100.f), L"Default");
	//Transform()->SetRelativeScale(2.f, 2.f, 2.f);
	Transform()->SetRelativeRot(-XM_PI / 2.f, 0.f, 0.f);

	m_pFSM = new CFSM();
	m_pFSM->SetOwner(pArtorias);

	CIdleState* pIdle = new CIdleState();
	pIdle->SetName(L"Idle");
	m_pFSM->AddState(STATE_TYPE::IDLE, pIdle);

	CWalkState* pWalk = new CWalkState();
	pWalk->SetName(L"Walk_");
	m_pFSM->AddState(STATE_TYPE::WALK, pWalk);

	CRunState* pRun = new CRunState();
	pRun->SetName(L"Run_");
	m_pFSM->AddState(STATE_TYPE::RUN, pRun);

	CRollState* pRoll = new CRollState;
	pRoll->SetName(L"Roll_");
	m_pFSM->AddState(STATE_TYPE::ROLL, pRoll);

	CJumpState* pJump = new CJumpState;
	pJump->SetName(L"Jump");
	m_pFSM->AddState(STATE_TYPE::JUMP, pJump);

	CJumpEnd* pJumpEnd = new CJumpEnd;
	pJumpEnd->SetName(L"Jump_End");
	m_pFSM->AddState(STATE_TYPE::JUMPEND , pJumpEnd);

	set_attack();

	m_pFSM->SetState(STATE_TYPE::IDLE);
	ChanageState(m_pFSM, STATE_TYPE::IDLE);
}


void CPlayerScript::set_attack()
{
	CAttackState* pAttack = new CAttackState;
	pAttack->SetName(L"Attack");
	m_pFSM->AddState(STATE_TYPE::ATTACK, pAttack);

	//Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);

	tAttackInfo attack0 = {};
	attack0.iAttackNum = 0;
	attack0.fForce = 100.f;
	attack0.fMoveTime = 0.6f;
	attack0.iStartFrame = 528;
	attack0.iEndFrame = 530;
	attack0.fOffsetPos = 60.f;
	attack0.vAttackScale = Vec3(150.f, 150.f, 150.f);
	attack0.tAttackValue.fAttRcnt = 5.f;

	CAttackScript* pAttackScript = new CAttackScript();
	pAttackScript->SetAttackValue(attack0.tAttackValue);
	CGameObject* pAttackObj = new CGameObject();
	pAttackObj->AddComponent(pAttackScript);
	m_pFSM->AddAttack(attack0, pAttackObj);

	tAttackInfo attack1 = {};
	attack1.iAttackNum = 1;
	attack1.fForce = 200.f;
	attack1.fMoveTime = 0.6f;
	attack1.iStartFrame = 831;
	attack1.iEndFrame = 835;
	attack1.fOffsetPos = 60.f;
	attack1.vAttackScale = Vec3(300.f, 300.f, 300.f);
	attack1.tAttackValue.fAttRcnt = 5.f;
	attack1.tAttackValue.bDown = true;

	pAttackScript = new CAttackScript();
	pAttackScript->SetAttackValue(attack1.tAttackValue);
	pAttackObj = new CGameObject();
	pAttackObj->AddComponent(pAttackScript);
	m_pFSM->AddAttack(attack1, pAttackObj);

	//tAttackInfo attack1 = {};
	//attack1.fForce = 10.f;
	//attack1.iAttackNum = 1;
	//pAttack->AddAttack(attack1);
	//
	//tAttackInfo attack2 = {};
	//attack2.iAttackNum = 2;
	//attack2.fForce = 10.f;
	//pAttack->AddAttack(attack2);
	//
	//tAttackInfo attack3 = {};
	//attack3.fForce = 10.f;
	//attack3.iAttackNum = 3;
	//pAttack->AddAttack(attack3);
	//
	//tAttackInfo attack4 = {};
	//attack4.fForce = 10.f;
	//attack4.iAttackNum = 4;
	//pAttack->AddAttack(attack4);
	//
	//tAttackInfo attack5 = {};
	//attack5.fForce = 10.f;
	//attack5.iAttackNum = 5;
	//pAttack->AddAttack(attack5);
	//
	//tAttackInfo attack6 = {};
	//attack6.fForce = 10.f;
	//attack6.iAttackNum = 6;
	//pAttack->AddAttack(attack6);
	//
	//tAttackInfo attack7 = {};
	//attack7.fForce = 10.f;
	//attack7.iAttackNum = 7;
	//pAttack->AddAttack(attack7);
}

void CPlayerScript::rotate()
{
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
	vRot.y += (DT / 2.f * vMouseDir.x);

	if (vRot.y != 0.f)
		int a = 10;

	Vec3 vNewRot = {};
	vNewRot.y = vRot.y;
	vNewRot.x = -XM_PI / 2.f;

	Transform()->SetRelativeRot(vNewRot);

	PxQuat xRotation(-XM_PI /2.f, PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat yRotation(vNewRot.y , PxVec3(0.0f, 1.0f, 0.0f));

	PxQuat newRotation = yRotation * xRotation;
	
	GetOwner()->PxRigidbody()->SetPxRotate(newRotation);
}

void CPlayerScript::BeginOverlap(CCollider3D* _Other)
{
	//플레이어가 먼저 들어옴
	//CMonsterAttackScript* pAttack = _Other->GetOwner()->GetScript<CMonsterAttackScript>();
	//if (pAttack)
	//{
	//	bool bOn = pAttack->IsAttackOn();
	//	if (bOn)
	//	{
	//		tAttack tAttack = pAttack->GetAttackValue();
	//		//CMonsterHit* pHit = dynamic_cast<CMonsterHit*>(m_pFSM->FindState(MONSTER_STATE_TYPE::HIT));
	//		//pHit->SetHitInfo(m_tHitInfo);
	//
	//		ChanageState(m_pFSM, STATE_TYPE::HIT);
	//	}
	//}
}

void CPlayerScript::OnOverlap(CCollider3D* _Other)
{

}

void CPlayerScript::EndOverlap(CCollider3D* _Other)
{

}



void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}