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
			if (u.iCurCos < v.iCurCos)
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
	m_fTraceTime(0.2f),
	m_vSearchDir(Vec3::Zero)
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

	int DX[8] = {1, 1,-1, -1, 0 ,1, 0, -1, };				// ���� ��ǥ �٢֢עء���� = �ð��������
	int DZ[8] = {1,-1, 1, -1, -1 ,0, 1,  0 , };

	// �������� �ʱ�ȭ
	tStartNode.x = _tStart.second; // x���� second
	tStartNode.z = _tStart.first; //  z���� first
	tStartNode.iCurCos = 0; //���� ����� 0

	//���⼭ 10�� 1 : 1 : ��2�� ����Ҷ� ���� ��Ÿ���� ���ؼ� 10�� ���ؼ� ǥ���� ���Դϴ�
	//�����¿� ������ ����� 10 �밢�� ����� 14
	//�� ���� ��忡�� ��ǥ������ ���� ���� ����մϴ� (x������ �󸶳� ���� z������ �󸶳� ����)
	tStartNode.iGoalCos = ((_tGoal.second - tStartNode.x) + abs(_tGoal.first - tStartNode.z)) * 10;
	tStartNode.pParentNode = make_pair(-1, -1);				// ���� ����� �θ� ���� -1,-1
	queueWay.push(tStartNode);
	//m_vecClose[tStartNode.z][tStartNode.x] = true;				// ���� �� ����(�ٽ� �� ����)

	while (!queueWay.empty()) //�� 
	{
		int x = queueWay.top().x;						// �켱���� ť���� top ���� ����
		int z = queueWay.top().z;
		int cost = queueWay.top().iCurCos;
		vecWay.push_back(queueWay.top());
		m_vecMap[z][x] = 8; //���� �� ���� ����صα� ���ؼ� 8�� ����(���� �� �� , ���� �̹� ���)
		queueWay.pop();
		if (x == _tGoal.second && z == _tGoal.first) break;	// ���� ������ ������ ��

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
					tAddNode.pParentNode = make_pair(z, x);	// ���� top��带 �θ� ���� ����
					//m_vecClose[nextZ][nextX] = true;		// ���� �� ���� true�ؼ� �ٽ� ������
					m_vecMap[nextZ][nextX] = 9;		// Ž���� �ֺ� ��� = (���� ���� �ʾ����ϱ� 9�� ǥ��)
					queueWay.push(tAddNode);				// �켱���� ť�� ����	
				}
			}
		}
	}

	if(vecWay.size() <= 1)
		m_vSearchDir = Vec3::Zero;
	else
	{
		//0�� ���� �� ��ġ 1�� ������ġ
		// �������� �ٲٱ�
		Vec3 vDir = Vec3(vecWay[1].x, 0.f, vecWay[1].z) - Vec3(vecWay[0].x, 0.f, vecWay[0].z);
		vDir.Normalize();
		m_vSearchDir = vDir;
	}
}

void CNavMesh::init_map()
{
	const vector<CGameObject*>& vecLand =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::LandScape)->GetObjects();

	if (vecLand.size() == 0)
		return;
	//�ϴ� ���� ó�� ������ Ž��
	CLandScape* pScape = vecLand[0]->LandScape();
	Matrix matScape = pScape->GetOwner()->Collider3D()->GetColliderWorldMat();
	Vec3 vScapePos = Vec3(matScape._41, matScape._42, matScape._43); //9000, 9000
	Vec3 vScale = pScape->GetOwner()->Collider3D()->GetOffsetScale(); // 18000 18000
	vScale /= 2.f;

	Vec3 vRightTop = vScapePos + vScale;
	Vec3 vLeftDown = vScapePos - vScale;
	m_vLandScapeLen = vRightTop - vLeftDown;
	//Vec3 vCapeLen 
	//�浹ü x, zũ���� 2���� �迭
	Vec3 vMonsterScale = GetOwner()->Collider3D()->GetOffsetScale();
	m_iDivideX = vMonsterScale.x; //��ü ������ ���� ��
	m_iDivideZ = vMonsterScale.z;

	m_vecClose = vector<vector<int>>(m_iDivideZ, vector<int>(m_iDivideX));

	//--- ���� ������ �� ����
	init_closemap();
}

void CNavMesh::init_closemap()
{
	const vector<CGameObject*>& vecLandform =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::landform)->GetObjects();

	for (int i = 0; i < vecLandform.size(); ++i)
	{
		if (!vecLandform[i]->Collider3D())
			continue;

		Matrix matForm = vecLandform[i]->Collider3D()->GetColliderWorldMat();
		Vec3 vformPos = Vec3(matForm._41, matForm._42, matForm._43);

		//ũ��
		Vec3 vformScale = vecLandform[i]->Collider3D()->GetOffsetScale();
		vformScale /= 2.f;
		//�� �� �� ��
		float fPos[4] = { vformPos.x - vformScale.x, vformPos.x + vformScale.x,
						 vformPos.z - vformScale.z, vformPos.z + vformScale.z };

		//ĭ�� �����ִ� ����
		float fOffsetX = m_vLandScapeLen.x / m_iDivideX;
		float fOffsetZ = m_vLandScapeLen.z / m_iDivideZ;

		for (float i = fPos[2] - vformScale.z/2.f; i <= fPos[3] + vformScale.z/2.f; i += fOffsetZ)//z
		{
			for (float j = fPos[0] - vformScale.x / 2.f; j <= fPos[1] + vformScale.x/2.f; j += fOffsetX) //x
			{
				//���� �˻��ϴ� ��ġ
				Vec3 vPos = Vec3{ j, 0.f, i };

				//�ʿ����� ������ ���
				Vec3 vRatio = vPos / m_vLandScapeLen;
				//Vec3 vAbsRatio = Vec3(abs(vRatio.x), abs(vRatio.y), abs(vRatio.z));

				//�ʿ����� ������ ���
				vRatio.x *= m_iDivideX; //0.5 * 40 = 20
				vRatio.z *= m_iDivideZ;
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
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetObjects();

	CGameObject* pPlayer = vecPlayer[0]->GetParent();
	if (pPlayer->IsDead())
		return;

	Matrix matPlayerPos = pPlayer->Collider3D()->GetColliderWorldMat();
	//�÷��̾� ���� ���� ��ġ ������ ��Ƽ� �ʿ� �ֱ�
	Vec3 vPlayerPos = Vec3(matPlayerPos._41, matPlayerPos._42, matPlayerPos._43);

	Vec3 vRatio = vPlayerPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX;
	vRatio.z *= m_iDivideZ;

	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 5;//�������� 5
	Vec3 vGoalPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);
	// --����

	CGameObject* pMonster = GetOwner();
	Matrix matStartPos = pMonster->Collider3D()->GetColliderWorldMat();
	Vec3 vMonsterPos = Vec3(matStartPos._41, matStartPos._42, matStartPos._43);

	vRatio = vMonsterPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX;
	vRatio.z *= m_iDivideZ;
	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 6;//���� ��ġ�� 6	
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

