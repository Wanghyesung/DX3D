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
#include "CLevelMgr.h"
#include "CLayer.h"
CMonsterAttack::CMonsterAttack():
	m_pTarget(nullptr)
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

	//check_event();
}

void CMonsterAttack::Exit()
{
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION);

	//�ӽ�
	if (m_pCurGameObj)
	{
		EraseObject(m_pCurGameObj, (int)LAYER_TYPE::MonsterAttack);
		m_pCurGameObj->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));

		add_objpull(m_tCurAttack.iAttackNum, m_pCurGameObj);
		m_pCurGameObj = nullptr;
	}
}

void CMonsterAttack::Enter()
{
	m_fCurMoveTime = 0.f;
	m_iAttackCount = 0;

	//���� ����
	int iMaxAttack = m_vecAttack.size() - 1;

	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, iMaxAttack);
	//m_iCurAttack = 3;// (UINT)num(en);
	m_iCurAttack = (UINT)num(en);

	m_tCurAttack = m_vecAttack[m_iCurAttack];

	wstring strName = GetName() + std::to_wstring(m_iCurAttack);
	Chanage_Anim(strName, false);


	//�÷��̾� �������� �ٶ󺸱�
	const vector<CGameObject*>& vecObj =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();
	if (vecObj[0] != nullptr)
		m_pTarget = vecObj[0];

	rotate();
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
	pRigid->SetGround(true, false); //������ , �߷� �ȹ��� 
	pRigid->SetPass(true); // �浹�ص� ����ǰ�

	//�Լ� �̺�Ʈ �ִϸ��̼ǿ� ���
	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack" + std::to_wstring(_tAttackInfo.iAttackNum), 
			std::bind(&CMonsterAttack::spawn_attack, this), _tAttackInfo.iStartFrame);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack" + std::to_wstring(_tAttackInfo.iAttackNum),
			std::bind(&CMonsterAttack::erase_attack, this), _tAttackInfo.iEndFrame);
}

void CMonsterAttack::add_force()
{

	//if (m_fCurMoveTime > m_tCurAttack.fMoveTime)
	//	return;
	//else
	//{
	m_tCurAttack = m_vecAttack[m_iCurAttack];

	if (m_tCurAttack.vForce != Vec3::Zero)
	{
		m_fCurMoveTime += DT;

		float fRatio = (m_tCurAttack.fMoveTime - m_fCurMoveTime);

		m_tCurAttack.vForce = m_tCurAttack.vForce * fRatio;

		GetOwner()->PxRigidbody()->SetVelocity(m_tCurAttack.vForce);
	}
	
		//�ӽ�
		
		
		//GetOwner()->PxRigidbody()->AddVelocity(Vec3(0.f,1000.f,0.f));�̰� ���µ�
		 
		//else
		//Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		//vFront.y = 0.f;
		//Vec3 vFrontForce = -vFront * m_tCurAttack.fForce;
		//GetOwner()->PxRigidbody()->AddVelocity(vFrontForce);
		
	//}
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

		//�߽��� ��ü�� �浹ü ��ġ�� �߽����� ���� 
		Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
		Vec3 vDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		Vec3 vOffsetPos = -vDir * m_tCurAttack.fOffsetPos;
		vPos += vOffsetPos;

		Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
		vRot.y += m_tCurAttack.fRotate;
		PxQuat yRotation(vRot.y, PxVec3(0.0f, 1.0f, 0.0f));

		//���⿡ �浹ü ��ġ�� ȸ�� rigidbody�� �ֱ�
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

void CMonsterAttack::spawn_attack()
{
	if (m_iAttackCount == 0)
	{
		m_iAttackCount = 1;

		m_pCurGameObj = m_vecAttackObj[m_tCurAttack.iAttackNum].front();
		m_vecAttackObj[m_tCurAttack.iAttackNum].pop();

		//�߽��� ��ü�� �浹ü ��ġ�� �߽����� ���� 
		Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
		Vec3 vDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		Vec3 vOffsetPos = -vDir * m_tCurAttack.fOffsetPos;
		vPos += vOffsetPos;

		Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
		vRot.y += m_tCurAttack.fRotate;
		PxQuat yRotation(vRot.y, PxVec3(0.0f, 1.0f, 0.0f));

		//���⿡ �浹ü ��ġ�� ȸ�� rigidbody�� �ֱ�
		m_pCurGameObj->PxRigidbody()->SetPxRotate(yRotation);
		m_pCurGameObj->PxRigidbody()->SetPxTransform(vPos);

		SpawnGameObject(m_pCurGameObj, vPos, (int)LAYER_TYPE::MonsterAttack);
	}
}

void CMonsterAttack::erase_attack()
{
	if (m_iAttackCount == 1)
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

void CMonsterAttack::rotate()
{
	//z <--> y fbx�� 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;

	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	Vec3 vDir = (vTargetPos - vPos).Normalize();

	//- = 90 ~ 270�� - �� ������ ������ �а��� �̷��.
	float fRadian;
	float fCos = vDir.Dot(vFoward);
	//���� �̿� ������ ���� �Ǻ�
	Vec3 vCross = vFoward.Cross(vDir);

	if (vCross.y >= 0)
	{
		fRadian = XM_PI + acos(fCos);
	}
	else
	{
		fRadian = XM_PI - acos(fCos);
	}

	//
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);

	PxQuat yRotation(vFinalRot.y, PxVec3(0.0f, 1.0f, 0.0f));
	GetOwner()->PxRigidbody()->SetPxRotate(yRotation);
	GetOwner()->Transform()->SetRelativeRot(vFinalRot);
}


