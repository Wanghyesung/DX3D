#include "pch.h"
#include "CEngineUIMgr.h"
#include "CKeyMgr.h"
#include "CLayer.h"
#include "CLevelMgr.h"
#include "CEngineUI.h"
#include "CFontMgr.h"
void CEngineUIMgr::tick()
{
	m_pFoucseUI = GetFoucseUI();

	if (m_pFoucseUI == nullptr)
		return;

	CEngineUI* pTargetUI = GetTargetUI(m_pFoucseUI);

	
	bool KeyDown = KEY_TAP(KEY::LBTN);
	bool KeyUP = KEY_RELEASE(KEY::LBTN);


	if (pTargetUI != nullptr)
	{
		//������
		pTargetUI->MouseOn();

		if (KeyDown)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbntDown = true;
		}

		else if (KeyUP)
		{
			pTargetUI->MouseLbtnUp();

			//���� �����ӿ����� down�� true���ٸ�
			if (pTargetUI->m_bLbntDown)
			{
				pTargetUI->MouseLbtnClicked();
			}
			pTargetUI->m_bLbntDown = false;
		}

		pTargetUI->m_bMouseOn = false;
	}
}


void CEngineUIMgr::SetFoucseUI(CEngineUI* _pUI)
{

}

void CEngineUIMgr::MoveFrontChildUI(CEngineUI* pUI)
{
}

CEngineUI* CEngineUIMgr::GetFoucseUI()
{
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::UI);

	bool bIsLbntDown = KEY_TAP(KEY::LBTN);

	CEngineUI* pFoucseUI = m_pFoucseUI;

	if (!bIsLbntDown)
		return pFoucseUI;

	vector<CGameObject*> vecUI = pLayer->GetParentObject();

	vector<CGameObject*>::iterator targetiter = vecUI.end();
	vector<CGameObject*>::iterator iter = vecUI.begin();

	//IsMouseOn�� true�� UIã��
	for (; iter != vecUI.end(); ++iter)
	{
		CEngineUI* pUI = dynamic_cast<CEngineUI*>(*iter);
		if (!pUI)
			continue;

		if (pUI->IsMosueOn())
		{
			targetiter = iter;
		}
	}

	if (vecUI.end() == targetiter)
		return nullptr;

	pFoucseUI = ((CEngineUI*)*targetiter);

	vecUI.erase(targetiter);
	vecUI.push_back(pFoucseUI);

	return pFoucseUI;
}

CEngineUI* CEngineUIMgr::GetTargetUI(CEngineUI* _pParentUI)
{
	bool bLbtnUP = KEY_RELEASE(KEY::LBTN);
	static vector<CEngineUI*> vecNoeTarget;
	static list<CEngineUI*> queue;

	CEngineUI* pTargetUI = nullptr;

	queue.clear();
	vecNoeTarget.clear();

	queue.push_back(_pParentUI);

	while (!queue.empty())
	{
		CEngineUI* pUI = queue.front();

		//���� ��Ŀ���� �����ٸ�
		if (pUI->m_bLbntDown)
		{
			pTargetUI = pUI;
			break;
		}

		queue.pop_front();

		if (pUI->IsMosueOn())
		{
			if (pTargetUI != nullptr)
			{
				//���� ��Ŀ�� ���� UI�� �ִٸ� ������ ��Ŀ���� UI�� �����ϱ�
				vecNoeTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			//��Ŀ���� ���� �ʾҴٸ� 
			vecNoeTarget.push_back(pUI);
		}

		const vector<CGameObject*>& vecChildUI = pUI->GetChild();
		for (int i = 0; i < vecChildUI.size(); ++i)
		{
			//�ڽ� UI ť�� �ֱ�
			queue.push_back( dynamic_cast<CEngineUI*>(vecChildUI[i]) );
		}
	}

	if (bLbtnUP)
	{
		//��Ŀ���� Ǯ�� UI ����
		for (int i = 0; i < vecNoeTarget.size(); ++i)
		{
			vecNoeTarget[i]->m_bLbntDown = false;
		}
	}


	return pTargetUI;
}

void CEngineUIMgr::ReleaseChildUI()
{
}
