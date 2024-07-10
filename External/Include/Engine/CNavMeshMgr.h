#pragma once
#include "CSingleton.h"
#include "Recast.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

struct tNavMeshInfo
{
    rcContext* context;
    rcPolyMesh* polyMesh;
    rcConfig config;
    rcPolyMeshDetail* polyMeshDetail;
    rcCompactHeightfield* compactHeightField;
    rcHeightfield* heightField;
    dtNavMesh* navMesh;

    dtNavMeshQuery* navQuery;
    dtCrowd* crowd;
    dtQueryFilter m_filter;
};

struct tBuildSettings
{
    UINT ID;
    // 길찾기 주체들의 최대 개체수
    int maxCrowdNumber{ 1024 };
    // 길찾기 주체들의 최대 충돌반경
    float maxAgentRadius{ 30 };
    // 오를수 있는 경사
    float walkableSlopeAngle{ 45 };

    float agentRadius{ 1000.f };
    // 오를 수 있는 단차
    float walkableClimb{ 0.3f };
    // 천장의 최소 높이
    float walkableHeight{ 1.f };
    // x축,z축 공간 분할의 단위, 단위가 작을수록 판정이 더 세밀해지지만, 네비게이션 빌드와 길찾기 시스템의 부하가 늘게 된다.
    float divisionSizeXZ{ 5.f };
    // y축 공간 분할의 단위, 단위가 작을수록 판정이 더 세밀해지지만, 네비게이션 빌드와 길찾기 시스템의 부하가 늘게 된다.
    float divisionSizeY{ 5.f };
    // 공간 분할은 xz축으로 250*330, y축으로 200개 정도 분할되는 정도면 순식간에 네비게이션 빌드도 되면서 길찾기도 무리없이 하게 되는 정도다.
    // xz축으로 743* 989개 정도 분할이 되도 큰 부하는 없다.
};


static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

#define MAX_POLY 256 
#define tNaviResult Vec3
class CNavMeshMgr : public CSingleton<CNavMeshMgr>
{
    SINGLE(CNavMeshMgr);

private:
    static UINT m_iPlaneCount;
    vector<Vec3> m_vecWorldVertices;
    vector<int> m_vecWorldFaces;

    rcContext* context;
    //rcPolyMesh* polyMesh;
    //rcConfig config;
    //rcPolyMeshDetail* polyMeshDetail;
    //rcCompactHeightfield* compactHeightField;
    //rcHeightfield* heightField;
    //dtNavMesh* navMesh;
    //
    //dtNavMeshQuery* navQuery;
    //dtCrowd* crowd;
    //dtQueryFilter m_filter;

    map<UINT, tNavMeshInfo> m_mapNavMesh;
private:
   
    //unordered_map<UINT, tNavMeshInfo>  m_hashObjID;
private:
    Vec3 m_vNavMeshScale; //현재 네비메쉬의 크기
    bool RayResultTrigger;

private:
    bool LoadNavMeshFromFile(const char* path);
    void free();
    
public:
    void init();
    void tick();
    void render();

    void CreatePlane(Vec3 _vPos, Vec3 _vScale);

    void BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const tBuildSettings& buildSettings = tBuildSettings{});
    void BuildField(const tBuildSettings& buildSettings = tBuildSettings{})
    {
        assert(sizeof(Vec3) == sizeof(float) * 3);
        assert(!m_vecWorldVertices.empty() && !m_vecWorldFaces.empty());
        assert(m_vecWorldFaces.size() % 3 == 0);
        BuildField(reinterpret_cast<float*>(&m_vecWorldVertices[0]), m_vecWorldVertices.size(),
            &m_vecWorldFaces[0], m_vecWorldFaces.size() / 3, buildSettings);
    }
    void ReBuildField();

    const static UINT GetPlaneCount() { return m_iPlaneCount; }
    void PlusPlaneCount() { ++m_iPlaneCount; }

public:
    const Vec3& FindPath(UINT _ID, float * _pStartPos, float* _pEndPos);

    //UINT InitMesh(const Vec3& _vScale);//메쉬 생성후 내 id를 반환해줌

    bool IsValidPoint(UINT _ID, const Vec3& _CheckPos);
public:
    dtCrowd* GetCrowd(UINT _ID) { return m_mapNavMesh.find(_ID)->second.crowd; }
    dtNavMeshQuery* GetNavMeshQuery(UINT _ID) { return m_mapNavMesh.find(_ID)->second.navQuery; }
};

