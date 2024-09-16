#include "pch.h"
#include "CEngineUI.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CMeshRender.h"
#include "CRenderMgr.h"
#include "CCamera.h"
CEngineUI::CEngineUI():
	m_bLbntDown(false),
	m_bMouseOn(false),
	m_pParentUI(nullptr),
	m_vecChildUI{}
{
	
}

CEngineUI::CEngineUI(const CEngineUI& _pOrigin):
	CGameObject(_pOrigin),
	m_bLbntDown(false),
	m_bMouseOn(false),
	m_pParentUI(nullptr),
	m_vecChildUI{}
{

}

CEngineUI::~CEngineUI()
{
	
}

void CEngineUI::Initialize(const wstring& _strTexName, const wstring& _strName, const Vec3& _vScale)
{
	SetName(_strName);

	CMeshRender* pMeshRender = new CMeshRender;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	
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
}

void CEngineUI::finaltick()
{
	CGameObject::finaltick();

	MouseOnCheck();
}


void CEngineUI::MouseOn()
{
	
}

void CEngineUI::MouseLbtnDown()
{
	int a = 10;
}

void CEngineUI::MouseLbtnUp()
{
}

void CEngineUI::MouseLbtnClicked()
{
}


void CEngineUI::MoveUI(CEngineUI* _pUI)
{
}

void CEngineUI::MoveToParent(Vector2 _vDiff)
{
}

void CEngineUI::MouseOnCheck()
{
	CTransform* pTransform = Transform();
	Vector3 vScale = pTransform->GetRelativeScale();
	Vector3 vPos = pTransform->GetRelativePos();

	
	Vector2 vMousePos = CKeyMgr::GetInst()->GetNDCMousePos();

	if ((vPos.x - vScale.x / 2.f) <= vMousePos.x && vMousePos.x <= (vPos.x + vScale.x / 2.f) &&
		(vPos.y - vScale.y / 2.f) <= vMousePos.y && vMousePos.y <= (vPos.y + vScale.y / 2.f))
		m_bMouseOn = true;
	else
		m_bMouseOn = false;
}

void CEngineUI::UpdateBillboard()
{
	int iTrue = TRUE;
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &iTrue);

	CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();

	Vec3 vCameraPos = pMainCam->Transform()->GetRelativePos();
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vDir = (vCameraPos - vPos).Normalize();

	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);

	float fRadian;
	float fCos = vDir.Dot(vFoward);

	Vec3 vCross = vFoward.Cross(vDir);

	if (vCross.y >= 0)
		fRadian = acos(fCos);
	else
		fRadian = -acos(fCos);

	Vec3 vRot = Transform()->GetRelativeRot();
	Transform()->SetRelativeRot(vRot.x, fRadian, vRot.z);

}

Ptr<CMaterial> CEngineUI::create_uimatrial()
{
	Ptr<CGraphicsShader> pShader = new CGraphicsShader();
	//pShader->SetKey(L"")
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	//CResMgr::GetInst()->AddRes(pShader->GetKey(), pShader);

	Ptr<CMaterial> pMaterial = new CMaterial();
	pMaterial->SetShader(pShader);

	return pMaterial;
}
