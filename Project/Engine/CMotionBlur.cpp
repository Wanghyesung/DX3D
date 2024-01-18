#include "pch.h"
#include "CMotionBlur.h"

#include "CTransform.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CMaterial.h"
#include "CTimeMgr.h"
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
}

void CMotionBlur::create_mesh()
{
	m_pMotionBlur = new CGameObject;
	m_pMotionBlur->SetName(L"MotionBlurObj");

	m_pMotionBlur->AddComponent(new CTransform);
	m_pMotionBlur->AddComponent(new CMeshRender);

	//CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh")
	Ptr<CMesh> pMesh = MeshRender()->GetMesh();
	m_pMotionBlur->MeshRender()->SetMesh(pMesh);
	m_pMotionBlur->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MotionBlurMtrl"), 0);


	SpawnGameObject(m_pMotionBlur, Vec3(0.f, 0.f, 0.f), (UINT)LAYER_TYPE::Default);
}

void CMotionBlur::SaveToLevelFile(FILE* _File)
{

}

void CMotionBlur::LoadFromLevelFile(FILE* _FILE)
{

}







