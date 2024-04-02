#pragma once
#include "CComponent.h"
#include "CRDNavMeshField.h"

class CRDNavMeshField;
class CNavAgent : public CComponent
{
private:
    CRDNavMeshField* m_pNavMesh;

    static int m_iAgentCount;
    int m_iAgentIdx{ -1 };
    //const dtCrowdAgent* agent{ nullptr };
    dtCrowd* m_pCrowd{ nullptr };
    dtPolyRef targetRef;
    
    Vec3 m_vTargetPos;

    dtCrowdAgentParams m_tAgentParams;
   
    CGameObject* m_pTaretObj;

public:
    void AssignToNavigationField(CRDNavMeshField* _pNavField);

    void SetSpeed(float _fSpeed);
    void SetAcceleration(float _fAccel);
    void SetRadius(float _fRadius);

    const Vec3 GetTargetPosition();
    float GetSpeed();
    float GetAcceleration();
    float GetRadius();
    void MoveTo(Vec3 destination);

public:
    virtual void begin()override;
    virtual void finaltick()override;
  
    CLONE(CNavAgent)
public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);


    CNavAgent();
    virtual ~CNavAgent();
 
 
    friend CRDNavMeshField;
};

