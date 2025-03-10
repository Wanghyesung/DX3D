#pragma once
#include "CComponent.h"


class CNavAgent;
typedef unsigned int dtObstacleRef;

class CRDNavMeshField : public CComponent
{
    friend class CNavMeshMgr;
private:
    CGameObject* m_pTarget;

   
    Vec3 m_vPathDir;

    bool m_bActive;
    float m_fRadius;

public:
    
    void SetActive(bool _bActive) { m_bActive = _bActive; }
    bool IsActive() { return m_bActive; }

    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
    float GetRadius() { return m_fRadius; }

public:
    virtual void begin();
    virtual void finaltick();
   
    void SetPathDir(const Vec3& _vPathDir) { m_vPathDir = _vPathDir; }
   
    const Vec3& GetFindPath() { return m_vPathDir; }
public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE) ;

    CLONE(CRDNavMeshField);
public:
	CRDNavMeshField();
	virtual ~CRDNavMeshField();

    friend class CNavAgent;
};

