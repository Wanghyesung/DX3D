#include "pch.h"
#include "CNavMeshMgr.h"
#include "CTransform.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "CPxRigidbody.h"
#include "CMeshRender.h"
#include "CNavMeshPlane.h"
#include "CLevelMgr.h"
#include "CLayer.h"

UINT CNavMeshMgr::m_iPlaneCount = 0;
map<UINT, CRDNavMeshField*> CNavMeshMgr::m_mapNavMeshField = {};
map<wstring, tNavMeshInfo> CNavMeshMgr::m_mapNavMesh = {};
vector<UINT> CNavMeshMgr::m_vecDeleteExpected = {};

CNavMeshMgr::CNavMeshMgr():
    m_bRunning(true)
{
    m_pContext = new rcContext();

    //스레드 생성
    m_pathThread = thread([this]()
        {
            while (m_bRunning)
            {
                {
                    std::lock_guard<mutex> lock(m_mutex);
                    CalculatePath();
                }
                std::this_thread::sleep_for(0.1s);
            }
        });

}

CNavMeshMgr::~CNavMeshMgr()
{
    {
        //락경합에서 이기면 그 때 false
        std::lock_guard<mutex> lock(m_mutex); 
        m_bRunning.store(false);
    }
    
    if (m_pathThread.joinable())
        m_pathThread.join();

    free();

    if (m_pContext)
        delete m_pContext;
}

inline bool inRange(const float* v1, const float* v2, const float r, const float h)
{
    const float dx = v2[0] - v1[0];
    const float dy = v2[1] - v1[1];
    const float dz = v2[2] - v1[2];
    return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}

static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
    const float minTargetDist,
    const dtPolyRef* path, const int pathSize,
    float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
    float* outPoints = 0, int* outPointCount = 0)
{
    // Find steer target.
    static const int MAX_STEER_POINTS = 3;
    float steerPath[MAX_STEER_POINTS * 3];
    unsigned char steerPathFlags[MAX_STEER_POINTS];
    dtPolyRef steerPathPolys[MAX_STEER_POINTS];
    int nsteerPath = 0;
    navQuery->findStraightPath(startPos, endPos, path, pathSize,
        steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
    if (!nsteerPath)
        return false;

    if (outPoints && outPointCount)
    {
        *outPointCount = nsteerPath;
        for (int i = 0; i < nsteerPath; ++i)
            dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
    }


    // Find vertex far enough to steer to.
    int ns = 0;
    while (ns < nsteerPath)
    {
        // Stop at Off-Mesh link or when point is further than slop away.
        if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
            !inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
            break;
        ns++;
    }
    // Failed to find good point to steer to.
    if (ns >= nsteerPath)
        return false;

    dtVcopy(steerPos, &steerPath[ns * 3]);
    steerPos[1] = startPos[1];
    steerPosFlag = steerPathFlags[ns];
    steerPosRef = steerPathPolys[ns];

    return true;
}
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
    if (npath < 3)
        return npath;

    // Get connected polygons
    static const int maxNeis = 16;
    dtPolyRef neis[maxNeis];
    int nneis = 0;

    const dtMeshTile* tile = 0;
    const dtPoly* poly = 0;
    if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
        return npath;

    for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
    {
        const dtLink* link = &tile->links[k];
        if (link->ref != 0)
        {
            if (nneis < maxNeis)
                neis[nneis++] = link->ref;
        }
    }

    // If any of the neighbour polygons is within the next few polygons
    // in the path, short cut to that polygon directly.
    static const int maxLookAhead = 6;
    int cut = 0;
    for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
        for (int j = 0; j < nneis; j++)
        {
            if (path[i] == neis[j]) {
                cut = i;
                break;
            }
        }
    }
    if (cut > 1)
    {
        int offset = cut - 1;
        npath -= offset;
        for (int i = 1; i < npath; i++)
            path[i] = path[i + offset];
    }

    return npath;
}

CGameObject* CNavMeshMgr::CreatePlane(Vec3 _vPos, Vec3 _vScale)
{

    //1000, 1000      2000, 2000
    Vec3 vTemScale = _vScale / 2.f;
    Vec3 vBotleft = _vPos - vTemScale; // 0, 0  
    Vec3 vTopRight = _vPos + vTemScale;// 2000, 2000

    m_iStartingIdx = m_vecWorldVertices.size();

    // 1 ----4
    // |     |
    // 2-----3
    m_vecWorldVertices.push_back(Vec3(vBotleft.x, vTopRight.y,vTopRight.z));  //0 2000
    m_vecWorldVertices.push_back(Vec3(vBotleft.x, vBotleft.y ,vBotleft.z));   //0, 0, 0
    m_vecWorldVertices.push_back(Vec3(vTopRight.x, vBotleft.y,vBotleft.z));   //2000 0 0
    m_vecWorldVertices.push_back(Vec3(vTopRight.x, vTopRight.y ,vTopRight.z));//2000 2000 0

   
    m_vecWorldFaces.push_back(m_iStartingIdx + 2);
    m_vecWorldFaces.push_back(m_iStartingIdx + 1);
    m_vecWorldFaces.push_back(m_iStartingIdx + 0);
    m_vecWorldFaces.push_back(m_iStartingIdx + 3);
    m_vecWorldFaces.push_back(m_iStartingIdx + 2);
    m_vecWorldFaces.push_back(m_iStartingIdx + 0);

    CGameObject* pGameObj = new CGameObject();
    pGameObj->SetName(L"navMeshPlane" + std::to_wstring(m_iPlaneCount));
    pGameObj->AddComponent(new CTransform);

    pGameObj->Transform()->SetRelativeScale(_vScale);

    //해당 mesh의 월드 좌표 기억해두기
    CNavMeshPlane* pPlane = new CNavMeshPlane();
    for (int i = m_vecWorldVertices.size() - 4; i < m_vecWorldVertices.size(); ++i)
        pPlane->SetWorldVertex(m_vecWorldVertices[i]);
    for (int i = m_vecWorldFaces.size() - 6; i < m_vecWorldFaces.size(); ++i)
        pPlane->SetWorldFaces(m_vecWorldFaces[i]);

    pGameObj->AddComponent(new CMeshRender);
    pGameObj->AddComponent(pPlane);
    SpawnGameObject(pGameObj, _vPos, (int)LAYER_TYPE::NavMeshPlane);

    ++m_iPlaneCount;

    return pGameObj;
}

void CNavMeshMgr::BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const tBuildSettings& buildSettings)
{
    if (m_mapNavMesh.find(buildSettings.Key) != m_mapNavMesh.end())
        return;

    tNavMeshInfo tNavMesh = {};
    float bmin[3] = { WCHAR_MAX,WCHAR_MAX, WCHAR_MAX };
    float bmax[3] = { -WCHAR_MAX, -WCHAR_MAX, -WCHAR_MAX };
    // 바운더리 정보부터 설정
    for (auto i = 0; i < verticesNum; i++)
    {
        if (bmin[0] > worldVertices[i * 3])
            bmin[0] = worldVertices[i * 3];
        if (bmin[1] > worldVertices[i * 3 + 1])
            bmin[1] = worldVertices[i * 3 + 1];
        if (bmin[2] > worldVertices[i * 3 + 2])
            bmin[2] = worldVertices[i * 3 + 2];

        if (bmax[0] < worldVertices[i * 3])
            bmax[0] = worldVertices[i * 3];
        if (bmax[1] < worldVertices[i * 3 + 1])
            bmax[1] = worldVertices[i * 3 + 1];
        if (bmax[2] < worldVertices[i * 3 + 2])
            bmax[2] = worldVertices[i * 3 + 2];
    }
    //auto& config{ impl->config };
    memset(&tNavMesh.config, 0, sizeof(rcConfig));

    tNavMesh.config.cs = buildSettings.divisionSizeXZ;
    tNavMesh.config.ch = buildSettings.divisionSizeY;
    tNavMesh.config.walkableSlopeAngle = buildSettings.walkableSlopeAngle;
    tNavMesh.config.walkableHeight = (int)ceilf(buildSettings.walkableHeight / tNavMesh.config.ch);
    tNavMesh.config.walkableClimb = (int)floorf(buildSettings.walkableClimb / tNavMesh.config.ch);
    tNavMesh.config.walkableRadius = (int)ceilf(buildSettings.agentRadius / tNavMesh.config.cs);
    tNavMesh.config.maxEdgeLen = (int)(tNavMesh.config.cs * 40 / tNavMesh.config.cs);
    tNavMesh.config.maxSimplificationError = 1.3f;
    tNavMesh.config.minRegionArea = (int)rcSqr(tNavMesh.config.cs * 27);		// Note: area = size*size
    tNavMesh.config.mergeRegionArea = (int)rcSqr(tNavMesh.config.cs * 67);	// Note: area = size*size
    tNavMesh.config.maxVertsPerPoly = (int)6;
    tNavMesh.config.detailSampleDist = 6.0f < 0.9f ? 0 : tNavMesh.config.cs * 6.0f;
    tNavMesh.config.detailSampleMaxError = tNavMesh.config.ch * 1;

    rcVcopy(tNavMesh.config.bmin, bmin);
    rcVcopy(tNavMesh.config.bmax, bmax);
    rcCalcGridSize(tNavMesh.config.bmin, tNavMesh.config.bmax, tNavMesh.config.cs,
        &tNavMesh.config.width, &tNavMesh.config.height);

    // 작업 맥락을 저장할 context 객체 생성, 작업의 성패여부를 저장할 processResult 선언
    //context = 
    bool processResult{ false };
    // 복셀 높이필드 공간 할당
    tNavMesh.heightField = rcAllocHeightfield();
    assert(tNavMesh.heightField != nullptr);

    processResult = rcCreateHeightfield(m_pContext, *tNavMesh.heightField, tNavMesh.config.width, tNavMesh.config.height, tNavMesh.config.bmin,
        tNavMesh.config.bmax, tNavMesh.config.cs, tNavMesh.config.ch);
    assert(processResult == true);


    std::vector<unsigned char> triareas;
    triareas.resize(facesNum);
   
    rcMarkWalkableTriangles(m_pContext, tNavMesh.config.walkableSlopeAngle, worldVertices, verticesNum, faces, facesNum, triareas.data());
    processResult = rcRasterizeTriangles(m_pContext, worldVertices, verticesNum, faces, triareas.data(),
        facesNum, *tNavMesh.heightField, tNavMesh.config.walkableClimb);
    assert(processResult == true);

    // 필요없는 부분 필터링
    rcFilterLowHangingWalkableObstacles(m_pContext, tNavMesh.config.walkableClimb, *tNavMesh.heightField);
    rcFilterLedgeSpans(m_pContext, tNavMesh.config.walkableHeight, tNavMesh.config.walkableClimb, *tNavMesh.heightField);
    rcFilterWalkableLowHeightSpans(m_pContext, tNavMesh.config.walkableHeight, *tNavMesh.heightField);

    // 밀집 높이 필드 만들기
    tNavMesh.compactHeightField = rcAllocCompactHeightfield();
    assert(tNavMesh.compactHeightField != nullptr);


    processResult = rcBuildCompactHeightfield(m_pContext, tNavMesh.config.walkableHeight, tNavMesh.config.walkableClimb,
        *tNavMesh.heightField, *tNavMesh.compactHeightField);
    //rcFreeHeightField(heightField);
    assert(processResult == true);

    //agentradius 범위에 따라 메쉬 가공 -> radius가 클수록 메쉬 범위 작게
    processResult = rcErodeWalkableArea(m_pContext, tNavMesh.config.walkableRadius, *tNavMesh.compactHeightField);
    assert(processResult == true);

    processResult = rcBuildDistanceField(m_pContext, *tNavMesh.compactHeightField);
    assert(processResult == true);

    rcBuildRegions(m_pContext, *tNavMesh.compactHeightField, 0, tNavMesh.config.minRegionArea,
        tNavMesh.config.mergeRegionArea);
    assert(processResult == true);

    // 윤곽선 만들기
    rcContourSet* contourSet{ rcAllocContourSet() };
    assert(contourSet != nullptr);

    processResult = rcBuildContours(m_pContext, *tNavMesh.compactHeightField, tNavMesh.config.maxSimplificationError,
        tNavMesh.config.maxEdgeLen, *contourSet);
    assert(processResult == true);

    // 윤곽선으로부터 폴리곤 생성
    tNavMesh.polyMesh = rcAllocPolyMesh();
    assert(tNavMesh.polyMesh != nullptr);

    processResult = rcBuildPolyMesh(m_pContext, *contourSet, tNavMesh.config.maxVertsPerPoly, *tNavMesh.polyMesh);
    assert(processResult == true);

    // 디테일 메시 생성
    tNavMesh.polyMeshDetail = rcAllocPolyMeshDetail();
    assert(tNavMesh.polyMeshDetail != nullptr);

    processResult = rcBuildPolyMeshDetail(m_pContext, *tNavMesh.polyMesh, *tNavMesh.compactHeightField,
        tNavMesh.config.detailSampleDist, tNavMesh.config.detailSampleMaxError, *tNavMesh.polyMeshDetail);
    assert(processResult == true);

    //rcFreeCompactHeightfield(compactHeightField);
    rcFreeContourSet(contourSet);

    // detour 데이터 생성
    unsigned char* navData{ nullptr };
    int navDataSize{ 0 };

    assert(tNavMesh.config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON);

    // Update poly flags from areas.
    for (int i = 0; i < tNavMesh.polyMesh->npolys; ++i)
    {
        if (tNavMesh.polyMesh->areas[i] == RC_WALKABLE_AREA)
        {
            tNavMesh.polyMesh->areas[i] = 0;
            tNavMesh.polyMesh->flags[i] = 1;
        }
    }
    dtNavMeshCreateParams params;
    memset(&params, 0, sizeof(params));
    params.verts = tNavMesh.polyMesh->verts;
    params.vertCount = tNavMesh.polyMesh->nverts;
    params.polys = tNavMesh.polyMesh->polys;
    params.polyAreas = tNavMesh.polyMesh->areas;
    params.polyFlags = tNavMesh.polyMesh->flags;
    params.polyCount = tNavMesh.polyMesh->npolys;
    params.nvp = tNavMesh.polyMesh->nvp;
    params.detailMeshes = tNavMesh.polyMeshDetail->meshes;
    params.detailVerts = tNavMesh.polyMeshDetail->verts;
    params.detailVertsCount = tNavMesh.polyMeshDetail->nverts;
    params.detailTris = tNavMesh.polyMeshDetail->tris;
    params.detailTriCount = tNavMesh.polyMeshDetail->ntris;
    params.offMeshConVerts = 0;
    params.offMeshConRad = 0;
    params.offMeshConDir = 0;
    params.offMeshConAreas = 0;
    params.offMeshConFlags = 0;
    params.offMeshConUserID = 0;
    params.offMeshConCount = 0;
    params.walkableHeight = tNavMesh.config.walkableHeight;
    params.walkableRadius = tNavMesh.config.walkableRadius;
    params.walkableClimb = tNavMesh.config.walkableClimb;
    rcVcopy(params.bmin, tNavMesh.polyMesh->bmin);
    rcVcopy(params.bmax, tNavMesh.polyMesh->bmax);
    params.cs = tNavMesh.config.cs;
    params.ch = tNavMesh.config.ch;
    params.buildBvTree = true;

    processResult = dtCreateNavMeshData(&params, &navData, &navDataSize);
    assert(processResult == true);


    tNavMesh.navMesh = dtAllocNavMesh();
    assert(tNavMesh.navMesh != nullptr);

    dtStatus status;
    status = tNavMesh.navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
    //dtFree(navData);
    assert(dtStatusFailed(status) == false);
    
    tNavMesh.navQuery = dtAllocNavMeshQuery();
    status = tNavMesh.navQuery->init(tNavMesh.navMesh, 2048);

    assert(dtStatusFailed(status) == false);

    tNavMesh.crowd = dtAllocCrowd();
    tNavMesh.crowd->init(1024, buildSettings.maxAgentRadius, tNavMesh.navMesh);

    m_mapNavMesh.insert(make_pair(buildSettings.Key, tNavMesh));
}

bool CNavMeshMgr::LoadNavMeshFromFile(const char* path)
{
    return false;
}


void CNavMeshMgr::init()
{
    CreatePlane({ 1000, 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
    CreatePlane({ 3000 , 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
    CreatePlane({ 1700 , 0, 5500 }, Vec3(1000.f, 0.f, 7000.f));

    //if (!navMesh && !navQuery)
    //    LoadNavMeshFromFile("navmesh\\solo_navmesh.bin");
}

void CNavMeshMgr::tick()
{

}

void CNavMeshMgr::render()
{

}

void CNavMeshMgr::ReBuildField()
{
    free();  //이전 메쉬 메모리 해제

    m_vecWorldVertices.clear();
    m_vecWorldFaces.clear();

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
    const vector<CGameObject*>& vecPlane = pCurLevel->GetLayer((int)LAYER_TYPE::NavMeshPlane)->GetParentObject();
    const vector<CGameObject*>& vecMonster = pCurLevel->GetLayer((int)LAYER_TYPE::Monster)->GetParentObject();

    for (int i = 0; i < vecPlane.size(); ++i)
    {
        CNavMeshPlane* pMeshPlane = (CNavMeshPlane*)vecPlane[i]->GetComponent(COMPONENT_TYPE::NAVMESHPLANE);

        const vector<int>& vecFaces = pMeshPlane->GetWorldFaces();
        const vector<Vec3>& vecVertices = pMeshPlane->GetWorldVertices();

        for (int j = 0; j < 4; ++j)
            m_vecWorldVertices.push_back(vecVertices[j]);
        for (int j = 0; j < 6; ++j)
            m_vecWorldFaces.push_back(vecFaces[j]);
    }

    for (int i = 0; i < vecMonster.size(); ++i)
    {
        if (!vecMonster[i]->RDNavMeshField())
            continue;

        const wstring& strKey = vecMonster[i]->GetName(); 
        tBuildSettings buildSettings = {};
        buildSettings.Key = strKey;
        buildSettings.agentRadius = vecMonster[i]->RDNavMeshField()->GetRadius();

        BuildField(reinterpret_cast<float*>(&m_vecWorldVertices[0]), m_vecWorldVertices.size(),
                &m_vecWorldFaces[0], m_vecWorldFaces.size() / 3, buildSettings);
    }

}

void CNavMeshMgr::AddPlaneVertex(CNavMeshPlane* _pNavMeshPlane)
{
    CGameObject* pPlane = _pNavMeshPlane->GetOwner();

    //1006 102 367
    Vec3 vTemScale = pPlane->Transform()->GetRelativeScale() / 2.f;
    Vec3 vBotleft  = pPlane->Transform()->GetRelativePos() - vTemScale;
    Vec3 vTopRight = pPlane->Transform()->GetRelativePos() + vTemScale;

    //mgr에 등록하지 않고 객체에만 내 인덱스 정보를 저장했기 때문에 따로 올려주기
    m_iStartingIdx += 4;

    // 1 ----4
    // |     |
    // 2-----3
    Vec3 vPoint1 = Vec3(vBotleft.x, vTopRight.y, vTopRight.z);
    Vec3 vPoint2 = Vec3(vBotleft.x, vBotleft.y, vBotleft.z);
    Vec3 vPoint3 = Vec3(vTopRight.x, vBotleft.y, vBotleft.z);
    Vec3 vPoint4 = Vec3(vTopRight.x, vTopRight.y, vTopRight.z);

    Vec3 v1 = vPoint2 - vPoint3;
    Vec3 v2 = vPoint1 - vPoint4;
    
    //수직방향으로 그려진 평면인지 확인
    bool bVertical = v1 == Vec3::Zero || v2 == Vec3::Zero;

    if (bVertical)//_bHorizontal
    {
        _pNavMeshPlane->SetWorldVertex(Vec3(vBotleft.x, vTopRight.y, vTopRight.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vBotleft.x, vBotleft.y, vTopRight.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vTopRight.x, vBotleft.y, vBotleft.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vTopRight.x, vTopRight.y, vBotleft.z));
    }
    else
    {
        _pNavMeshPlane->SetWorldVertex(Vec3(vBotleft.x, vTopRight.y, vTopRight.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vBotleft.x, vBotleft.y, vBotleft.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vTopRight.x, vBotleft.y, vBotleft.z));
        _pNavMeshPlane->SetWorldVertex(Vec3(vTopRight.x, vTopRight.y, vTopRight.z));
    }

    _pNavMeshPlane->SetWorldFaces(2 + m_iStartingIdx);
    _pNavMeshPlane->SetWorldFaces(1 + m_iStartingIdx);
    _pNavMeshPlane->SetWorldFaces(0 + m_iStartingIdx);
    _pNavMeshPlane->SetWorldFaces(3 + m_iStartingIdx);
    _pNavMeshPlane->SetWorldFaces(2 + m_iStartingIdx);
    _pNavMeshPlane->SetWorldFaces(0 + m_iStartingIdx);
}

void CNavMeshMgr::AddNavMeshField(UINT _ID, CRDNavMeshField* _pNavMeshField)
{
    m_mapNavMeshField.insert(make_pair(_ID, _pNavMeshField));
}

void CNavMeshMgr::DeleteNavMeshField(UINT _ID)
{
    m_vecDeleteExpected.push_back(_ID);
}

//다른 스레드에서 계산
void CNavMeshMgr::CalculatePath()
{
    //삭제 예정
    for (int i = 0; i < m_vecDeleteExpected.size(); ++i)
    {
        m_mapNavMeshField.find(m_vecDeleteExpected[i])->second->SetPathDir(Vec3::Zero);
        m_mapNavMeshField.erase(m_vecDeleteExpected[i]);
    }
    m_vecDeleteExpected.clear();

    for (const auto& iter : m_mapNavMeshField)
    {
        CRDNavMeshField* pNavMeshCom = iter.second;
        if (!pNavMeshCom->m_bActive)
            return;

        const wstring& _strName = pNavMeshCom->GetOwner()->GetName();//해당 몬스터에 맞는 네비 메쉬
   
        Vec3 vTargetPos = pNavMeshCom->m_pTarget->PxRigidbody()->GetPxPosition();
        Vec3 vPos = pNavMeshCom->GetOwner()->PxRigidbody()->GetPxPosition();
        float fStart[3] = { vPos.x, vPos.y,vPos.z };
        float fEnd[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };

        pNavMeshCom->SetPathDir(FindPath(_strName, fStart, fEnd));
    }
}

Vec3 CNavMeshMgr::FindPath(const wstring& _strKey, float* _pStartPos, float* _pEndPos)
{
    float polyPickExt[3] = { 2000.f, 2000.f, 2000.f }; // 적절한 탐색 범위 설정

    
    map<wstring, tNavMeshInfo>::iterator iter = m_mapNavMesh.find(_strKey);
    if (iter == m_mapNavMesh.end())
        return Vec3::Zero;

    const tNavMeshInfo& tNav = iter->second;
    const dtQueryFilter* filter = tNav.crowd->getFilter(0);

    dtPolyRef startPoly, endPoly;
    float nearestStartPoint[3], nearestEndPoint[3];

    // 시작점과 끝점에서 가장 가까운 네비게이션 메시 폴리곤을 찾음
    tNav.navQuery->findNearestPoly(_pStartPos, polyPickExt, filter, &startPoly, nearestStartPoint);
    tNav.navQuery->findNearestPoly(_pEndPos, polyPickExt, filter, &endPoly, nearestEndPoint);

    // 실제 네비 메시 위의 좌표를 찾음
    float nearestStartPos[3], nearestEndPos[3];
    dtStatus status01 = tNav.navQuery->closestPointOnPoly(startPoly, _pStartPos, nearestStartPos, 0);
    dtStatus status02 = tNav.navQuery->closestPointOnPoly(endPoly, _pEndPos, nearestEndPos, 0);

    dtPolyRef path[128];
    int pathCount;
    tNav.navQuery->findPath(startPoly, endPoly, nearestStartPos, nearestEndPos, filter, path, &pathCount, 128);

    float* actualPath = new float[3 * 256];
    unsigned char pathFlags[256];
    dtPolyRef pathPolys[256];
    int actualPathCount;

    tNav.navQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, pathFlags, pathPolys, &actualPathCount, 256);

    // Vec3 형태의 경로를 생성
    vector<Vec3> vecPath(actualPathCount);
    for (int i = 0; i < actualPathCount; ++i)
    {
        vecPath[i] = Vec3(actualPath[3 * i], actualPath[3 * i + 1], actualPath[3 * i + 2]);
    }

    delete[] actualPath; // 항상 실행되도록 이동

    if (vecPath.size() < 2)
    {
        return Vec3::Zero;
    }

    Vec3 vDir = Vec3(vecPath[1].x - vecPath[0].x, vecPath[1].y - vecPath[0].y, vecPath[1].z - vecPath[0].z);
    vDir.Normalize();

    return vDir;

}


bool CNavMeshMgr::IsValidPoint(const wstring& _strKey, const Vec3& _CheckPos)
{
    map<wstring, tNavMeshInfo>::iterator iter = m_mapNavMesh.find(_strKey);
    if (iter == m_mapNavMesh.end())
    {
        return Vec3::Zero;
    }

    tNavMeshInfo tNav = iter->second;

    float checkpos[3] = { _CheckPos.x, _CheckPos.y, -_CheckPos.z }; // 검사 위치

    dtPolyRef checkRef;
    float polyPickExt[3] = { 2000,2000,2000 }; // 범위를 제한하기 위한 벡터

    dtQueryFilter filter;
    filter.setIncludeFlags(0xFFFF); // 모든 폴리곤 참조
    filter.setExcludeFlags(0);      // 제외할 폴리곤 없음

    // 가까운 폴리곤 검색
    dtStatus status = tNav.navQuery->findNearestPoly(checkpos, polyPickExt, &filter, &checkRef, 0);

    // 시작과 끝 위치를 찾습니다.
    float nearestPos[3];
    status = tNav.navQuery->closestPointOnPoly(checkRef, checkpos, nearestPos, 0);

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

void CNavMeshMgr::free()
{
    map<wstring, tNavMeshInfo>::iterator iter = m_mapNavMesh.begin();
    for (iter; iter != m_mapNavMesh.end(); ++iter)
    {
        tNavMeshInfo tNav = iter->second;

        if (tNav.navMesh)
            dtFreeNavMesh(tNav.navMesh);

        if (tNav.polyMesh)
            rcFreePolyMesh(tNav.polyMesh);

        if (tNav.navQuery)
            dtFreeNavMeshQuery(tNav.navQuery);

        if (tNav.polyMeshDetail)
            rcFreePolyMeshDetail(tNav.polyMeshDetail);

        if (tNav.heightField)
            rcFreeHeightField(tNav.heightField);

        if (tNav.compactHeightField)
            rcFreeCompactHeightfield(tNav.compactHeightField);

        if (tNav.crowd)
            dtFreeCrowd(tNav.crowd);
    }

    m_mapNavMesh.clear();
}