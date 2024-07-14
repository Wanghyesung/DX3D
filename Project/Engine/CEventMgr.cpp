#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CFSM.h"
#include "CMonsterFSM.h"
#include "CFontMgr.h"
CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}


void CEventMgr::tick()
{

	m_LevelChanged = false;

	GC_Clear();
	EC_Clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{
			// wParam : GameObject, lParam : Layer Index
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;
			NewObject->m_bDead = false;

			CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(NewObject, iLayerIdx, false);

			if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
			{
				NewObject->begin();
			}

			m_LevelChanged = true;
		}
		break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			CGameObject* DeleteObject = (CGameObject*)m_vecEvent[i].wParam;

			if (false == DeleteObject->m_bDead)
			{
				DeleteObject->m_bDead = true;
				m_vecGC.push_back(DeleteObject);
			}
		}
		break;

		case EVENT_TYPE::ERASE_OBJECT:
		{
			CGameObject* EraseObject = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;

			if (false == EraseObject->m_bDead)
			{
				//충돌 해지시키기 위해서 이번프레임만 살림
				EraseObject->m_bDead = true;
				m_vecEC.push_back(make_pair(iLayerIdx, EraseObject));
			}
		}
		break;

		case EVENT_TYPE::ADD_CHILD:
			// wParam : ParentObject, lParam : ChildObject
		{
			CGameObject* pDestObj = (CGameObject*)m_vecEvent[i].wParam;
			CGameObject* pSrcObj = (CGameObject*)m_vecEvent[i].lParam;

			// 부모로 지정된 오브젝트가 없으면, Child 오브젝트가 최상위 부모 오브젝트가 된다.
			if (nullptr == pDestObj)
			{
				if (pSrcObj->GetParent())
				{
					// 기존 부모와의 연결 해제
					pSrcObj->DisconnectFromParent();
					// 최상위 부모 오브젝트로, 소속 레이어에 등록
					pSrcObj->AddParentList();
				}
			}
			else
			{
				pDestObj->AddChild(pSrcObj);
			}

			m_LevelChanged = true;
		}



		break;
		case EVENT_TYPE::DELETE_RESOURCE:
			// wParam : RES_TYPE, lParam : Resource Adress
		{
			RES_TYPE type = (RES_TYPE)m_vecEvent[i].wParam;
			CRes* pRes = (CRes*)m_vecEvent[i].lParam;
			CResMgr::GetInst()->DeleteRes(type, pRes->GetKey());
		}

		break;
		case EVENT_TYPE::LEVEL_CHANGE:
		{
			// wParam : Level Adress
			CLevel* Level = (CLevel*)m_vecEvent[i].wParam;
			CLevelMgr::GetInst()->ChangeLevel(Level);
			CRenderMgr::GetInst()->ClearCamera();
			m_LevelChanged = true;
		}
		break;

		case EVENT_TYPE::CHANAGE_STATE:
		{
			CFSM* pFSM = (CFSM*)m_vecEvent[i].wParam;
			STATE_TYPE eType = (STATE_TYPE)m_vecEvent[i].lParam;
			pFSM->ChanageState(eType);
		}

		break;
		case EVENT_TYPE::CHANAGE_MONSTER_STATE:
		{
			CMonsterFSM* pFSM = (CMonsterFSM*)m_vecEvent[i].wParam;
			MONSTER_STATE_TYPE eType = (MONSTER_STATE_TYPE)m_vecEvent[i].lParam;
			pFSM->ChanageState(eType);
		}
		}
	}

	m_vecEvent.clear();
}


void CEventMgr::GC_Clear()
{
	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		if (nullptr != m_vecGC[i])
		{
			// 자식 타입 오브젝트인 경우
			if (m_vecGC[i]->GetParent())
				m_vecGC[i]->DisconnectFromParent();

			delete m_vecGC[i];

			//outliner에서 확인하고 tool에서 목록 갱신
			m_LevelChanged = true;
		}
	}
	m_vecGC.clear();
}

//level vector에 해당 오브젝트를 지우기만 하기 때문에 따로 해제를 해줘야함
void CEventMgr::EC_Clear()
{
	for (size_t i = 0; i < m_vecEC.size(); ++i)
	{
		if (nullptr != m_vecEC[i].second)
		{
			if (m_vecEC[i].second->GetParent())
				m_vecEC[i].second->DisconnectFromParent();

			CLevelMgr::GetInst()->EraseObject(m_vecEC[i].first, m_vecEC[i].second);

			
			m_LevelChanged = true;
		}
	}
	m_vecEC.clear();
}
