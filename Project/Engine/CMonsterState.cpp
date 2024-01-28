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
		//�ڽ� ������Ʈ�� �ִϸ��̼ǿ��� ���� �� ���¿� �´� �ִϸ��̼� ����
		vecChild[i]->Animator3D()->Play(_strName, _bRepeat);
	}
}

CGameObject* CMonsterState::GetOwner()
{
	return m_pFSM->GetOwner();
}



