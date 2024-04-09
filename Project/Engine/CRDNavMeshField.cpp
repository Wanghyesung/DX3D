#include "pch.h"
#include "CRDNavMeshField.h"

#include "CPxRigidbody.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CCollider3D.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CNavMeshPlane.h"

#include "DetourTileCacheBuilder.h"

UINT CRDNavMeshField::m_iPlaneCount = 0;
CRDNavMeshField::CRDNavMeshField() :
    CComponent(COMPONENT_TYPE::RDNAVMESHFIELD),
    m_bActive(false),
    m_fCurTime(0.1f),
    m_fSearchTime(0.1f),
    m_worldVertices{},
    m_worldFaces{}
{
	navQuery = dtAllocNavMeshQuery();
	crowd = dtAllocCrowd();
	context = new rcContext();
}

CRDNavMeshField::~CRDNavMeshField()
{
	if (navMesh)
		dtFreeNavMesh(navMesh);

    if (polyMesh)
        rcFreePolyMesh(polyMesh);

	if (navQuery)
		dtFreeNavMeshQuery(navQuery);

    if (polyMeshDetail)
        rcFreePolyMeshDetail(polyMeshDetail);
		
	if (context)
		delete context;
    
    if (heightField)
        delete heightField;

    if (compactHeightField)
        delete compactHeightField;
    
    if (crowd)
        dtFreeCrowd(crowd);
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

void CRDNavMeshField::begin()
{
	m_pTarget =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
}

void CRDNavMeshField::RecastCleanup()
{
	
}



void CRDNavMeshField::BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const tBuildSettings& buildSettings)
{
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
    memset(&config, 0, sizeof(rcConfig));

    config.cs = buildSettings.divisionSizeXZ;
    config.ch = buildSettings.divisionSizeY;
    config.walkableSlopeAngle = buildSettings.walkableSlopeAngle;
    config.walkableHeight = (int)ceilf(buildSettings.walkableHeight / config.ch);
    config.walkableClimb = (int)floorf(buildSettings.walkableClimb / config.ch);
    config.walkableRadius = (int)ceilf(config.cs * 2 / config.cs);
    config.maxEdgeLen = (int)(config.cs * 40 / config.cs);
    config.maxSimplificationError = 1.3f;
    config.minRegionArea = (int)rcSqr(config.cs * 27);		// Note: area = size*size
    config.mergeRegionArea = (int)rcSqr(config.cs * 67);	// Note: area = size*size
    config.maxVertsPerPoly = (int)6;
    config.detailSampleDist = 6.0f < 0.9f ? 0 : config.cs * 6.0f;
    config.detailSampleMaxError = config.ch * 1;

    rcVcopy(config.bmin, bmin);
    rcVcopy(config.bmax, bmax);
    rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);

    // 작업 맥락을 저장할 context 객체 생성, 작업의 성패여부를 저장할 processResult 선언
    //context = 
    bool processResult{ false };
    // 복셀 높이필드 공간 할당
    heightField = rcAllocHeightfield();
    assert(heightField != nullptr);

    processResult = rcCreateHeightfield(context, *heightField, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch);
    assert(processResult == true);


    std::vector<unsigned char> triareas;
    triareas.resize(facesNum);
    //unsigned char * triareas = new unsigned char[facesNum];
    //memset(triareas, 0, facesNum*sizeof(unsigned char));

    rcMarkWalkableTriangles(context, config.walkableSlopeAngle, worldVertices, verticesNum, faces, facesNum, triareas.data());
    processResult = rcRasterizeTriangles(context, worldVertices, verticesNum, faces, triareas.data(), facesNum, *heightField, config.walkableClimb);
    assert(processResult == true);

    // 필요없는 부분 필터링
    rcFilterLowHangingWalkableObstacles(context, config.walkableClimb, *heightField);
    rcFilterLedgeSpans(context, config.walkableHeight, config.walkableClimb, *heightField);
    rcFilterWalkableLowHeightSpans(context, config.walkableHeight, *heightField);

    // 밀집 높이 필드 만들기
    compactHeightField = rcAllocCompactHeightfield();
    assert(compactHeightField != nullptr);


    processResult = rcBuildCompactHeightfield(context, config.walkableHeight, config.walkableClimb, *heightField, *compactHeightField);
    //rcFreeHeightField(heightField);
    assert(processResult == true);

    //processResult = rcErodeWalkableArea(context, config.walkableRadius, *compactHeightField);
    //assert(processResult == true);

    processResult = rcBuildDistanceField(context, *compactHeightField);
    assert(processResult == true);

    rcBuildRegions(context, *compactHeightField, 0, config.minRegionArea, config.mergeRegionArea);
    assert(processResult == true);

    // 윤곽선 만들기
    rcContourSet* contourSet{ rcAllocContourSet() };
    assert(contourSet != nullptr);

    processResult = rcBuildContours(context, *compactHeightField, config.maxSimplificationError, config.maxEdgeLen, *contourSet);
    assert(processResult == true);

    // 윤곽선으로부터 폴리곤 생성
    polyMesh = rcAllocPolyMesh();
    assert(polyMesh != nullptr);

    processResult = rcBuildPolyMesh(context, *contourSet, config.maxVertsPerPoly, *polyMesh);
    assert(processResult == true);

    // 디테일 메시 생성
    polyMeshDetail = rcAllocPolyMeshDetail();
    assert(polyMeshDetail != nullptr);

    processResult = rcBuildPolyMeshDetail(context, *polyMesh, *compactHeightField, config.detailSampleDist, config.detailSampleMaxError, *polyMeshDetail);
    assert(processResult == true);

    //rcFreeCompactHeightfield(compactHeightField);
    rcFreeContourSet(contourSet);

    // detour 데이터 생성
    unsigned char* navData{ nullptr };
    int navDataSize{ 0 };

    assert(config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON);

    // Update poly flags from areas.
    for (int i = 0; i < polyMesh->npolys; ++i)
    {
        if (polyMesh->areas[i] == RC_WALKABLE_AREA)
        {
            polyMesh->areas[i] = 0;
            polyMesh->flags[i] = 1;
        }
    }
    dtNavMeshCreateParams params;
    memset(&params, 0, sizeof(params));
    params.verts = polyMesh->verts;
    params.vertCount = polyMesh->nverts;
    params.polys = polyMesh->polys;
    params.polyAreas = polyMesh->areas;
    params.polyFlags = polyMesh->flags;
    params.polyCount = polyMesh->npolys;
    params.nvp = polyMesh->nvp;
    params.detailMeshes = polyMeshDetail->meshes;
    params.detailVerts = polyMeshDetail->verts;
    params.detailVertsCount = polyMeshDetail->nverts;
    params.detailTris = polyMeshDetail->tris;
    params.detailTriCount = polyMeshDetail->ntris;
    params.offMeshConVerts = 0;
    params.offMeshConRad = 0;
    params.offMeshConDir = 0;
    params.offMeshConAreas = 0;
    params.offMeshConFlags = 0;
    params.offMeshConUserID = 0;
    params.offMeshConCount = 0;
    params.walkableHeight = config.walkableHeight;
    params.walkableRadius = config.walkableRadius;
    params.walkableClimb = config.walkableClimb;
    rcVcopy(params.bmin, polyMesh->bmin);
    rcVcopy(params.bmax, polyMesh->bmax);
    params.cs = config.cs;
    params.ch = config.ch;
    params.buildBvTree = true;

    processResult = dtCreateNavMeshData(&params, &navData, &navDataSize);
    assert(processResult == true);


    navMesh = dtAllocNavMesh();
    assert(navMesh != nullptr);

    dtStatus status;
    status = navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
    //dtFree(navData);
    assert(dtStatusFailed(status) == false);

    navQuery = dtAllocNavMeshQuery();
    status = navQuery->init(navMesh, 2048);
    
    assert(dtStatusFailed(status) == false);

    crowd->init(1024, buildSettings.maxAgentRadius, navMesh);
}



int CRDNavMeshField::FindPath(float* pStartPos, float* pEndPos)
{
   
    /*Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
    float fTargetPos[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };

    const dtQueryFilter* filter_ = crowd->getFilter(0);
    const dtCrowdAgent* agent = crowd->getAgent(0);
   
    const float* halfExtents = crowd->getQueryExtents();

    dtPolyRef targetRef = {};
    navQuery->findNearestPoly(fTargetPos, halfExtents, filter_, &targetRef, vTargetPos);
    crowd->requestMoveTarget(0, targetRef, vTargetPos);
    crowd->update()*/

    ////
    const Vec3 vScale = GetOwner()->Collider3D()->GetOffsetScale();
    float ext[3] = { vScale.x, vScale.y, vScale.z };

    const dtQueryFilter* filter = crowd->getFilter(0);

    dtPolyRef startPoly;
    float nearestPoint[3];
    navQuery->findNearestPoly(pStartPos, ext, filter, &startPoly, nearestPoint);

    const Vec3 vTargetScale = m_pTarget->Collider3D()->GetOffsetScale();
    float targetext[3] = { vTargetScale.x, vTargetScale.y, vTargetScale.z};

    dtPolyRef endPoly;
    navQuery->findNearestPoly(pEndPos, targetext, filter, &endPoly, nearestPoint);

    // 시작과 끝 위치를 찾습니다.
    float nearestStartPos[3], nearestEndPos[3];
    dtStatus status01 = navQuery->closestPointOnPoly(startPoly, pStartPos, nearestStartPos, 0);
    dtStatus status02 = navQuery->closestPointOnPoly(endPoly, pEndPos, nearestEndPos, 0);

    dtPolyRef path[128];
    int pathCount;
    navQuery->findPath(startPoly, endPoly, pStartPos, pEndPos, filter, path, &pathCount, 128);
    
    float* actualPath = new float[3 * 256];
    int actualPathCount;
    navQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, &actualPathCount, 256);

    // Vec3 형태의 경로를 생성합니다.
    vector<Vec3> vecPath(actualPathCount);
    for (int i = 0; i < actualPathCount; ++i)
    {
        vecPath[i] = Vec3(actualPath[3 * i], actualPath[3 * i + 1], actualPath[3 * i + 2]);
    }

    delete[] actualPath; // 더이상 필요없는 calcPath를 삭제합니다.

    m_vPathDir = Vec3(vecPath[1].x - vecPath[0].x, vecPath[1].y - vecPath[0].y,
        vecPath[1].z - vecPath[0].z);
    
    m_vPathDir.Normalize();
    
    //const dtPolyRef polyRef = path[1];
    //float closestPoint[3];
    //navQuery->closestPointOnPoly(polyRef, pStartPos, closestPoint, NULL);

   //const dtPolyRef CurPloyRef = path[0];
    //float curclosestPoint[3];
    //navQuery->closestPointOnPoly(CurPloyRef, pStartPos, curclosestPoint, NULL);

    //m_vPathDir = Vec3(closestPoint[0] - curclosestPoint[0] , closestPoint[1] - curclosestPoint[1] ,
    //    closestPoint[2] - curclosestPoint[2]);
    //
    //m_vPathDir.Normalize();
	return 0;
}

void CRDNavMeshField::CreatePlane2(const Vec3& _vPos, Vec3 _vScale, bool _bHeightMesh)
{
    Vec3 vTemScale = _vScale / 2.f;
    Vec3 vBotleft = _vPos - vTemScale; //1275, 0 , 1990
    Vec3 vTopRight = _vPos + vTemScale;//1275, 700 ,8990

    int startingIdx = m_worldVertices.size();

    // 1 ----4
    // |     |
    // 2-----3
    m_worldVertices.push_back({ vBotleft.x, vTopRight.y,vTopRight.z }); //0 0 18000
    m_worldVertices.push_back({ vBotleft.x, vBotleft.y ,vBotleft.z });//0, 0, -18000
    m_worldVertices.push_back({ vTopRight.x, vBotleft.y,vBotleft.z });//18000 0 0
    m_worldVertices.push_back({ vTopRight.x, vTopRight.y ,vTopRight.z });//-18000 0 0


    m_worldFaces.push_back(startingIdx + 2);
    m_worldFaces.push_back(startingIdx + 1);
    m_worldFaces.push_back(startingIdx + 0);
    m_worldFaces.push_back(startingIdx + 3);
    m_worldFaces.push_back(startingIdx + 2);
    m_worldFaces.push_back(startingIdx + 0);

    CGameObject* pGameObj = new CGameObject();
    pGameObj->SetName(L"navMeshPlane" + std::to_wstring(m_iPlaneCount));
    pGameObj->AddComponent(new CTransform);
    
    pGameObj->Transform()->SetRelativeScale(_vScale);
   
    pGameObj->AddComponent(new CMeshRender);
    pGameObj->AddComponent(new CNavMeshPlane);
    SpawnGameObject(pGameObj, _vPos, (int)LAYER_TYPE::Default);

    ++m_iPlaneCount;


}

void CRDNavMeshField::CreatePlane(Vec3 botleft, Vec3 topright)
{
	int startingIdx = m_worldVertices.size();
	m_worldVertices.push_back({ botleft.x,0,topright.z });
	m_worldVertices.push_back({ botleft.x,0,botleft.z });
	m_worldVertices.push_back({ topright.x,0,botleft.z });
	m_worldVertices.push_back({ topright.x,0,topright.z });

	m_worldFaces.push_back(startingIdx + 2);
	m_worldFaces.push_back(startingIdx + 1);
	m_worldFaces.push_back(startingIdx + 0);
	m_worldFaces.push_back(startingIdx + 3);
	m_worldFaces.push_back(startingIdx + 2);
	m_worldFaces.push_back(startingIdx + 0);

    CGameObject* pGameObj = new CGameObject();
    pGameObj->SetName(L"navMeshPlane" + m_iPlaneCount);
    pGameObj->AddComponent(new CTransform);

    //Vec3 vScale = 
    //pGameObj->Transform()->SetRelativeScale(new Vec3())
    pGameObj->AddComponent(new CMeshRender);
    pGameObj->AddComponent(new CNavMeshPlane);
    Vec3 vPos = (botleft + topright) / 2.f;
    SpawnGameObject(pGameObj, vPos, (int)LAYER_TYPE::Default);

    ++m_iPlaneCount;
	/*auto tilePlane = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DebugTilePlane>();
	auto size = topright - botleft;
	tilePlane->GetTransform()->SetWorldPosition((botleft + topright) / 2.0);
	tilePlane->width = size.x;
	tilePlane->height = size.z;
	tilePlane->SetTiles();*/
}


void CRDNavMeshField::finaltick()
{
    if (!m_bActive)
        return;

    if (crowd == nullptr)
        return;

    crowd->update(DT, nullptr);

   m_fCurTime += DT;
   if (m_fSearchTime <= m_fCurTime)
   {
       m_fCurTime = 0.f;
   
       if (!m_pTarget)
           return;
   
       Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
       Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
   
       float fStart[3] = { vPos.x, vPos.y,vPos.z };
       float fEnd[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };
   
       FindPath(fStart, fEnd);

       int a = 10;
   }
}


void CRDNavMeshField::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMeshField::LoadFromLevelFile(FILE* _FILE)
{
}

