#include "pch.h"
#include "CMonsterState.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CMonsterFSM.h"

CMonsterState::CMonsterState() :
	m_eType(MONSTER_STATE_TYPE::END),
	m_pFSM(nullptr)
{

}

CMonsterState::~CMonsterState()
{

}

void CMonsterState::Chanage_Anim(const wstring& _strName, bool _bRepeat)
{
	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		//자식 오브젝트에 애니메이션에서 지금 내 상태에 맞는 애니메이션 실행
		vecChild[i]->Animator3D()->Play(_strName, _bRepeat);
	}
}

CGameObject* CMonsterState::GetOwner()
{
	return m_pFSM->GetOwner();
}



