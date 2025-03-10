#include "pch.h"
#include "CRDNavMeshField.h"

#include "CNavMeshMgr.h"
#include "CPxRigidbody.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CTimeMgr.h"

CRDNavMeshField::CRDNavMeshField() :
    CComponent(COMPONENT_TYPE::RDNAVMESHFIELD),
    m_bActive(false)
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
    
}


void CRDNavMeshField::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMeshField::LoadFromLevelFile(FILE* _FILE)
{
}

