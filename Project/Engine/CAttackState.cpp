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
		return;
	}
	
	add_force();

	check_event();
}

void CAttackState::Exit()
{
	//GetOwner()->Rigidbody()->SetAcumulate(false);
	m_fCurMoveTime = 0.f;
	m_iAttackCount = 0;
}

void CAttackState::Enter()
{
	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, 7);
	int iNum = (int)num(en);
	iNum = 0;
	m_tCurAttack = m_vecAttack[iNum];
	m_strAttackNum = std::to_wstring(iNum);

	//임시
	if (iNum == 4)
		m_strAttackNum = L"0";

	GetOwner()->Rigidbody()->SetAcumulate(true);

	wstring strFinalAnim = GetName() + m_strAttackNum;
	Chanage_Anim(strFinalAnim, false);
}

void CAttackState::add_force()
{
	m_fCurMoveTime += DT;

	if (m_fCurMoveTime >= m_tCurAttack.fMoveTime)
	{
		GetOwner()->Rigidbody()->SetAcumulate(false);
	}
	else
	{
		Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		vFront.y = 0.f;
		GetOwner()->Rigidbody()->AddForce(vFront * -m_tCurAttack.fForce);
	}
}


void CAttackState::check_event()
{
	CGameObject* pChildObj = GetOwner()->GetChild().at(0);
	int iCurFrame = pChildObj->Animator3D()->GetCurAnim()->GetCurFrame();

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

		//rotate는 collider에서 처리를 하지않음
		Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
		vRot.y += m_tCurAttack.fRotate;
		m_pCurGameObj->Transform()->SetRelativeRot(vRot);
		//spawn
		SpawnGameObject(m_pCurGameObj, vPos, (int)LAYER_TYPE::Attack);
	}

	else if (m_iAttackCount == 1 && m_tCurAttack.iEndFrame <= iCurFrame)
	{
		m_iAttackCount = 2;

		EraseObject(m_pCurGameObj, (int)LAYER_TYPE::Attack);

		add_objpull(m_iCurAttack, m_pCurGameObj);
		m_pCurGameObj = nullptr;
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
}


