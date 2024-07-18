#include "pch.h"
#include "CPlayerScript.h"
#include "CAttackScript.h"
#include "CJumpAttackScript.h"
#include "CMonsterAttackScript.h"
#include "CMonsterScript.h"

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
#include <Engine\CJumpingState.h>
#include <Engine\CPxRigidbody.h>
#include <Engine\CHitState.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CStructuredBuffer.h>

#include <Engine\CGage.h>
CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(100.f)
	, m_iActive(1)
	, m_pFSM(nullptr)
	, m_iBone(125)
	, m_vOffsetTransform(Vec3(0.f, 500.f, 520.f))
	, m_vecWeapon{}
	, m_iActiveWeaponIdx(0)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
	AddScriptParam(SCRIPT_PARAM::INT,  &m_iActive, "Player Active");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iBone, "Player Bone");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vOffsetTransform, "Player OffsetPos");
	// 4 16	61 26
	// 0 500 520
	// 26
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_pFSM)
		delete m_pFSM;


	for (int i = 0; i < m_vecWeapon.size(); ++i)
	{
		if (m_iActiveWeaponIdx == i)
			continue;

		delete m_vecWeapon[i];
		m_vecWeapon[i] = nullptr;
		
	}
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
	if (eType == STATE_TYPE::IDLE ||
		eType == STATE_TYPE::WALK)
	{
		rotate();
	}
	m_pFSM->final_tick();

	tick_gage();
	
	change_equip();
	
	//m_vecWeapon[m_iActiveWeaponIdx]->Equip()->SetIndex(m_iBone);

	//test용
	// GetOwner()->Equip()->SetIndex(m_iBone);
	//CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Monster);
	//CGameObject* _pMonster = pLayer->GetParentObject().at(1)->GetChild().at(0);
	//CAnimator3D* pAnim = _pMonster->Animator3D();
	//CStructuredBuffer* pBoneBuffer = pAnim->GetFinalBoneMat();
	//
	//vector<Matrix> vecBone = {};
	//vecBone.resize(pAnim->GetBoneCount());
	//pBoneBuffer->GetData(vecBone.data());
	//
	//Matrix m_matFinalBone = {};
	//m_matFinalBone = vecBone[m_iBone];
	//m_matFinalBone.m[3][3] = 1;
	//m_matFinalBone = XMMatrixTranspose(m_matFinalBone);
	//
	//Matrix matMonWorldMat = _pMonster->Transform()->GetWorldMat(); //월드 행렬
	//Matrix matMonScaleMat = _pMonster->Transform()->GetWorldScaleMat();
	//Matrix matMonScaleInv = XMMatrixInverse(nullptr, matMonScaleMat);//크기 역행렬
	//
	//Matrix matFinalPos = matMonScaleInv * m_matFinalBone * matMonWorldMat;
	//
	//Vec3 vPos = matFinalPos.Translation();
	//	
	//vPos += m_vOffsetTransform;
	//GetOwner()->PxRigidbody()->SetPxTransform(vPos);
}



void CPlayerScript::SetFSM(CFSM* _pFSM)
{
	m_pFSM = _pFSM;
	m_pFSM->SetOwner(GetOwner());
}

void CPlayerScript::Initialize()
{
	m_tPlayerInfo.fMaxHP = 1000.f;
	m_tPlayerInfo.fHP = 1000.f;

	CGameObject* pArtorias = GetOwner();
	// ============
	// FBX Loading
	// ============	

	vector<Ptr<CMeshData>> vecMeshData = {};
	CGameObject* pObj = nullptr;

	//player
	vector<Ptr<CMeshData>> pVecMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Artorias4.fbx");
	//마지막 인덱스는 기본 검
	for (int i = 0; i < pVecMeshData.size()-1; ++i)
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

			GetOwner()->AddChild(pObj);

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

			pObj->Animator3D()->CreateAnimationF(L"Jump", 881, 953);
			pObj->Animator3D()->CreateAnimationF(L"Jumping", 953, 953);
			pObj->Animator3D()->CreateAnimationF(L"Jump_Attack", 954, 954);

			pObj->Animator3D()->CreateAnimationF(L"Jump_Attack_End", 954, 981); //1135
			pObj->Animator3D()->CreateAnimationF(L"Jump_End", 1142, 1212);

			pObj->Animator3D()->CreateAnimationF(L"Attack6", 1214, 1287);
			pObj->Animator3D()->CreateAnimationF(L"Attack7", 1290, 1363);

			pObj->Animator3D()->CreateAnimationF(L"Hit", 1465, 1483);
			pObj->Animator3D()->CreateAnimationF(L"Hit_", 1475, 1483, true);//반대로

			pObj->Animator3D()->CreateAnimationF(L"Dead", 1465, 1685);

			pObj->Animator3D()->Play(L"Idle", true);
			//pObj->Animator3D()->StartEvent() = std::bind(std::bind(&SkillLuck::create_luck, this)
		}
	}

	//weapon
	//기본 무기 먼저 push

	float fRadian = XM_PI / 180.f;

	CGameObject* pMainWeapon = pVecMeshData[pVecMeshData.size() - 1]->Instantiate();
	pMainWeapon->SetName(L"PlayerWeapon0");
	pMainWeapon->AddComponent(new CEquip());
	pMainWeapon->Equip()->SetActive(true);
	pMainWeapon->Equip()->SetChar(GetOwner());
	pMainWeapon->Equip()->SetIndex(m_iBone);
	SpawnGameObject(pMainWeapon, Vec3(125, -28.f, 252.f), (int)LAYER_TYPE::Default);
	pMainWeapon->Transform()->SetRelativeRot(-140 * fRadian, 164 * fRadian, 173 * fRadian);
	pMainWeapon->Equip()->SetFixedPos(Vec3(73, 88, 117));
	m_vecWeapon.push_back(pMainWeapon);

	CGameObject* pWeapon = InitializeFBX(L"TaurusDemon_Axe");
	pWeapon->AddComponent(new CTransform());
	pWeapon->SetName(L"PlayerWeapon1");
	pWeapon->AddComponent(new CEquip());
	pWeapon->Equip()->SetChar(GetOwner());
	pWeapon->Equip()->SetIndex(m_iBone);
	pWeapon->Transform()->SetRelativeRot(-10 * fRadian, -72 * fRadian, -71 * fRadian);
	pWeapon->Equip()->SetFixedPos(Vec3(153, 1.f, 252.f));

	m_vecWeapon.push_back(pWeapon);
	//SpawnGameObject(pWeapon, Vec3(153, 1.f, 252.f), (int)LAYER_TYPE::Default);


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

	CJumpingState* pJumping = new CJumpingState;
	pJumping->SetName(L"Jump_Ing");
	m_pFSM->AddState(STATE_TYPE::JUMPING, pJumping);

	CHitState* pHit = new CHitState();
	pHit->SetName(L"Hit");
	m_pFSM->AddState(STATE_TYPE::HIT, pHit);

	set_attack();

	m_pFSM->SetState(STATE_TYPE::IDLE);
	ChanageState(m_pFSM, STATE_TYPE::IDLE);

	set_ui();
}


void CPlayerScript::set_ui()
{
	m_pHP = new CGage();
	m_pHP->Initialize(L"texture\\GameTexture\\PlayerHP.png", Vec3(800.f, 24.f, 0.f), L"PlayerHP");
	SpawnGameObject(m_pHP, Vec3(-85.f, 283.f, -2.f), (int)LAYER_TYPE::UI);

	//-85
	//m_pHeal = new CGage();
	//m_pHeal->Initialize(L"texture\\GameTexture\\PlayerHeal.png", Vec3(800.f, 18.f, 0.f), L"PlayerHeal");
	//SpawnGameObject(m_pHeal, Vec3(-85.f, 260.f, -2.f), (int)LAYER_TYPE::UI);
}

void CPlayerScript::set_attack()
{
	CAttackState* pAttack = new CAttackState;
	pAttack->SetName(L"Attack");
	m_pFSM->AddState(STATE_TYPE::ATTACK, pAttack);

	//Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);

	tAttackInfo attack0 = {};
	attack0.iAttackNum = 0;
	attack0.vForce = Vec3::Zero;
	attack0.fForce = 200.f;
	attack0.fMoveTime = 0.f;
	attack0.iStartFrame = 528;
	attack0.iEndFrame = 530;
	attack0.fOffsetPos = 60.f;
	attack0.vAttackScale = Vec3(150.f, 150.f, 150.f);
	attack0.tAttackValue.iMaxCount = 2;
	attack0.tAttackValue.fAttackTime = 0.5f;
	attack0.tAttackValue.fAttRcnt = 120.f;
	attack0.tAttackValue.fDamage = 100.f;
	attack0.tAttackValue.fAddForceTime = 0.4f;

	CAttackScript* pAttackScript = new CAttackScript();
	pAttackScript->SetAttackValue(attack0.tAttackValue);
	CGameObject* pAttackObj = new CGameObject();
	pAttackObj->AddComponent(pAttackScript);
	m_pFSM->AddAttack(attack0, pAttackObj);

	tAttackInfo attack1 = {};
	attack1.iAttackNum = 1;
	attack1.vForce = Vec3::Zero;
	attack1.fForce = 400.f;
	attack1.fMoveTime = 0.6f;
	attack1.iStartFrame = 832;
	attack1.iEndFrame = 835;
	attack1.fOffsetPos = 60.f;
	attack1.vAttackScale = Vec3(300.f, 300.f, 300.f);
	attack1.tAttackValue.iMaxCount = 5;
	attack1.tAttackValue.fAttRcnt = 200.f;
	attack1.tAttackValue.fAttackTime = 0.5f;
	attack1.tAttackValue.fDamage = 200.f;
	attack0.tAttackValue.fAddForceTime = 0.55f;
	attack1.tAttackValue.bDown = true;

	pAttackScript = new CAttackScript();
	pAttackScript->SetAttackValue(attack1.tAttackValue);
	pAttackObj = new CGameObject();
	pAttackObj->AddComponent(pAttackScript);
	m_pFSM->AddAttack(attack1, pAttackObj);

	tAttackInfo attackjump = {};
	attackjump.iStartFrame = 954;
	attackjump.iEndFrame = 960;
	attackjump.vAttackScale = Vec3(200.f, 250.f, 200.f);
	attackjump.tAttackValue.iMaxCount = 1;
	attackjump.tAttackValue.fAttackTime = 0.f;
	attackjump.tAttackValue.fDamage = 200.f;
	attackjump.tAttackValue.bDown = true;
	
	CJumpAttackScript* pJumpAttack = new CJumpAttackScript();
	pJumpAttack->SetAttackValue(attackjump.tAttackValue);
	pAttackObj = new CGameObject();

	CCollider3D* pCollider = new CCollider3D();
	pCollider->SetOffsetScale(attackjump.vAttackScale);
	pAttackObj->AddComponent(pCollider);
	pAttackObj->AddComponent(new CTransform);
	pAttackObj->AddComponent(pJumpAttack);

	CPxRigidbody* pRigid = new CPxRigidbody();
	pAttackObj->AddComponent(pRigid);
	pRigid->init(Vec3(-2000.f, -2000.f, -2000.f), attackjump.vAttackScale, (int)LAYER_TYPE::Attack, pAttackObj);
	pRigid->SetGround(true, false); //땅상태 , 중력 안받음 
	pRigid->SetPass(true); // 충돌해도 통과되게

	CJumpingState* pJumpingState = m_pFSM->GetState<CJumpingState>();
	pJumpingState->SetAttackObj(pAttackObj);
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

void CPlayerScript::change_equip()
{
	if (KEY_TAP(KEY::ESC))
	{
		int iTemIdx = m_iActiveWeaponIdx; //다음 무기로
		if (++iTemIdx > m_vecWeapon.size() - 1)
			iTemIdx = 0; //다시 처음 무기로

		//이전 무기 해제
		m_vecWeapon[m_iActiveWeaponIdx]->Equip()->SetActive(false);
		EraseObject(m_vecWeapon[m_iActiveWeaponIdx], (int)LAYER_TYPE::Default);

		//새로운 무기 할당
		m_iActiveWeaponIdx = iTemIdx;
		
		m_vecWeapon[m_iActiveWeaponIdx]->Equip()->SetActive(true);
		SpawnGameObject(m_vecWeapon[m_iActiveWeaponIdx], Vec3::Zero, (int)LAYER_TYPE::Default);
	}
}

void CPlayerScript::Chanage_AnimDT(float _fDivDT)
{
	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
		vecChild[i]->Animator3D()->SetAnimDivDT(_fDivDT);
	}
}

void CPlayerScript::tick_gage()
{
	m_pHP->UpdateGage(m_tPlayerInfo.fMaxHP, m_tPlayerInfo.fHP);
}

void CPlayerScript::BeginOverlap(CCollider3D* _Other)
{
	//플레이어가 먼저 들어옴
	//CMonsterAttackScript* pAttack = _Other->GetOwner()->GetScript<CMonsterAttackScript>();
	//if (pAttack)
	//{
	//	tAttack tAttack = pAttack->GetAttackValue();
	//	CPlayerHit* pHit = dynamic_cast<CPlayerHit*>(m_pFSM->FindState(STATE_TYPE::HIT));
	//	//pHit->SetHitInfo(m_tHitInfo);
	//	
	//	ChanageState(m_pFSM, STATE_TYPE::HIT);
	//}
}

void CPlayerScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();
	CMonsterAttackScript* pAttack = pObj->GetScript<CMonsterAttackScript>();

	if (pAttack)
	{
		if (m_pFSM->GetCurStateType() == STATE_TYPE::HIT)
			return;

		tAttack tAttack = pAttack->GetAttackValue();
		CHitState* pHit = dynamic_cast<CHitState*>(m_pFSM->FindState(STATE_TYPE::HIT));
		pHit->SetHitInfo(m_tHitInfo);

		m_tPlayerInfo.fHP -= m_tHitInfo.fDamage;

		ChanageState(m_pFSM, STATE_TYPE::HIT);
	}
}

void CPlayerScript::EndOverlap(CCollider3D* _Other)
{
	int a = 10;
}



void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}