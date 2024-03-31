#include "pch.h"
#include "CRDNavMesh.h"

#include "CPxRigidbody.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CCollider3D.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CResMgr.h"

CRDNavMesh::CRDNavMesh():
	CComponent(COMPONENT_TYPE::RDNAVMESH),
    m_bActive(false)
{
	navQuery = dtAllocNavMeshQuery();
	crowd = dtAllocCrowd();
	context = new rcContext();
}

CRDNavMesh::~CRDNavMesh()
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

    if (crowd)
        dtFreeCrowd(crowd);

}


void CRDNavMesh::begin()
{
	m_pTarget =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
}

void CRDNavMesh::RecastCleanup()
{
	
}

void CRDNavMesh::BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings)
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
    rcHeightfield* heightField{ rcAllocHeightfield() };
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
    rcCompactHeightfield* compactHeightField{ rcAllocCompactHeightfield() };
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
    rcPolyMesh* polyMesh = rcAllocPolyMesh();
    assert(polyMesh != nullptr);

    processResult = rcBuildPolyMesh(context, *contourSet, config.maxVertsPerPoly, *polyMesh);
    assert(processResult == true);

    // 디테일 메시 생성
    rcPolyMeshDetail* detailMesh = rcAllocPolyMeshDetail();
    assert(detailMesh != nullptr);

    processResult = rcBuildPolyMeshDetail(context, *polyMesh, *compactHeightField, config.detailSampleDist, config.detailSampleMaxError, *detailMesh);
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
    params.detailMeshes = detailMesh->meshes;
    params.detailVerts = detailMesh->verts;
    params.detailVertsCount = detailMesh->nverts;
    params.detailTris = detailMesh->tris;
    params.detailTriCount = detailMesh->ntris;
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


int CRDNavMesh::FindPath(float* pStartPos, float* pEndPos)
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
    
    //dtPoly 

    //navQuery->closestPointOnPoly()
    //const dtPoly* poly = NULL;
    //navQuery->getPolyByRef(path[i], &poly);
    //const float* polyCenter = poly->centroid;
    const dtPolyRef polyRef = path[1];
    float closestPoint[3];
    navQuery->closestPointOnPoly(polyRef, pStartPos, closestPoint, NULL);

    //crowd->requestMoveTarget(0, endPoly, pEndPos);
   
    int a = 10;
  
	return 0;
}

void CRDNavMesh::CreatePlane(Vec3 botleft, Vec3 topright, vector<Vec3>& worldVertices, vector<int>& worldFaces)
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


void CRDNavMesh::finaltick()
{
    if (!m_pTarget)
        return;
    if (!m_bActive)
        return;

    Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
    Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

    float fStart[3] = { vPos.x, vPos.y, vPos.z };
    float fEnd[3] = { vTargetPos.x, vTargetPos.y, vTargetPos.z };

    FindPath(fStart, fEnd);
}


void CRDNavMesh::SaveToLevelFile(FILE* _File)
{
}

void CRDNavMesh::LoadFromLevelFile(FILE* _FILE)
{
}

