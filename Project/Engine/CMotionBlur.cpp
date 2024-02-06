#include "pch.h"
#include "CMotionBlur.h"

#include "CTransform.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CMaterial.h"
#include "CTimeMgr.h"
#include "CStructuredBuffer.h"
#include "CAnimator3D.h"
CMotionBlur::CMotionBlur() :
	CComponent(COMPONENT_TYPE::MOTIONBLUR),
	m_fPosCheckTime(0.1f),
	m_fCurCheckTime(0.f),
	m_pMotionBlur(nullptr)
{
	SetName(L"MotionBlur");


	//MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
}

CMotionBlur::~CMotionBlur()
{

}

void CMotionBlur::Initialize()
{
	create_mesh();
}


void CMotionBlur::finaltick()
{
	//m_fCurCheckTime += DT;
	//if (m_fCurCheckTime >= m_fPosCheckTime)
	//{
	//	m_fCurCheckTime = 0.f;
	//
	//}

	Vec3 vScale = Transform()->GetRelativeScale();
	m_pMotionBlur->Transform()->SetRelativeScale(vScale);

	Vec3 vPostion = Transform()->GetRelativePos();
	m_pMotionBlur->Transform()->SetRelativePos(vPostion);

	//const vector<CGameObject*>& vecChild = m_pMotionBlur->GetChild();
	//const vector<CGameObject*>& vecOwnerChild = GetOwner()->GetChild();
	//
	//for (int i = 0; i < vecChild.size(); ++i)
	//{
	//	vecChild[i]->MeshRender()->GetMaterial(0)->SetAnim3D(true);
	//	vecOwnerChild[i]->Animator3D()->Animator3D()->GetFinalBoneMat()->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	//}
}

void CMotionBlur::create_mesh()
{
	m_pMotionBlur = new CGameObject;
	m_pMotionBlur->SetName(L"MotionBlurObj");

	const vector<CGameObject*>& vecChild = GetOwner()->GetChild();
	m_pMotionBlur->AddComponent(new CTransform);
	*m_pMotionBlur->Transform() = *GetOwner()->Transform();
	m_pMotionBlur->AddComponent(new CMeshRender);

	//CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh")
	if (vecChild.size() == 0)
	{
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		m_pMotionBlur->MeshRender()->SetMesh(pMesh);
		m_pMotionBlur->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MotionBlurMtrl"), 0);
	}

	for (int i = 0; i < vecChild.size(); ++i)
	{
		CGameObject* pChild = new CGameObject();
		pChild->AddComponent(new CTransform);
		pChild->AddComponent(new CMeshRender);

		Ptr<CMesh> pMesh = vecChild[i]->MeshRender()->GetMesh();
		pChild->MeshRender()->SetMesh(pMesh);
		pChild->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MotionBlurMtrl"), 0);
		m_pMotionBlur->AddChild(pChild);
	}

	SpawnGameObject(m_pMotionBlur, Vec3(0.f, 0.f, 0.f), (UINT)LAYER_TYPE::Default);
}

void CMotionBlur::SaveToLevelFile(FILE* _File)
{

}

void CMotionBlur::LoadFromLevelFile(FILE* _FILE)
{

}







