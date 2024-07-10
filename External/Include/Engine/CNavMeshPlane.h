#pragma once
#include "CComponent.h"

class CNavMeshPlane : public CComponent
{
private:
	vector<Vec3> m_vecWorldVertices;
	vector<int> m_vecWorldFaces;

	bool m_bActiveRay;//마우스피킹 성공
	bool m_bActiveCreate;//장애물 오브젝트를 생성할지

	Vec3 m_vRayPoint;
	Vec3 m_vCreateScale;

	static bool m_bChanage;
private:
	void CreateObstacle();
	bool RayCasting();
	float GetArea(const Vec3& _vP1, const Vec3& _vP2);

	void DrawCube();

public:
	void SetWorldVertex(const Vec3& _vecVertex) { m_vecWorldVertices.push_back(_vecVertex); }
	void SetWorldFaces(const int& _iFace) { m_vecWorldFaces.push_back(_iFace); }

	const vector<Vec3>& GetWorldVertices() { return m_vecWorldVertices; }
	const vector<int>& GetWorldFaces() { return m_vecWorldFaces; }
public:
	virtual void finaltick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	
	CLONE(CNavMeshPlane);

public:
	CNavMeshPlane();
	virtual ~CNavMeshPlane();
};

