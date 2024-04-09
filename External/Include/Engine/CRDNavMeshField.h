#pragma once
#include "CComponent.h"
#include "Recast.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"


struct tBuildSettings
{
    // ��ã�� ��ü���� �ִ� ��ü��
    int maxCrowdNumber{ 1024 };
    // ��ã�� ��ü���� �ִ� �浹�ݰ�
    float maxAgentRadius{ 30 };
    // ������ �ִ� ���
    float walkableSlopeAngle{ 45 };
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

class CNavAgent;
typedef unsigned int dtObstacleRef;

class CRDNavMeshField : public CComponent
{

private:
    static UINT m_iPlaneCount;

	CGameObject* m_pTarget;

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
   
  
    vector<Vec3> m_worldVertices;
    vector<int> m_worldFaces;
    //result
    Vec3 m_vPathDir;


    bool m_bActive;

    /// <summary>
    /// </summary>
    static const int MAX_POLYS = 256;
    static const int MAX_SMOOTH = 2048;
    int m_nsmoothPath;
    float m_smoothPath[2048 * 3];

    float m_fCurTime;
    float m_fSearchTime;

public:
	void RecastCleanup();

    //void BuildObstacle(tile)
    void BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const tBuildSettings& buildSettings = tBuildSettings{});
    void BuildField(const tBuildSettings& buildSettings = tBuildSettings{})
    {
        assert(sizeof(Vec3) == sizeof(float) * 3);
        assert(!m_worldVertices.empty() && !m_worldFaces.empty());
        assert(m_worldFaces.size() % 3 == 0);
        BuildField(reinterpret_cast<float*>(&m_worldVertices[0]), m_worldVertices.size(), &m_worldFaces[0], m_worldFaces.size() / 3, buildSettings);
    }
    

	int FindPath(float* pStartPos, float* pEndPos);
    void CreatePlane(Vec3 botleft, Vec3 topright);
    void CreatePlane2(const Vec3& _vPos, Vec3 _vScale, bool _bHeightMesh);

    void SetActive(bool _bActive) { m_bActive = _bActive; }
    bool IsActive() { return m_bActive; }

    void SetSearchTime(float _fTime) { m_fSearchTime = _fTime; }
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

