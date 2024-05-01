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
    m_fCurTime(0.1f),
    m_fSearchTime(0.1f)
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


void CRDNavMeshField::finaltick()
{
    if (!m_bActive)
        return;

   m_fCurTime += DT;
   if (m_fSearchTime <= m_fCurTime)
   {
       m_fCurTime = 0.f;
   
       if (!m_pTarget)
           return;
   
       Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
       Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
   
       float fStart[3] = { vPos.x, vPos.y,vPos.z };
       float fEnd[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };
   
       m_vPathDir = CNavMeshMgr::GetInst()->FindPath(fStart, fEnd);
   }
}


void CRDNavMeshField::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMeshField::LoadFromLevelFile(FILE* _FILE)
{
}

