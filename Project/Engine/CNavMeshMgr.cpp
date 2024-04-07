#include "pch.h"
#include "CNavMeshMgr.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLevel.h"
#include "CEventMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CCollisionMgr.h"

UINT CNavMeshMgr::m_iNextID = 0;

CNavMeshMgr::CNavMeshMgr()
    : m_NavMesh(nullptr)
    , m_NavQuery(nullptr)
    , RayResultTrigger(false)
    , m_MapCollider(nullptr)
    , m_hashObjID{}
{
}

CNavMeshMgr::~CNavMeshMgr()
{
    if (m_NavMesh)
    {
        dtFreeNavMesh(m_NavMesh);
        m_NavMesh = nullptr;
    }
    if (m_NavQuery)
    {
        dtFreeNavMeshQuery(m_NavQuery);
        m_NavQuery = nullptr;
    }
}


bool CNavMeshMgr::LoadNavMeshFromFile(const char* path)
{
    // NavMesh ���� ��ηκ��� �ҷ�����
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    wstring wpath = converter.from_bytes(path);
    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += wpath;

    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
    if (err != 0 || !fp)
    {
        return 0;
    }

    // NavMesh ��� �о����
    NavMeshSetHeader header;
    fread(&header, sizeof(NavMeshSetHeader), 1, fp);
    if (header.magic != NAVMESHSET_MAGIC)
    {
        fclose(fp);
        return false;
    }
    if (header.version != NAVMESHSET_VERSION)
    {
        fclose(fp);
        return false;
    }

    // NavMesh �ʱ�ȭ
    m_NavMesh = dtAllocNavMesh();
    if (!m_NavMesh)
    {
        fclose(fp);
        return false;
    }

    dtStatus status = m_NavMesh->init(&header.params);
    if (dtStatusFailed(status))
    {
        fclose(fp);
        return false;
    }

    // NavMesh�� Ÿ�� ���� �о���� (����� Ÿ�� �ɼ��� ������� �ʽ��ϴ�)
    for (int i = 0; i < header.numTiles; ++i)
    {
        NavMeshTileHeader tileHeader;
        fread(&tileHeader, sizeof(tileHeader), 1, fp);
        if (!tileHeader.tileRef || !tileHeader.dataSize)
            break;

        unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
        if (!data) break;
        memset(data, 0, tileHeader.dataSize);
        fread(data, tileHeader.dataSize, 1, fp);

        m_NavMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
    }

    // NavQuery �ʱ�ȭ
    m_NavQuery = dtAllocNavMeshQuery();
    if (!m_NavQuery)
    {
        dtFreeNavMesh(m_NavMesh);
        m_NavQuery = nullptr;
        fclose(fp);
        return false;
    }
    if (dtStatusFailed(m_NavQuery->init(m_NavMesh, 2048))) {
        assert(false);
    }

    return true;

}

void CNavMeshMgr::init()
{
    if (!m_NavMesh && !m_NavQuery)
        LoadNavMeshFromFile("navmesh\\solo_navmesh.bin");
}

void CNavMeshMgr::tick()
{

}

void CNavMeshMgr::render()
{

}

vector<Vec3> CNavMeshMgr::FindPath(const Vec3& _vStartPos, const Vec3& _vEndPos, UINT _iId)
{
    NavMeshID ID = m_hashObjID[_iId];

    // ���� ��ġ�� �� ��ġ�� �����մϴ�.
    float startpos[3] = { _vStartPos.x, _vStartPos.y, -_vStartPos.z }; // ���� ��ġ
    float endpos[3] = { _vEndPos.x, _vEndPos.y, -_vEndPos.z }; // �� ��ġ

    dtPolyRef startRef, endRef;
    //float polyPickExt[3] = { ID.vScale.x, ID.vScale.y, ID.vScale.z };
    //float polyPickExt[3] = { 6000,6000,6000 }; // ������ �����ϱ� ���� ����
    float polyPickExt[3] = { 22000,22000,22000 }; // ������ �����ϱ� ���� ����

    dtQueryFilter filter;
    filter.setIncludeFlags(0xFFFF); // ��� ������ ����
    filter.setExcludeFlags(0);      // ������ ������ ����

    // ���� ������(��, ��, �� ��)�� ����ġ�� �ִ� �κ��ε� ���� �޽����� ���� ���� ������ �Ǿ����� �ʽ��ϴ�.
    //filter.setAreaCost(1, 1.0f); // Set cost for ground area.
    //filter.setAreaCost(2, 10.0f); // Set high cost for water area.
    //filter.setAreaCost(3, FLT_MAX); // Set infinite cost for wall area.

    // ����� ������ �˻�
    dtStatus status01 = m_NavQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
    dtStatus status02 = m_NavQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

    // ���۰� �� ��ġ�� ã���ϴ�.
    float nearestStartPos[3], nearestEndPos[3];
    status01 = m_NavQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
    status02 = m_NavQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

    // ��θ� ��ȹ�մϴ�.
    dtPolyRef path[MAX_POLY];
    int pathCount;
    m_NavQuery->findPath(startRef, endRef, nearestStartPos, nearestEndPos, &filter, path, &pathCount, 256);

    // ��θ� ���� ���� �̵� ��θ� �����մϴ�.
    float* actualPath = new float[3 * MAX_POLY];
    int actualPathCount;
    m_NavQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, &actualPathCount, MAX_POLY);

    // Vec3 ������ ��θ� �����մϴ�.
    vector<Vec3> vecPath(actualPathCount);
    for (int i = 0; i < actualPathCount; ++i)
    {
        vecPath[i] = Vec3(actualPath[3 * i], actualPath[3 * i + 1], -actualPath[3 * i + 2]);
    }

    delete[] actualPath; // ���̻� �ʿ���� calcPath�� �����մϴ�.

    return vecPath;

}

UINT CNavMeshMgr::InitMesh(const Vec3& _vScale)
{
    NavMeshID IDValue = {};
    IDValue.vScale = _vScale;

    UINT iID = m_iNextID++;
    m_hashObjID.insert(make_pair(iID, IDValue));

    return iID;
}

bool CNavMeshMgr::IsValidPoint(const Vec3& _CheckPos)
{
    float checkpos[3] = { _CheckPos.x, _CheckPos.y, -_CheckPos.z }; // �˻� ��ġ

    dtPolyRef checkRef;
    float polyPickExt[3] = { 6000,6000,6000 }; // ������ �����ϱ� ���� ����

    dtQueryFilter filter;
    filter.setIncludeFlags(0xFFFF); // ��� ������ ����
    filter.setExcludeFlags(0);      // ������ ������ ����

    // ����� ������ �˻�
    dtStatus status = m_NavQuery->findNearestPoly(checkpos, polyPickExt, &filter, &checkRef, 0);

    // ���۰� �� ��ġ�� ã���ϴ�.
    float nearestPos[3];
    status = m_NavQuery->closestPointOnPoly(checkRef, checkpos, nearestPos, 0);

    Vec3 FinalPos = { nearestPos[0], nearestPos[1], -nearestPos[2] };
    Vec3 OriginPos = _CheckPos;

    float diffX = FinalPos.x - OriginPos.x;
    float diffZ = FinalPos.z - OriginPos.z;

    if (fabsf(diffX) > 0.1f || fabsf(diffZ) > 0.1f)
    {
        return false;
    }

    return true;

}
