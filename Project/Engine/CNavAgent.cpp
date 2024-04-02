#include "pch.h"
#include "CNavAgent.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CPxRigidbody.h"
int CNavAgent::m_iAgentCount = 0;

CNavAgent::CNavAgent():
	CComponent(COMPONENT_TYPE::NAVAGENT),
    m_pCrowd(nullptr),
    m_pTaretObj(nullptr)
{
    m_tAgentParams.radius = 1;
    m_tAgentParams.maxAcceleration = 0;
    m_tAgentParams.maxSpeed = 5;
    m_tAgentParams.collisionQueryRange = 12;
    m_tAgentParams.pathOptimizationRange = 30;
    m_tAgentParams.separationWeight = 2;
    m_tAgentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS |
        DT_CROWD_OBSTACLE_AVOIDANCE;
    m_tAgentParams.obstacleAvoidanceType = (unsigned char)3;

    
}

CNavAgent::~CNavAgent()
{
    m_pCrowd->removeAgent(m_iAgentIdx);
}


void CNavAgent::AssignToNavigationField(CRDNavMeshField* _pNavField)
{
    m_pNavMesh = _pNavField;
    m_pCrowd = m_pNavMesh->crowd;

    int i = m_pNavMesh->crowd->getAgentCount();

    m_iAgentIdx = m_iAgentCount;
    ++m_iAgentCount;
}

void CNavAgent::SetSpeed(float _fSpeed)
{
    m_tAgentParams.maxSpeed = _fSpeed;
    if (m_pCrowd != nullptr)
        m_pCrowd->updateAgentParameters(m_iAgentIdx, &m_tAgentParams);
}

void CNavAgent::SetAcceleration(float _fAccel)
{
    m_tAgentParams.maxAcceleration = _fAccel;
    if (m_pCrowd != nullptr)
        m_pCrowd->updateAgentParameters(m_iAgentIdx, &m_tAgentParams);
}

void CNavAgent::SetRadius(float _fRadius)
{
    m_tAgentParams.radius = _fRadius;
    if (m_pCrowd != nullptr)
        m_pCrowd->updateAgentParameters(m_iAgentIdx, &m_tAgentParams);
}

const Vec3 CNavAgent::GetTargetPosition()
{
    return m_vTargetPos;
}

float CNavAgent::GetSpeed()
{
    return m_tAgentParams.maxSpeed;
}

float CNavAgent::GetAcceleration()
{
    return m_tAgentParams.maxAcceleration;
}

float CNavAgent::GetRadius()
{
    return m_tAgentParams.radius;
}

void CNavAgent::MoveTo(Vec3 destination)
{
    if (m_pNavMesh == nullptr)
        return;

    Vec3 vTargetPos = m_pTaretObj->PxRigidbody()->GetPxPosition();
    float fTargetPos[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };

    const dtQueryFilter* filter = m_pCrowd->getFilter(0);
    const dtCrowdAgent* agent = m_pCrowd->getAgent(m_iAgentIdx);
    const float* halfExtents = m_pCrowd->getQueryExtents();

    m_pNavMesh->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &targetRef, vTargetPos);
    m_pCrowd->requestMoveTarget(m_iAgentIdx, targetRef, vTargetPos);


}

void CNavAgent::begin()
{
    m_pTaretObj =
        CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
}

void CNavAgent::finaltick()
{
    if (!m_pTaretObj)
        return;

    const Vec3& vPos = GetOwner()->PxRigidbody()->GetPxPosition();
    MoveTo(vPos);
}

void CNavAgent::SaveToLevelFile(FILE* _File)
{

}

void CNavAgent::LoadFromLevelFile(FILE* _FILE)
{

}
