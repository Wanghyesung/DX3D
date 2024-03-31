#pragma once

#include "Recast.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

class CNavField
{
private:
    rcContext* context;
    rcPolyMesh* polyMesh;
    rcConfig config;
    rcPolyMeshDetail* polyMeshDetail;
    class dtNavMesh* navMesh;
    class dtNavMeshQuery* navQuery;
    class dtCrowd* crowd;

 public:
    struct BuildSettings
    {
        // ��ã�� ��ü���� �ִ� ��ü��
        int maxCrowdNumber{ 1024 };
        // ��ã�� ��ü���� �ִ� �浹�ݰ�
        float maxAgentRadius{ 0.6 };
        // ������ �ִ� ���
        float walkableSlopeAngle{ 30 };
        // ���� �� �ִ� ����
        float walkableClimb{ 0.2 };
        // õ���� �ּ� ����
        float walkableHeight{ 0.3 };
        // x��,z�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
        float divisionSizeXZ{ 0.3 };
        // y�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
        float divisionSizeY{ 0.2 };
        // ���� ������ xz������ 250*330, y������ 200�� ���� ���ҵǴ� ������ ���İ��� �׺���̼� ���嵵 �Ǹ鼭 ��ã�⵵ �������� �ϰ� �Ǵ� ������.
        // xz������ 743* 989�� ���� ������ �ǵ� ū ���ϴ� ����.
    };
    CNavField();
    virtual ~CNavField();
  
    void BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings = BuildSettings{});
    void BuildField(vector<Vec3> worldVertices, std::vector<int> faces, const BuildSettings& buildSettings = BuildSettings{})
    {
        assert(sizeof(Vec3) == sizeof(float) * 3);
        assert(!worldVertices.empty() && !faces.empty());
        assert(faces.size() % 3 == 0);
        BuildField(reinterpret_cast<float*>(&worldVertices[0]), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
    }

    void CreatePlane(Vec3 botleft, Vec3 topright, vector<Vec3>& worldVertices, vector<int>& worldFaces);
};

