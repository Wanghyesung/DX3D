#pragma once
#include "CComponent.h"
#include "Recast.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

struct BuildSettings
{
    // ��ã�� ��ü���� �ִ� ��ü��
    int maxCrowdNumber{ 1024 };
    // ��ã�� ��ü���� �ִ� �浹�ݰ�
    float maxAgentRadius{ 30 };
    // ������ �ִ� ���
    float walkableSlopeAngle{ 45 };
    // ���� �� �ִ� ����
    float walkableClimb{ 10.f };
    // õ���� �ּ� ����
    float walkableHeight{ 10.f };
    // x��,z�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float divisionSizeXZ{ 18.f };
    // y�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float divisionSizeY{ 18.f };
    // ���� ������ xz������ 250*330, y������ 200�� ���� ���ҵǴ� ������ ���İ��� �׺���̼� ���嵵 �Ǹ鼭 ��ã�⵵ �������� �ϰ� �Ǵ� ������.
    // xz������ 743* 989�� ���� ������ �ǵ� ū ���ϴ� ����.
};

class CNavAgent;
class CRDNavMeshField : public CComponent
{

private:
	CGameObject* m_pTarget;

    rcContext* context;
    rcPolyMesh* polyMesh;
    rcConfig config;
    rcPolyMeshDetail* polyMeshDetail;
    dtNavMesh* navMesh;
    dtNavMeshQuery* navQuery;
    dtCrowd* crowd;
    dtQueryFilter m_filter;

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

    void BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings = BuildSettings{});
    void BuildField(vector<Vec3> worldVertices, std::vector<int> faces, const BuildSettings& buildSettings = BuildSettings{})
    {
        assert(sizeof(Vec3) == sizeof(float) * 3);
        assert(!worldVertices.empty() && !faces.empty());
        assert(faces.size() % 3 == 0);
        BuildField(reinterpret_cast<float*>(&worldVertices[0]), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
    }
  
	int FindPath(float* pStartPos, float* pEndPos);
    void CreatePlane(Vec3 botleft, Vec3 topright, vector<Vec3>& worldVertices, vector<int>& worldFaces);

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

