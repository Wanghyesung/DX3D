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

struct tNode
{
	int x, z;//��ǥ
	int iCurCos, iGoalCos;//���ۺ��� ������� ���, ������� ������������ ���� ��� 
	pair<int, int> pParentNode;//�������� ���� ���� ���
};

//���⼭ ��ġ ������ ���� collider�� ����ϴ�
class CNavMeshMgr :
	public CSingleton<CNavMeshMgr>
{
	SINGLE(CNavMeshMgr);

private:
	//���� �� ��
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//���� ��

	//���͸��� ���Ž���� �ʿ��� �ð�
	map<int, tNavInfo> m_mapNavInfo;

	Vec3 m_vLandScapeLen;//�� �� ��ü ����
	int  m_iDivideX; //���� ���� ��
	int  m_iDivideZ;
private:
	void init_target(int _ID, const Vec3& _vStartPos , const Vec3& _vGoalPos);
	void astar(int _ID, pair<int, int> start, pair<int, int> goal);
public:
	void tick();

	//���� �ʿ� ���� �ʺ��� �ʱ�ȭ (������, ������ ..)
	void Init_Map();
	void Init_CloseMap();

	void AddMonster(CGameObject* pGameObj, float _fSearchTime);
	void DeleteMonster(CGameObject* pGameObj);

	void Tracking(const Vec3& _vStartPos, const Vec3& _vGoalPos);
	void TrackingPlayer(CGameObject* _pStartObj);

	
	Vec3 GetTargetPath(CGameObject* _pObj);
	
};

