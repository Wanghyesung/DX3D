#pragma once
#include "CComponent.h"


class CNavAgent;
typedef unsigned int dtObstacleRef;

class CRDNavMeshField : public CComponent
{

private:
    CGameObject* m_pTarget;

   
    Vec3 m_vPathDir;

    bool m_bActive;
    float m_fRadius;
    /// <summary>
    /// </summary>
    //static const int MAX_POLYS = 256;
    //static const int MAX_SMOOTH = 2048;
    //int m_nsmoothPath;
    //float m_smoothPath[2048 * 3];

    float m_fCurTime;
    float m_fSearchTime;

public:
    
    void SetActive(bool _bActive) { m_bActive = _bActive; }
    bool IsActive() { return m_bActive; }

    void SetSearchTime(float _fTime) { m_fSearchTime = _fTime; }

    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
    float GetRadius() { return m_fRadius; }
public:
    virtual void begin();
    virtual void finaltick();
   
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

