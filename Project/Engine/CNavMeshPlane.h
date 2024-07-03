#pragma once
#include "CComponent.h"

class CNavMeshPlane : public CComponent
{
private:
	vector<Vec3> m_vecWorldVertices;

	bool m_bActiveRay;

private:
	bool RayCasting();
	float GetArea(const Vec3& _vP1, const Vec3& _vP2);
public:
	void SetWorldVertex(const Vec3& _vecVertex) { m_vecWorldVertices.push_back(_vecVertex); }

public:
	virtual void finaltick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	
	CLONE(CNavMeshPlane);

public:
	CNavMeshPlane();
	virtual ~CNavMeshPlane();
};

