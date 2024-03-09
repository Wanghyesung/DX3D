#include "pch.h"
#include "CState.h"
#include "CFSM.h"
#include "CGameObject.h"
#include "CAnimator3D.h"



void CState::Chanage_Anim(const wstring& _strName, bool _bRepeat)
{
	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
		vecChild[i]->Animator3D()->Play(_strName, _bRepeat);
	}
}

void CState::Stop_Anim(bool _bStop)
{
	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
		vecChild[i]->Animator3D()->Stop(_bStop);
	}
}

CGameObject* CState::GetOwner()
{
	return m_pFSM->GetOwner();
}

CState::CState() :
	m_pFSM(nullptr),
	m_eType(STATE_TYPE::END)
{
}

CState::~CState()
{
}
