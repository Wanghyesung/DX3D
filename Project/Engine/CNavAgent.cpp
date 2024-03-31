#include "pch.h"
#include "CNavAgent.h"

CNavAgent::CNavAgent():
	CComponent(COMPONENT_TYPE::NAVAGENT),
    m_pCrowd(nullptr)
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
}


void CNavAgent::AssignToNavigationField(CRDNavMesh* _pNavField)
{
    m_pNavMesh = _pNavField;

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

    const dtQueryFilter* filter = m_pCrowd->getFilter(0);
    const dtCrowdAgent* agent = m_pCrowd->getAgent(m_iAgentIdx);
    const float* halfExtents = m_pCrowd->getQueryExtents();

    m_pNavMesh->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &targetRef, m_vTargetPos);
    m_pCrowd->requestMoveTarget(m_iAgentIdx, targetRef, m_vTargetPos);
}

void CNavAgent::begin()
{

}

void CNavAgent::finaltick()
{

}

void CNavAgent::SaveToLevelFile(FILE* _File)
{

}

void CNavAgent::LoadFromLevelFile(FILE* _FILE)
{

}
