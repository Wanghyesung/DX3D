#include "pch.h"
#include "CEngineUI.h"
#include "CTransform.h"
#include "CKeyMgr.h"
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
	for (CEngineUI* pChildUI : m_vecChildUI)
	{
		if (pChildUI != nullptr)
		{
			delete pChildUI;
			pChildUI = nullptr;
		}
	}
}

void CEngineUI::Initialize()
{

}

void CEngineUI::finaltick()
{

}


void CEngineUI::MouseOn()
{
}

void CEngineUI::MouseLbtnDown()
{
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

	
	//Vector2 vMousePos = CKeyMgr::GetInst()->GetNDCMousePos();

	//if ((vPos.x - vScale.x / 2.f) <= vMousePos.x && vMousePos.x <= (vPos.x + vScale.x / 2.f) &&
	//	(vPos.y - vScale.y / 2.f) <= vMousePos.y && vMousePos.y <= (vPos.y + vScale.y / 2.f))
	//	m_bMouseOn = true;
	//else
	//	m_bMouseOn = false;
}