#include "pch.h"
#include "CAttackState.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CFSM.h"
#include "CRigidbody.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CTransform.h"
#include "CCollider3D.h"
#include "CPxRigidbody.h"


#define JUMP_ATTACK 10

CAttackState::CAttackState() :
	m_strAttackNum(L"0"),
	m_vecAttack{},
	m_iCurAttack(0),
	m_iAttackCount(0),
	m_fCurMoveTime(0.f),
	m_tCurAttack{},
	m_vecAttackObj{}
{

}

CAttackState::~CAttackState()
{
	for (int i = 0; i < m_vecAttackObj.size(); ++i)
	{
		std::queue<CGameObject*> queue = m_vecAttackObj[i];
		while (!queue.empty())
		{
			CGameObject* pObj = queue.front();
			delete pObj;
			pObj = nullptr;
			queue.pop();
		}
	}

	if (!m_pCurGameObj)
	{
		delete m_pCurGameObj;
		m_pCurGameObj = nullptr;
	}
}

void CAttackState::final_tick()
{
	CGameObject* pChildObj = GetOwner()->GetChild().at(0);
	bool bComplete = pChildObj->Animator3D()->GetCurAnim()->IsComplete();

	if (bComplete)
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
	}
	
	add_force();

	check_event();
}

void CAttackState::Exit()
{
	//GetOwner()->Rigidbody()->SetAcumulate(false);
	if (m_pCurGameObj)
	{
		erase_attack();
	}

	m_fCurMoveTime = 0.f;
	m_iAttackCount = 0;
}

void CAttackState::Enter()
{
	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, 1);
	int iNum = (int)num(en);
	m_tCurAttack = m_vecAttack[iNum];
	m_strAttackNum = std::to_wstring(iNum);
	wstring strFinalAnim = GetName() + m_strAttackNum;

	//GetOwner()->PxRigidbody()->SetAcumulate(true);
	Chanage_Anim(strFinalAnim, false);
}

void CAttackState::add_force()
{
	m_fCurMoveTime += DT;

	if (m_fCurMoveTime >= m_tCurAttack.fMoveTime)
		return;

	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	vFront.y = 0.f;
	GetOwner()->PxRigidbody()->AddVelocity(vFront * -m_tCurAttack.fForce);
}


void CAttackState::check_event()
{
	CGameObject* pChildObj = GetOwner()->GetChild().at(0);
	int iCurFrame = pChildObj->Animator3D()->GetCurAnim()->GetCurFrame();

	if (m_iAttackCount == 0 && m_tCurAttack.iStartFrame <= iCurFrame)
	{
		m_iAttackCount = 1;

		spawn_attack();
	}

	else if (m_iAttackCount == 1 && m_tCurAttack.iEndFrame <= iCurFrame)
	{
		m_iAttackCount = 2;

		erase_attack();
	}
}

void CAttackState::add_objpull(UINT _iAttackNum, CGameObject* _pAttackObj)
{
	m_vecAttackObj[_iAttackNum].push(_pAttackObj);
}

void CAttackState::AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj)
{
	if (m_vecAttack.size() <= _tAttackInfo.iAttackNum)
	{
		m_vecAttack.resize(_tAttackInfo.iAttackNum + 1);
		m_vecAttackObj.resize(_tAttackInfo.iAttackNum + 1);
	}

	m_vecAttack[_tAttackInfo.iAttackNum] = _tAttackInfo;
	m_vecAttackObj[_tAttackInfo.iAttackNum].push(_pAttackObj);

	_pAttackObj->AddComponent(new CTransform);
	CCollider3D* pCollider = new CCollider3D();
	pCollider->SetOffsetScale(_tAttackInfo.vAttackScale);
	_pAttackObj->AddComponent(pCollider);

	//////////
	CPxRigidbody* pRigid = new CPxRigidbody();
	_pAttackObj->AddComponent(pRigid);

	pRigid->init(Vec3(-2000.f,-2000.f,-2000.f), _tAttackInfo.vAttackScale, (int)LAYER_TYPE::Attack, _pAttackObj);
	pRigid->SetGround(true,false); //땅상태 , 중력 안받음 
	pRigid->SetPass(true); // 충돌해도 통과되게
}

void CAttackState::erase_attack()
{
	EraseObject(m_pCurGameObj, (int)LAYER_TYPE::Attack);
	//잠시만 테스트용 코드 -> 피직스 물체는 사라지지 않음 계속 충돌이 발생함
	m_pCurGameObj->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));

	add_objpull(m_tCurAttack.iAttackNum, m_pCurGameObj);
	m_pCurGameObj = nullptr;
}

void CAttackState::spawn_attack()
{
	m_pCurGameObj = m_vecAttackObj[m_tCurAttack.iAttackNum].front();
	m_vecAttackObj[m_tCurAttack.iAttackNum].pop();

	//중심은 물체의 충돌체 위치를 중심으로 잡음 
	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
	Vec3 vDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vOffsetPos = -vDir * m_tCurAttack.fOffsetPos;
	vPos += vOffsetPos;

	//rotate는 collider에서 처리를 하지않음
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	vRot.y += m_tCurAttack.fRotate;
	PxQuat yRotation(vRot.y, PxVec3(0.0f, 1.0f, 0.0f));

	//여기에 충돌체 위치랑 회전 rigidbody에 넣기
	m_pCurGameObj->PxRigidbody()->SetPxRotate(yRotation);
	m_pCurGameObj->PxRigidbody()->SetPxTransform(vPos);

	//spawn
	SpawnGameObject(m_pCurGameObj, vPos, (int)LAYER_TYPE::Attack);
}


