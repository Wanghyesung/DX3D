#pragma once
#include "CSingleton.h"
#include "Recast.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"
#include "CRDNavMeshField.h"

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
    wstring Key;

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
    float walkableHeight{ 10.f };
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
    static map<wstring, tNavMeshInfo> m_mapNavMesh;        //���� �̸��� ���� �׺�޽�
    static map<UINT, CRDNavMeshField*> m_mapNavMeshField;
    static unordered_set<UINT> m_hashDeleteExpected;
    static unordered_set<CRDNavMeshField*> m_hashAddExpected; //���� ���� ���̵�� �׺�޽� ������Ʈ


    static UINT m_iPlaneCount;

    UINT m_iStartingIdx;
    vector<Vec3> m_vecWorldVertices;
    vector<int> m_vecWorldFaces;
        
    rcContext* m_pContext;
    
    atomic<bool> m_bRunning;
    //mutex m_mutex;
    thread m_pathThread;

private:
    bool LoadNavMeshFromFile(const char* path);
    void free();
    
public:
    void init();
    void tick();
    void render();

    CGameObject* CreatePlane(Vec3 _vPos, Vec3 _vScale);


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

    void AddPlaneVertex(class CNavMeshPlane* _pNavMeshPlane);

    void AddNavMeshField(CRDNavMeshField* _pNavMeshField);
    void DeleteNavMeshField(UINT _ID);
public:
    
    static Vec3 FindPath(const wstring& _strKey, float * _pStartPos, float* _pEndPos, float _fSearchRange);
    static void CalculatePath();

  
    bool IsValidPoint(const wstring& _strKey, const Vec3& _CheckPos);

public:
    dtCrowd* GetCrowd(const wstring& _strKey) { return m_mapNavMesh.find(_strKey)->second.crowd; }
    dtNavMeshQuery* GetNavMeshQuery(const wstring& _strKey) { return m_mapNavMesh.find(_strKey)->second.navQuery; }
};

