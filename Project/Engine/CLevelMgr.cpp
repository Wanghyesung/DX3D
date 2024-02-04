#include "pch.h"
#include "CLevelMgr.h"

#include "CLayer.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	/*CStartLevel* pStartLevel = new CStartLevel();
	pStartLevel->init();
	pStartLevel->ChangeState(LEVEL_STATE::STOP);
	m_mapLevel.insert(make_pair(pStartLevel->GetName(), pStartLevel));
	m_pCurLevel = pStartLevel;
	m_pCurLevel->enter();*/
	m_pCurLevel = new CLevel;
	m_pCurLevel->ChangeState(LEVEL_STATE::STOP);
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();		
	}

	m_pCurLevel->finaltick();
}


CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);	
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _vec)
{
	m_pCurLevel->FindObjectByName(_strName, _vec);
}

void CLevelMgr::EraseObject(int _iLayer, CGameObject* _pGameObj)
{
	vector<CGameObject*>& vecObj = m_pCurLevel->GetLayer(_iLayer)->m_vecParentObj;

	vector<CGameObject*>::iterator iter = vecObj.begin();
	for (iter; iter != vecObj.end(); ++iter)
	{
		if (*iter == _pGameObj)
		{
			vecObj.erase(iter);
			return;
		}
	}
}

CLevel* CLevelMgr::FindLevel(const wstring& _strName)
{
	map<wstring, CLevel*>::iterator iter =
		m_mapLevel.find(_strName);

	if(iter == m_mapLevel.end())
		return nullptr;

	return iter->second;
}

void CLevelMgr::ChangeLevel(CLevel* _pLevel)
{
	CLevel* pNextLevel = FindLevel(_pLevel->GetName());

	m_pCurLevel->exit();

	//if (nullptr != m_pCurLevel)
	//{
	//	delete m_pCurLevel;
	//	m_pCurLevel = nullptr;
	//}

	m_pCurLevel = pNextLevel;

	m_pCurLevel->enter();
}

void CLevelMgr::AddLevel(CLevel* _pLevel)
{
	m_mapLevel.insert(make_pair(_pLevel->GetName(), _pLevel));
}
