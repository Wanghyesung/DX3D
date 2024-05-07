#include "pch.h"
#include "CDemonJumpAttack.h"
#include "CGameObject.h"
#include "CPxRigidbody.h"
#include "CAnimation3D.h"
CDemonJumpAttack::CDemonJumpAttack() :
	m_pJumpAttackObj(nullptr),
	m_bFall(false),
	m_iAttackFrame(1761),
	m_iAttackEndFrame(1768),
	m_iFallFrame(1825),
	m_fAttackRange(1000.f),
	m_vAttackOffset(Vec3(0.f,100.f,-1000.f))
{

}

CDemonJumpAttack::~CDemonJumpAttack()
{
	if (m_pJumpAttackObj)
		delete m_pJumpAttackObj;
}

void CDemonJumpAttack::SetAttackObj(CGameObject* _pJumpAttack)
{
	if (m_pJumpAttackObj)
		return;

	m_pJumpAttackObj = _pJumpAttack;

	//이벤트 걸어두기
	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Jump_Attack", std::bind(&CDemonJumpAttack::attack, this), m_iAttackFrame);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Jump_Attack", std::bind(&CDemonJumpAttack::EraseAttack, this), m_iAttackEndFrame);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Jump_Attack", std::bind(&CDemonJumpAttack::set_fall, this), m_iFallFrame);
}

void CDemonJumpAttack::attack()
{
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
	vPos += m_vAttackOffset;
	m_pJumpAttackObj->PxRigidbody()->SetPxTransform(vPos);
}

void CDemonJumpAttack::fall()
{
	GetOwner()->PxRigidbody()->SetVelocity(Vec3(0.f, -2200.f, 0.f));
	
}

void CDemonJumpAttack::set_fall()
{
	m_bFall = true;
	//GetOwner()->PxRigidbody()->SetAddGravity(true);
	//GetOwner()->PxRigidbody()->SetGround(false, true);
	
}

void CDemonJumpAttack::final_tick()
{
	CAnimation3D* pAnim = GetOwner()->GetChild().at(0)->Animator3D()->GetCurAnim();

	int iFrame = pAnim->GetCurFrame();
	bool bComplete = pAnim->IsComplete();

	if (bComplete)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
		return;
	}

	if (m_bFall)
	{
		fall();
	}
}

void CDemonJumpAttack::Exit()
{
	//const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	//for (int i = 0; i < vecChild.size(); ++i)
	//{
	//	//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
	//	vecChild[i]->Animator3D()->SetAnimDivDT(1);
	//}
}

void CDemonJumpAttack::Enter()
{
	Chanage_Anim(GetName(),false);

	m_bFall = false;
	SpawnGameObject(m_pJumpAttackObj, Vec3::Zero, (int)LAYER_TYPE::MonsterAttack);
}

void CDemonJumpAttack::EraseAttack()
{
	EraseObject(m_pJumpAttackObj, (int)LAYER_TYPE::MonsterAttack);
	m_pJumpAttackObj->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));
}

