#include "pch.h"
#include "CGage.h"

#include "CFontMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CMeshRender.h"
#include "CTransform.h"
CGage::CGage():
	m_fMaxRatio(100.f),
	m_fCurRatio(100.f)
{

}

CGage::~CGage()
{

}


void CGage::Initialize(const wstring& _strTexName, const Vec3& _vScale, const wstring& _strName)
{
	//EngineUIMtrl
	CMeshRender* pMeshRender = new CMeshRender;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EngineUIMtrl"), 0);

	AddComponent(pMeshRender);
	AddComponent(new CTransform);

	Ptr<CMaterial> pMaterial = pMeshRender->MeshRender()->GetMaterial(0);
	if (pMaterial->GetTexParam(TEX_0) == nullptr)
		pMaterial->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(_strTexName));
	else
	{
		Ptr<CMaterial> pDyanmicMater = pMeshRender->MeshRender()->GetDynamicMaterial(0);
		pDyanmicMater->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(_strTexName));
	}

	Transform()->SetRelativeScale(_vScale);

	SetName(_strName);

	Vec2 vUISize = Vec2(100.f, 100.f);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &vUISize);
}

void CGage::UpdateGage(float _fMaxGage, float _fCurGage)
{
	m_fCurRatio = _fCurGage / _fMaxGage;
	m_fCurRatio *= m_fMaxRatio;

	//상수 버퍼로 넘기기
	int gage = 1;
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_fCurRatio);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &gage);
}




