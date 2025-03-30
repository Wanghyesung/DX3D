#include "pch.h"
#include "CRDNavMeshField.h"

#include "CNavMeshMgr.h"
#include "CPxRigidbody.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CTimeMgr.h"

CRDNavMeshField::CRDNavMeshField() :
    CComponent(COMPONENT_TYPE::RDNAVMESHFIELD),
    m_bActive(false),
	m_iWriteIndex(0),
	m_atReady(false)
{

}

CRDNavMeshField::~CRDNavMeshField()
{
	
}


void CRDNavMeshField::begin()
{
	m_pTarget =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
}

//CPU가 최적화 코드 재배치 방지
//release : store 이전의 모든 작업은 다 끝난 상태
//acquire : load 이후의 작업은 load가 끝난 뒤에만 실행

void CRDNavMeshField::SetPathDir(const Vec3& _vPathDir)
{
	int iNextIdx = (m_iWriteIndex.load() + 1) % 2;
	m_vPreDir[iNextIdx] = _vPathDir;
	m_iWriteIndex.store(iNextIdx, std::memory_order_release);
	m_atReady.store(true, std::memory_order_release);
}

const Vec3& CRDNavMeshField::GetFindPath()
{
	if (m_atReady.load(std::memory_order_acquire))
	{
		int iReadIdx = m_iWriteIndex.load(std::memory_order_acquire);
		m_vPathDir = m_vPreDir[iReadIdx];
		m_atReady.store(false, std::memory_order_release);
	}

	return m_vPathDir;
}

void CRDNavMeshField::finaltick()
{
    
}


void CRDNavMeshField::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMeshField::LoadFromLevelFile(FILE* _FILE)
{
}

