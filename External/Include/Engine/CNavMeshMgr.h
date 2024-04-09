#pragma once
#include "CSingleton.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

struct NavMeshSetHeader
{
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};

struct NavMeshTileHeader
{
    dtTileRef tileRef;
    int dataSize;
};

struct NavMeshID
{
    Vec3 vScale;
};

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

#define MAX_POLY 256 
#define tNaviResult Vec3
class CNavMeshMgr : public CSingleton<CNavMeshMgr>
{
    SINGLE(CNavMeshMgr);

private:
    dtNavMesh* m_NavMesh;
    dtNavMeshQuery* m_NavQuery;
    CGameObject* m_MapCollider;
    vector<CGameObject*>    m_vColliderObjects;

    static UINT m_iNextID;

    vector<Vec3> m_vecWorldVertexes;
    vector<int>  m_vecWorldIndexes;
private:
    tNaviResult             m_sResultPos;

private:
    vector<Vec4>                    m_vNaviVtx;
    map<pair<float, float>, float>  m_mNaviMap;
   
    unordered_map<UINT, NavMeshID>  m_hashObjID;
private:
    Vec3 m_vNavMeshScale; //현재 네비메쉬의 크기
    bool RayResultTrigger;

private:
    bool LoadNavMeshFromFile(const char* path);

    
public:
    void init();
    void tick();
    void render();

    bool BuildNavMesh();
    void CreatePlane(Vec3 _vPos, Vec3 _vScale);

public:
    vector<Vec3> FindPath(const Vec3& _vStartPos, const Vec3& _vEndPos/*, UINT _iId*/);

    UINT InitMesh(const Vec3& _vScale);//메쉬 생성후 내 id를 반환해줌

    bool IsValidPoint(const Vec3& _CheckPos);
public:
    void SetNaviResult(tNaviResult _result) { m_sResultPos = _result; }
    void SetNavMeshScale(Vec3 _scale) { m_vNavMeshScale = _scale; }
    void SetRayResultTrigger(bool _trigger) { RayResultTrigger = _trigger; }
    void SetMapCollider(CGameObject* _object) { m_MapCollider = _object; }
    void AddColliderObjects(CGameObject* _object) { m_vColliderObjects.push_back(_object); }

    tNaviResult GetNaviResult() { return m_sResultPos; }
    const vector<Vec4> GetNaviVtx() { return m_vNaviVtx; }
    bool GetRayResultTrigger() { return RayResultTrigger; }
    CGameObject* GetMapCollider() { return m_MapCollider; }

};

