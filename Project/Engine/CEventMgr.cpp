#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CFSM.h"
#include "CMonsterFSM.h"
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

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{
			// wParam : GameObject, lParam : Layer Index
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;
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

		case EVENT_TYPE::ADD_CHILD:
			// wParam : ParentObject, lParam : ChildObject
		{
			CGameObject* pDestObj = (CGameObject*)m_vecEvent[i].wParam;
			CGameObject* pSrcObj = (CGameObject*)m_vecEvent[i].lParam;

			// �θ�� ������ ������Ʈ�� ������, Child ������Ʈ�� �ֻ��� �θ� ������Ʈ�� �ȴ�.
			if (nullptr == pDestObj)
			{
				if (pSrcObj->GetParent())
				{
					// ���� �θ���� ���� ����
					pSrcObj->DisconnectFromParent();
					// �ֻ��� �θ� ������Ʈ��, �Ҽ� ���̾ ���
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
			// �ڽ� Ÿ�� ������Ʈ�� ���
			if (m_vecGC[i]->GetParent())
				m_vecGC[i]->DisconnectFromParent();

			delete m_vecGC[i];

			//outliner���� Ȯ���ϰ� tool���� ��� ����
			m_LevelChanged = true;
		}
	}
	m_vecGC.clear();
}