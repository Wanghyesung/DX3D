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

CRDNavMeshField::CRDNavMeshField():
	CComponent(COMPONENT_TYPE::RDNAVMESHFIELD),
    m_bActive(false),
    m_fCurTime(0.1f),
    m_fSearchTime(0.1f)
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
		delete polyMesh;

	if (navQuery)
		dtFreeNavMeshQuery(navQuery);

	if (polyMeshDetail)
		delete polyMeshDetail;

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

void CRDNavMeshField::BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings)
{
    float bmin[3] = { 0.f, 0.f, 0.f };//{ std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };
    float bmax[3] = { 0.f, 0.f, 0.f };//{ -std::numeric_limits<float>::max(),-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max() };
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
    //rcFreeContourSet(contourSet);

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


void CRDNavMeshField::AddObstacle(const float* pos, const float radius, const float height, dtObstacleRef* result)
{

}

int CRDNavMeshField::FindPath(float* pStartPos, float* pEndPos)
{
    
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

    dtPolyRef path[128];
    int pathCount;
    navQuery->findPath(startPoly, endPoly, pStartPos, pEndPos, filter, path, &pathCount, 128);
    
    if (pathCount)
    {
        dtPolyRef polys[128];
        memcpy(polys, path, sizeof(dtPolyRef) * pathCount);
        int npolys = pathCount;

        float iterPos[3], targetPos[3];
        navQuery->closestPointOnPoly(startPoly, pStartPos, iterPos, 0);
        navQuery->closestPointOnPoly(polys[npolys - 1], pEndPos, targetPos, 0);

        static const float STEP_SIZE = 18.f;
        static const float SLOP = 0.01f;

        m_nsmoothPath = 0;

        dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
        m_nsmoothPath++;

        float result[3] = { 0,0,0 };
        //지금부터는 반복문에 진입하여 부드러운 경로(smoothPath)를 만들어내는 구간이다.
        //getSteerTarget함수는 네비게이션 메시가 목적지까지 가기 위해 직진하다가 경로를 
        // 꺾어야(Steer) 하는 지점을 찾아준다.더 이상 경로를 꺾을 것도 없이 직진만 해도 
        // 목표지점에 도달할 수 있다면 endOfPath가 참 값이 된다.
         // 만약 경로를 꺾어야 하는 지점이 네비게이션 메시 바깥에 존재한다면 
         // offMeshConnection이 참 값이 된다.

        while (npolys && m_nsmoothPath < MAX_SMOOTH)
        {
            float steerPos[3];
            unsigned char steerPosFlag;
            dtPolyRef steerPosRef;

            if (!getSteerTarget(navQuery, iterPos, targetPos, SLOP,
                polys, npolys, steerPos, steerPosFlag, steerPosRef))
                break;

            bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
            bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

            float delta[3], len;
            dtVsub(delta, steerPos, iterPos);
            len = dtMathSqrtf(dtVdot(delta, delta));
            // If the steer target is end of path or off-mesh link, do not move past the location.
            if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
                len = 1;
            else
                len = STEP_SIZE / len; //2.f / 900.f
            float moveTgt[3];
            dtVmad(moveTgt, iterPos, delta, len);

            //이제 네비게이션 쿼리의 moveAlongSurface 함수를 호출하면 목표위치로 이동을 시도하고, 
            // 이동이 끝난 좌표를 result에 저장한다. 길을 찾아 진행하는 과정에서 어떤 네비게이션 폴리곤들을 
            // 탐색하였는지에 대한 결과를 visitied,nvisited 변수로 받아온다. 
            // fixupShortcuts 함수는 탐색된 경로에 유턴이 없는지 확인하고 수정한다

            //move
            result[3];
            dtPolyRef visited[16];
            int nvisited = 0;
            navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
                result, visited, &nvisited, 16);


            npolys = dtMergeCorridorStartMoved(polys, npolys,
                                               MAX_POLYS, visited, nvisited);
            npolys = fixupShortcuts(polys, npolys, navQuery);


            float h = 0;
            navQuery->getPolyHeight(polys[0], result, &h);
            result[1] = h;
            dtVcopy(iterPos, result);


            int a = 10;
        }

            
        m_vPathDir = Vec3(result[0] - pStartPos[0], result[1] - pStartPos[1],
            result[2] - pStartPos[2]);

        m_vPathDir.Normalize();
    }
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

void CRDNavMeshField::CreatePlane(Vec3 botleft, Vec3 topright, vector<Vec3>& worldVertices, vector<int>& worldFaces)
{
	int startingIdx = worldVertices.size();
	worldVertices.push_back({ botleft.x,0,topright.z });
	worldVertices.push_back({ botleft.x,0,botleft.z });
	worldVertices.push_back({ topright.x,0,botleft.z });
	worldVertices.push_back({ topright.x,0,topright.z });

	worldFaces.push_back(startingIdx + 2);
	worldFaces.push_back(startingIdx + 1);
	worldFaces.push_back(startingIdx + 0);
	worldFaces.push_back(startingIdx + 3);
	worldFaces.push_back(startingIdx + 2);
	worldFaces.push_back(startingIdx + 0);

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
   
       float fStart[3] = { vPos.x, vPos.y, vPos.z };
       float fEnd[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };
   
       FindPath(fStart, fEnd);
   }
}


void CRDNavMeshField::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMeshField::LoadFromLevelFile(FILE* _FILE)
{
}

