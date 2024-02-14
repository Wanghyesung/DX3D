#include "pch.h"
#include "CEquip.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CMeshRender.h"
#include "CTransform.h"
CEquip::CEquip() :
	CComponent(COMPONENT_TYPE::EQUIP),
	m_iIndex(0),
	m_matFinalBone{}
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
	/*CTransform* pTransform = GetOwner()->Transform();
	CAnimator3D* pAnim = GetOwner()->Animator3D();
	
	bool bOn = pAnim->IsFinalMatUpdate();
	if (bOn)
	{
		UINT iBoneCount = pAnim->GetBoneCount();
		m_vecFinalBone.resize(iBoneCount);

		pAnim->GetFinalBoneMat()->GetData(m_vecFinalBone.data());
	}

	m_matFinalBone = m_vecFinalBone[m_iIndex];
	m_matFinalBone = XMMatrixInverse(nullptr, m_matFinalBone);*/
}

void CEquip::SaveToLevelFile(FILE* _File)
{

}

void CEquip::LoadFromLevelFile(FILE* _FILE)
{

}

