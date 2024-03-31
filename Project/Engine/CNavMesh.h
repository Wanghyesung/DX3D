#pragma once
#include "CComponent.h"

struct tNode
{
	int x, z;//��ǥ
	int iCurCos, iGoalCos;//���ۺ��� ������� ���, ������� ������������ ���� ��� 
	pair<int, int> pParentNode;//�������� ���� ���� ���
};


class CNavMesh : public CComponent
{
private:
	float m_fCurTime;
	float m_fTraceTime;

	//���� �� ��
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//���� ��

	Vec3 m_vOwnerScale; //���� ũ�⸸ŭ �� ������
	Vec3 m_vLandScapeLen;//�� �� ��ü ����

	Vec3 m_vDivede;
	int  m_iDivideX; //�� ��ü ��
	int  m_iDivideZ;

	
	//�� ����
	Vec3 m_vSearchDir;

	bool m_bActive;

private:
	//void init_target(int _ID);
	void astar(pair<int, int> _tStart, pair<int, int> _tGoal);
	bool aabb(const Vec3& _vGoalNode, const Vec3& _vCurNode);

	void init_map();
	void init_closemap();

	void tracking_player();

public:
	virtual void begin()override;
	virtual void finaltick() override;

	void SetActive(bool _bActive) { m_bActive = _bActive; }
	bool IsActive() { return m_bActive; }

	void Initialize();

	CLONE(CNavMesh);

	Vec3 GetTargetPath() { return m_vSearchDir; }

public:
	virtual void SaveToLevelFile(FILE* _File)override;
	virtual void LoadFromLevelFile(FILE* _FILE)override;

public:
	CNavMesh();
	virtual ~CNavMesh();
};

