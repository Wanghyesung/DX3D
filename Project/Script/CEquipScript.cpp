#include "pch.h"
#include "CEquipScript.h"

#include <Engine\CTransform.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CStructuredBuffer.h>

CEquipScript::CEquipScript() :
	CScript(SCRIPT_TYPE::EQUIPSCRIPT),
	m_pTarget(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iIndex, "Bone_Index");
}

CEquipScript::~CEquipScript()
{
}

void CEquipScript::tick()
{
	if (!check_matrix())
		return;

	Vec3 VecPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	
	XMVECTOR S,R,T; 
	XMMatrixDecompose(&S, &R, &T, m_matFinalBone);
 	Vec3 vRot = Vec3(R.m128_f32[0], R.m128_f32[2], R.m128_f32[2]);
	Vec3 vFinalPos = XMVector3Transform(VecPos, m_matFinalBone);

	m_pTarget->Transform()->SetRelativePos(vFinalPos);
	m_pTarget->Transform()->SetRelativeRot(vRot);
}

bool CEquipScript::check_matrix()
{
	CAnimator3D* pAnim = GetOwner()->Animator3D();

	bool bOn = pAnim->IsFinalMatUpdate();
	if (bOn)
	{
		UINT iBoneCount = pAnim->GetBoneCount();
		m_vecFinalBone.resize(iBoneCount);

		pAnim->GetFinalBoneMat()->GetData(m_vecFinalBone.data());

		
		m_matFinalBone = m_vecFinalBone[m_iIndex];
		m_matFinalBone = XMMatrixTranspose(m_matFinalBone);
		
		return true;
	}

	return false;
}


