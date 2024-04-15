#include "pch.h"
#include "CEngineUI.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CMeshRender.h"
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

void CEngineUI::Initialize(const wstring& _strTexName, const Vec3& _vScale)
{
	CMeshRender* pMeshRender = new CMeshRender;
	pMeshRender->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMeshRender->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"),0);

	AddComponent(pMeshRender);
	AddComponent(new CTransform);

	pMeshRender->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0,
		CResMgr::GetInst()->FindRes<CTexture>(_strTexName));

	Transform()->SetRelativeScale(_vScale);
	
	SetName(L"Engine UI");
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