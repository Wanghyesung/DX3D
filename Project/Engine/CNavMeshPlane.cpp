#include "pch.h"
#include "CNavMeshPlane.h"
#include "CTransform.h"

CNavMeshPlane::CNavMeshPlane():
	CComponent(COMPONENT_TYPE::NAVMESHPLANE)
{
}

CNavMeshPlane::~CNavMeshPlane()
{

}

void CNavMeshPlane::finaltick()
{
	Vec4 vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	const Matrix& m_tMat = GetOwner()->Transform()->GetWorldMat();

	DrawDebugRect(m_tMat,vColor,0.0f,false);
}

void CNavMeshPlane::SaveToLevelFile(FILE* _File)
{
}

void CNavMeshPlane::LoadFromLevelFile(FILE* _File)
{
}


