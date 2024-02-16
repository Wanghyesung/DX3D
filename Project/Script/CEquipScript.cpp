#include "pch.h"
#include "CEquipScript.h"

#include <Engine\CTransform.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CStructuredBuffer.h>

CEquipScript::CEquipScript() :
	CScript(SCRIPT_TYPE::EQUIPSCRIPT),
	m_pTarget(nullptr),
	m_iIndex(38)
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

	
	Matrix matOwnerWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();//몬스터
	Matrix matTargetWorld = m_pTarget->Transform()->GetWorldMat();//무기
	//Matrix matTargetScaleInv = XMMatrixInverse(nullptr, m_pTarget->Transform()->GetWorldScaleMat());
	//matTargetWorld *= matTargetScaleInv;
	
	matTargetWorld *= matOwnerWorld;
	m_matFinalBone *= matTargetWorld;
	

	XMVECTOR S, R, T;
	XMMatrixDecompose(&S, &R, &T, m_matFinalBone);

	const vector<CGameObject*>& vecChild  = m_pTarget->GetChild();
	for (int i = 0 ; i<vecChild.size(); ++i )
	{
		vecChild[i]->Transform()->SetRelativePos(T);
	}
	//m_pTarget->Transform()->SetRelativePos(T);
	//m_pTarget->Transform()->SetRelativeRot(R);
	//m_pTarget->Transform()->SetRelativeScale(S);

}

void CEquipScript::begin()
{
	if (!m_pTarget)
		return;

	//-90 280 100 rot
	// -15 162 173
}

bool CEquipScript::check_matrix()
{
	CAnimator3D* pAnim = GetOwner()->Animator3D();


	UINT iBoneCount = pAnim->GetBoneCount();
	m_vecFinalBone.resize(iBoneCount);

	pAnim->GetFinalBoneMat()->GetData(m_vecFinalBone.data());

	
	m_matFinalBone = m_vecFinalBone[m_iIndex];
	m_matFinalBone.m[3][3] = 1;

	m_matFinalBone = XMMatrixTranspose(m_matFinalBone);
	
	return true;
}


