#pragma once
#include "CComponent.h"

class CNavMeshPlane : public CComponent
{
private:
	vector<Vec3> m_vecWorldVertices;
	vector<int> m_vecWorldFaces;
	vector<CGameObject*> m_vecTargetPlane;//���� �������� �÷� ������Ʈ

	bool m_bActiveRay;//���콺��ŷ ����
	bool m_bActiveCreate;//��ֹ� ������Ʈ�� ��������
	bool m_bObstaclePlane; //�ش� ��ü�� ��ֹ� ������Ʈ����

	Vec3 m_vRayPoint;
	Vec3 m_vCreateScale;

	static UINT m_iObstacleCount;
private:
	void CreateObstacle();
	bool RayCasting();
	float GetArea(const Vec3& _vP1, const Vec3& _vP2);

	void DrawCube();

	void CreateTemObstacle();
	void ReBulid();

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

