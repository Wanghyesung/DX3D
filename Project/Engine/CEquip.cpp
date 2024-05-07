#include "pch.h"
#include "CEquip.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CMeshRender.h"
#include "CTransform.h"
CEquip::CEquip() :
	CComponent(COMPONENT_TYPE::EQUIP),
	m_iIndex(38),
	m_matFinalBone{},
	m_pChar(nullptr)
{
	
}

CEquip::~CEquip()
{
}


void CEquip::begin()
{
	
	
}

void CEquip::finaltick()
{	
	if (!m_pChar)
		return;

	if (!check_matrix())
		return;

	//m_pTarget->Transform()->SetAbsolute(true);

	Matrix matWeaponWorld = Transform()->GetWorldMat();//무기

	Matrix matCharWorldScale = m_pChar->Transform()->GetWorldScaleMat();//캐릭터 크기 역행렬
	Matrix matCharSclaeInv = XMMatrixInverse(nullptr, matCharWorldScale);
	Matrix matCharWorld = m_pChar->Transform()->GetWorldMat(); //캐릭터 월드
	Matrix matWeapon = matCharSclaeInv * matWeaponWorld * m_matFinalBone * matCharWorld;

	const vector<CGameObject*> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->Transform()->SetDependent(true);//final_tick에서 월드행렬 따로 만들지 않게
		vecChild[i]->Transform()->SetWorldMat(matWeapon);
	}
}

bool CEquip::check_matrix()
{
	if (m_pChar == nullptr)
		return false;

	else if (m_pChar->IsDead())
	{
		m_pChar = nullptr;
		return false;
	}

	CAnimator3D* pAnim = m_pChar->GetChild().at(0)->Animator3D();

	bool bOn = pAnim->IsFinalMatUpdate();
	if (bOn)
	{
		UINT iBoneCount = pAnim->GetBoneCount();
		m_vecFinalBone.resize(iBoneCount);

		pAnim->GetFinalBoneMat()->GetData(m_vecFinalBone.data());

		m_matFinalBone = m_vecFinalBone[m_iIndex];
		m_matFinalBone.m[3][3] = 1;

		m_matFinalBone = XMMatrixTranspose(m_matFinalBone);

		return true;
	}

	return false;
}

void CEquip::SetDead(bool _bDelete)
{
	if (_bDelete)
	{
		DestroyObject(GetOwner()); //내 무기 삭제
		return;
	}
	m_pChar = nullptr;
}

void CEquip::SaveToLevelFile(FILE* _File)
{

}

void CEquip::LoadFromLevelFile(FILE* _FILE)
{

}

