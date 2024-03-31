#include "pch.h"
#include "CMonsterAttack.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CMonsterFSM.h"
#include "CCollider3D.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CRigidbody.h"
#include "CPxRigidbody.h"
CMonsterAttack::CMonsterAttack()
{

}

CMonsterAttack::~CMonsterAttack()
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

void CMonsterAttack::final_tick()
{
	CGameObject* pObj = GetOwner()->GetChild().at(0);

	CAnimation3D* pAnim = pObj->Animator3D()->GetCurAnim();
	
	if (pAnim->IsComplete())
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
	}

	add_force();

	check_event();
}

void CMonsterAttack::Exit()
{
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION);
	m_fCurMoveTime = 0.f;
	m_iAttackCount = 0;
}

void CMonsterAttack::Enter()
{
	//랜덤 공격
	int iMaxAttack = m_vecAttack.size()-1;

	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, iMaxAttack);
	UINT iAttackNum = (UINT)num(en);

	m_tCurAttack = m_vecAttack[iAttackNum];

	wstring strName = GetName() + std::to_wstring(iAttackNum);
	Chanage_Anim(strName, false);

	//GetOwner()->PxRigidbody()->SetAcumulate(true);
}

void CMonsterAttack::AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj)
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

	CPxRigidbody* pRigid = new CPxRigidbody();
	_pAttackObj->AddComponent(pRigid);
	pRigid->init(Vec3(-2000.f, -2000.f, -2000.f), _tAttackInfo.vAttackScale, (int)LAYER_TYPE::MonsterAttack, _pAttackObj);
	pRigid->SetGround(true, false); //땅상태 , 중력 안받음 
	pRigid->SetPass(true); // 충돌해도 통과되게

}

void CMonsterAttack::add_force()
{
	m_fCurMoveTime += DT;

	if (m_fCurMoveTime >= m_tCurAttack.fMoveTime)
		return;
	else
	{
		Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		vFront.y = 0.f;
		GetOwner()->PxRigidbody()->AddVelocity(vFront * m_tCurAttack.fForce);
	}
}

void CMonsterAttack::check_event()
{
	CGameObject* pChildObj = GetOwner()->GetChild().at(0);
	int iCurFrame = pChildObj->Animator3D()->GetCurAnim()->GetCurFrame();

	if (m_vecAttackObj.size() == 0)
		return;

	if (m_iAttackCount == 0 && m_tCurAttack.iStartFrame <= iCurFrame)
	{
		m_iAttackCount = 1;

		m_pCurGameObj = m_vecAttackObj[m_tCurAttack.iAttackNum].front();
		m_vecAttackObj[m_tCurAttack.iAttackNum].pop();

		//중심은 물체의 충돌체 위치를 중심으로 잡음 
		Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
		Vec3 vDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		Vec3 vOffsetPos = -vDir * m_tCurAttack.fOffsetPos;
		vPos += vOffsetPos;

		Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
		vRot.y += m_tCurAttack.fRotate;
		PxQuat yRotation(vRot.y, PxVec3(0.0f, 1.0f, 0.0f));

		//여기에 충돌체 위치랑 회전 rigidbody에 넣기
		m_pCurGameObj->PxRigidbody()->SetPxRotate(yRotation);
		m_pCurGameObj->PxRigidbody()->SetPxTransform(vPos);

		SpawnGameObject(m_pCurGameObj, vPos, (int)LAYER_TYPE::MonsterAttack);
	}

	else if (m_iAttackCount == 1 && m_tCurAttack.iEndFrame <= iCurFrame)
	{
		m_iAttackCount = 2;

		EraseObject(m_pCurGameObj, (int)LAYER_TYPE::MonsterAttack);
		m_pCurGameObj->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));

		add_objpull(m_tCurAttack.iAttackNum, m_pCurGameObj);
		m_pCurGameObj = nullptr;
	}
}

void CMonsterAttack::add_objpull(UINT _iAttackNum, CGameObject* _pAttackObj)
{
	m_vecAttackObj[_iAttackNum].push(_pAttackObj);
}


