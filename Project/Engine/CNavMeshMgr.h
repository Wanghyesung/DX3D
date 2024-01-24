#pragma once
#include "CSingleton.h"

struct tNavInfo
{
	int iID;

	float fCurTime;
	float fEndTime;

	Vec3 vCurPos;
	Vec3 vGoalPos;

	//�� ����
	Vec3 vSearchFoce;

	//Ž���ʿ�ð��� �Ǿ�����
	bool bSearchOn;

};

class CNavMeshMgr :
	public CSingleton<CNavMeshMgr>
{
	SINGLE(CNavMeshMgr);

private:
	//���� �� ��
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//���� ��

	//���͸��� ���Ž���� �ʿ��� �ð�
	map<int, tNavInfo> m_mapInitTime;

private:
	void add_monster(CGameObject* pGameObj, float _fSearchTime);

	void astar();
public:
	void tick();

	//���� �ʿ� ���� �ʺ��� �ʱ�ȭ (������, ������ ..)
	void Init_Map();
	void Init_Close();

	//���� �� ��ġ���� ������������ �ִܰ��
	const vector<std::pair<int, int>>& GetTargetPath(int _iID, const Vec3& _vecStartPos);
	
};

