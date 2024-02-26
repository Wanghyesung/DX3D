#include "pch.h"
#include "CNavMesh.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLandScape.h"
#include "CCollider3D.h"
//���� �� �Ÿ� �� + ������������ �Ÿ� 
//�� ���� ���� ��尡 ���� ���� ���� ���
struct cmp {//�켱���� ť �� �Լ�
	bool operator()(const tNode u, const tNode v) {
		if (u.iCurCos + u.iGoalCos > v.iCurCos + v.iGoalCos)
			return true;
		else if (u.iCurCos + u.iGoalCos == v.iCurCos + v.iGoalCos)
		{
			if (u.iCurCos > v.iCurCos)
				return true;
			else
				return false;
		}

		else
			return false;
	}
};

CNavMesh::CNavMesh() :
	CComponent(COMPONENT_TYPE::NAVMESH),
	m_fCurTime(0.f),
	m_fTraceTime(0.15f),
	m_vSearchDir(Vec3::Zero),
	m_vOwnerScale(Vec3::Zero),
	m_bActive(false)
{

}

CNavMesh::~CNavMesh()
{

}

void CNavMesh::begin()
{
	init_map();
}

void CNavMesh::finaltick()
{
	if (!m_bActive)
		return;

	m_fCurTime += DT;

	if (m_fCurTime >= m_fTraceTime)
	{
		m_fCurTime = 0.f;
		tracking_player();
	}
}

void CNavMesh::Initialize()
{

}

void CNavMesh::astar(pair<int, int> _tStart, pair<int, int> _tGoal)
{
	priority_queue<tNode, vector<tNode>, cmp> queueWay;	// �켱���� ť

	tNode tStartNode;									// ���� ���

	vector<tNode> vecWay;							//���� �� ��

	int DX[8] = { 1, 1,-1, -1, 0 ,1, 0, -1};				// ���� ��ǥ �٢֢עء���� = �ð��������
	int DZ[8] = { 1,-1, 1, -1, -1 ,0, 1,  0};

	// �������� �ʱ�ȭ
	tStartNode.x = _tStart.second; // x���� second
	tStartNode.z = _tStart.first; //  z���� first
	tStartNode.iCurCos = 0; //���� ����� 0

	tStartNode.iGoalCos = (abs(_tGoal.second - tStartNode.x) + abs(_tGoal.first - tStartNode.z)) * 10;
	tStartNode.pParentNode = make_pair(-1, -1);				// ���� ����� �θ� ���� -1,-1
	queueWay.push(tStartNode);
	
	while (!queueWay.empty()) //�� 
	{
		int x = queueWay.top().x;						// �켱���� ť���� top ���� ����
		int z = queueWay.top().z;
		int cost = queueWay.top().iCurCos;
		vecWay.push_back(queueWay.top());
		m_vecMap[z][x] = 8; 
		queueWay.pop();
		if (x == _tGoal.second && z == _tGoal.first) break;	

		tNode tAddNode; //�ֺ�Ž���� ���� ���
		for (int i = 0; i < 8; i++)
		{// top ��忡�� �����¿� 8�������� Ž��
			int nextX = x + DX[i];
			int nextZ = z + DZ[i];
			//������ ������ ����� �ʰ�
			if (nextX >= 0 && nextX < m_vecMap[0].size() && nextZ >= 0 && nextZ < m_vecMap.size())
			{
				//�������̰ų� ���� �� ���̸� �ٽ� ������
				if (m_vecClose[nextZ][nextX] != 1 && m_vecMap[nextZ][nextX] == false)
				{
					tAddNode.x = nextX;
					tAddNode.z = nextZ;
					tAddNode.iCurCos = i >= 4 ? cost + 10 : cost + 14;// �����¿�� 10, �밢���̸� 14(��200)
					tAddNode.iGoalCos = (abs(_tGoal.second - tAddNode.x) + abs(_tGoal.first - tAddNode.z)) * 10;
					tAddNode.pParentNode = make_pair(z, x);	

					m_vecMap[nextZ][nextX] = 9;		// Ž���� �ֺ� ��� = (���� ���� �ʾ����ϱ� 9�� ǥ��)
					queueWay.push(tAddNode);				// �켱���� ť�� ����	
				}
			}
		}
	}

	int px = vecWay.back().x;
	int pz = vecWay.back().z;
	while (vecWay.size())
	{			
		if (px == vecWay.back().x && pz == vecWay.back().z)
		{
			if (vecWay.back().pParentNode.second == _tStart.second &&
				vecWay.back().pParentNode.first == _tStart.first)
				break;

			px = vecWay.back().pParentNode.second;
			pz = vecWay.back().pParentNode.first;
		}
		vecWay.pop_back(); //���� �ô� ��
	}

	Vec3 vDir;
	if (vecWay.size() <= 1)
		vDir = Vec3::Zero;
	else
		vDir = Vec3(px, 0.f, pz) - Vec3(vecWay[0].x, 0.f, vecWay[0].z);

	vDir.Normalize();
	m_vSearchDir = vDir;
}

void CNavMesh::init_map()
{
	const vector<CGameObject*>& vecLand =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::LandScape)->GetParentObject();

	if (vecLand.size() == 0)
		return;
	//�ϴ� ���� ó�� ������ Ž��
	//���� ���� 
	CGameObject* pScape = vecLand[0];
	Vec3 vScapePos = pScape->Collider3D()->GetWorldPos(); //9000, 9000
	Vec3 vScale = pScape->Collider3D()->GetOffsetScale(); // 18000 18000
	vScale /= 2.f;

	Vec3 vRightTop = vScapePos + vScale;
	Vec3 vLeftDown = vScapePos - vScale;
	m_vLandScapeLen = vRightTop - vLeftDown;
	//Vec3 vCapeLen 
	//�浹ü x, zũ���� 2���� �迭
	m_vOwnerScale = GetOwner()->Collider3D()->GetOffsetScale();
	//float fTem = m_vOwnerScale.z;
	//m_vOwnerScale.z = m_vOwnerScale.y;
	//m_vOwnerScale.y = fTem;

	Vec3 vDivideSize = m_vLandScapeLen / m_vOwnerScale; // 150 150������ ���Ͱ� ������ �� �ִ� ������ ���� 
	//120��
	m_iDivideX =  vDivideSize.x; //��ü ������ ���� ��
	m_iDivideZ =  vDivideSize.z;

	m_vecClose = vector<vector<int>>(m_iDivideZ, vector<int>(m_iDivideX));

	//--- ���� ������ �� ����
	init_closemap();
}

void CNavMesh::init_closemap()
{
	const vector<CGameObject*>& vecLandform =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Obstacle)->GetObjects();

	for (int i = 0; i < vecLandform.size(); ++i)
	{
		if (!vecLandform[i]->Collider3D())
			continue;

		Vec3 vformPos = vecLandform[i]->Collider3D()->GetWorldPos();

		//ũ��
		Vec3 vformScale = vecLandform[i]->Collider3D()->GetOffsetScale();
		vformScale /= 2.f;
		//�� �� �� ��
		float fPos[4] = { vformPos.x - vformScale.x, vformPos.x + vformScale.x,
						 vformPos.z - vformScale.z, vformPos.z + vformScale.z };

		//ĭ�� �����ִ� ����
		float fOffsetX = m_iDivideX /3.f;
		float fOffsetZ = m_iDivideZ /3.f;

		float fCorrectionZ = vformScale.z / 3.f;
		float fCorrectionX = vformScale.x / 3.f;

		for (float i = fPos[2] + fCorrectionZ; i <= fPos[3] - fCorrectionZ; i += fOffsetZ)//z
		{
			for (float j = fPos[0] + fCorrectionX; j <= fPos[1] - fCorrectionX; j += fOffsetX) //x
			{
				//���� �˻��ϴ� ��ġ
				Vec3 vPos = Vec3{ j, 0.f, i };

				//�ʿ����� ������ ���
				Vec3 vRatio = {}; 
				vRatio.x = vPos.x / m_vOwnerScale.x;
				vRatio.z = vPos.z / m_vOwnerScale.z;

				//Vec3 vAbsRatio = Vec3(abs(vRatio.x), abs(vRatio.y), abs(vRatio.z));

				//�ʿ����� ������ ���
				//vRatio.x /= m_iDivideX; //0.5 * 40 = 20
				//vRatio.z /= m_iDivideZ;
				//������ ��
				m_vecClose[(int)vRatio.z][(int)vRatio.x] = 1;
			}
		}
	}
}

void CNavMesh::tracking_player()
{
	if (m_iDivideZ == 0)
		return;

	m_vecMap = vector<vector<int>>(m_iDivideZ, vector<int>(m_iDivideX));

	const vector<CGameObject*>& vecPlayer =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject();

	CGameObject* pPlayer = vecPlayer[0];
	if (pPlayer->IsDead())
		return;

	
	//�÷��̾� ���� ���� ��ġ ������ ��Ƽ� �ʿ� �ֱ�
	Vec3 vPlayerPos = pPlayer->Collider3D()->GetWorldPos();

	Vec3 vRatio = {};

	vRatio.x = vPlayerPos.x / m_vOwnerScale.x;
	vRatio.z = vPlayerPos.z / m_vOwnerScale.z;

	//m_vecMap[(int)vRatio.x][(int)vRatio.z] = 5;//�������� 5
	Vec3 vGoalPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);
	// --����

	CGameObject* pMonster = GetOwner();
	Vec3 vMonsterPos = pMonster->Collider3D()->GetWorldPos();

	//vRatio = vMonsterPos / m_iDivideX;
	vRatio.x = vMonsterPos.x / m_vOwnerScale.x;
	vRatio.z = vMonsterPos.z / m_vOwnerScale.z;
	//m_vecMap[(int)vRatio.x][(int)vRatio.z] = 6;//���� ��ġ�� 6	
	Vec3 vStartPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);


	pair<int, int> tStart = make_pair(vStartPos.z, vStartPos.x);
	pair<int, int> tGoal = make_pair(vGoalPos.z, vGoalPos.x);

	//��ã��
	astar(tStart, tGoal);
}

void CNavMesh::SaveToLevelFile(FILE* _File)
{

}

void CNavMesh::LoadFromLevelFile(FILE* _FILE)
{

}

