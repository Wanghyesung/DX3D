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
    // ��ã�� ��ü���� �ִ� ��ü��
    int maxCrowdNumber{ 1024 };
    // ��ã�� ��ü���� �ִ� �浹�ݰ�
    float maxAgentRadius{ 30 };
    // ������ �ִ� ���
    float walkableSlopeAngle{ 45 };

    float agentRadius{ 1000.f };
    // ���� �� �ִ� ����
    float walkableClimb{ 0.3f };
    // õ���� �ּ� ����
    float walkableHeight{ 1.f };
    // x��,z�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float divisionSizeXZ{ 5.f };
    // y�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float divisionSizeY{ 5.f };
    // ���� ������ xz������ 250*330, y������ 200�� ���� ���ҵǴ� ������ ���İ��� �׺���̼� ���嵵 �Ǹ鼭 ��ã�⵵ �������� �ϰ� �Ǵ� ������.
    // xz������ 743* 989�� ���� ������ �ǵ� ū ���ϴ� ����.
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
    Vec3 m_vNavMeshScale; //���� �׺�޽��� ũ��
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

    //UINT InitMesh(const Vec3& _vScale);//�޽� ������ �� id�� ��ȯ����

    bool IsValidPoint(UINT _ID, const Vec3& _CheckPos);
public:
    dtCrowd* GetCrowd(UINT _ID) { return m_mapNavMesh.find(_ID)->second.crowd; }
    dtNavMeshQuery* GetNavMeshQuery(UINT _ID) { return m_mapNavMesh.find(_ID)->second.navQuery; }
};

