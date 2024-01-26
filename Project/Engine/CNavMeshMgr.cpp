#include "pch.h"
#include "CNavMeshMgr.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CLandScape.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider3D.h"

CNavMeshMgr::CNavMeshMgr():
	m_mapNavInfo{},
	m_vecMap{},
	m_vecClose{}
{

}

CNavMeshMgr::~CNavMeshMgr()
{

}

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

void CNavMeshMgr::AddMonster(CGameObject* pGameObj , float _fSearchTime)
{
	int iID = pGameObj->GetID();

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(iID);

	if (iter != m_mapNavInfo.end())
		return;

	tNavInfo tNav = {};
	tNav.iID = iID;
	tNav.fCurTime = 0.f;
	tNav.fEndTime = _fSearchTime;
	tNav.vCurPos = Vec3::Zero;
	tNav.vGoalPos = Vec3::Zero;
	tNav.bSearchOn = false;

	m_mapNavInfo.insert(make_pair(iID, tNav));
}

void CNavMeshMgr::DeleteMonster(CGameObject* pGameObj)
{
	int iID = pGameObj->GetID();

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(iID);

	if (iter != m_mapNavInfo.end())
		return;

	m_mapNavInfo.erase(iID);
}


void CNavMeshMgr::astar(int _ID , pair<int, int> _tStart, pair<int, int> _tGoal)
{
	priority_queue<tNode, vector<tNode>, cmp> queueWay;	// �켱���� ť

	tNode tStartNode;									// ���� ���

	vector<tNode> vecWay;							//���� �� ��

	int DX[8] = { 0 ,1, 0, -1, 1, 1,-1, -1 };				// ���� ��ǥ �����٢֢ע� = �ð��������
	int DZ[8] = {-1 ,0, 1,  0, 1,-1, 1, -1 };

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
	m_vecClose[tStartNode.z][tStartNode.x] = true;				// ���� �� ����(�ٽ� �� ����)

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
			if (nextX >= 0 && nextX < m_vecMap.size() && nextZ >= 0 && nextZ < m_vecMap.size())
			{
				//�������̰ų� ���� �� ���̸� �ٽ� ������
				if (m_vecMap[nextZ][nextX] != 1 && m_vecMap[nextZ][nextX] == false)
				{
					tAddNode.x = nextX;
					tAddNode.z = nextZ;
					tAddNode.iCurCos = i < 4 ? cost + 10 : cost + 14;// �����¿�� 10, �밢���̸� 14(��200)
					tAddNode.iGoalCos = (abs(_tGoal.second - tAddNode.x) + abs(_tGoal.first - tAddNode.z)) * 10;
					tAddNode.pParentNode = make_pair(z, x);	// ���� top��带 �θ� ���� ����
					m_vecClose[nextZ][nextX] = true;		// ���� �� ���� true�ؼ� �ٽ� ������
					m_vecMap[nextZ][nextX] = 9;		// Ž���� �ֺ� ��� = (���� ���� �ʾ����ϱ� 9�� ǥ��)
					queueWay.push(tAddNode);				// �켱���� ť�� ����	
				}
			}
		}
	}

	//�̶����� �� ���� �������� �����ɴϴ�
	int px = vecWay.back().x;
	int pz = vecWay.back().z;
	//������ �ϳ� ���ܵα�
	while (vecWay.size() <= 1) 
	{
		if (px == vecWay.back().x && pz == vecWay.back().z) {// ��ǥ ������ �θ� ��带 Ž���� ������
			m_vecMap[pz][px] = 4;//������ �� ���� 4�� ����صӴϴ�
			//������ ������ ����� �θ���(���� �Դ� ��)�� xy���������ͼ� ���� ���� �����մϴ�
			px = vecWay.back().pParentNode.second;
			pz = vecWay.back().pParentNode.first;
		}
		vecWay.pop_back(); //���� �ô� ���� �����մϴ� (���� ��带 Ȯ���ϱ� ���ؼ�)
	}
	
	tNode tDir = vecWay.back();
	m_mapNavInfo.find(_ID)->second.vSearchFoce = Vec3(tDir.x, 0.f, tDir.z);
}

void CNavMeshMgr::init_target(int _ID, const Vec3& _vStartPos, const Vec3& _vGoalPos)
{
	map<int, tNavInfo>::iterator iter
		= m_mapNavInfo.find(_ID);

	if (iter == m_mapNavInfo.end())
		return;

	iter->second.bSearchOn = true;
	iter->second.vCurPos = _vStartPos;
	iter->second.vGoalPos = _vGoalPos;
}


void CNavMeshMgr::tick()
{
	map<int, tNavInfo>::iterator iter
		= m_mapNavInfo.begin();

	for (iter; iter != m_mapNavInfo.end(); ++iter)
	{
		if (!iter->second.bSearchOn)
			continue;

		iter->second.fCurTime += DT;
		if (iter->second.fCurTime > iter->second.fEndTime)
		{
			iter->second.fCurTime = 0.f;
		}
	}
}


void CNavMeshMgr::Init_Map()
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
	//40 X 40 2���� �迭
	m_iDivideX = 80; //��ü ������ ���� ��
	m_iDivideZ = 80;

	m_vecMap.resize((int)m_iDivideZ);
	m_vecClose.resize((int)m_iDivideZ);
	for (int i = 0; i < (int)m_iDivideZ; ++i)
	{
		m_vecMap[i].resize(m_iDivideX);
		m_vecClose[i].resize(m_iDivideX);
	}


	//--- ���� ������ �� ����
	Init_CloseMap();
}

void CNavMeshMgr::Init_CloseMap()
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

		for (float i = fPos[2]; i <= fPos[3]; i += fOffsetZ)//z
		{
			for (float j = fPos[0]; j <= fPos[1]; j += fOffsetX) //x
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
				m_vecClose[(int)vRatio.x][(int)vRatio.z] = 1;
			}
		}
	}
}

void CNavMeshMgr::Tracking(const Vec3& _vStartPos, const Vec3& _vGoalPos)
{

}

void CNavMeshMgr::TrackingPlayer(CGameObject* _pStartObj)
{
	const vector<CGameObject*>& vecPlayer =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetObjects();

	CGameObject* pPlayer = vecPlayer[0];
	if (pPlayer->IsDead())
		return;

	Matrix matPlayerPos = pPlayer->Collider3D()->GetColliderWorldMat();
	//�÷��̾� ���� ���� ��ġ ������ ��Ƽ� �ʿ� �ֱ�
	Vec3 vPlayerPos = Vec3(matPlayerPos._41, matPlayerPos._42, matPlayerPos._43); 

	//y <----> z ����
	Vec3 vRatio = vPlayerPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX; 
	vRatio.y *= m_iDivideZ;
	
	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 5;//�������� 5
	Vec3 vGoalPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);
	// --����

	Matrix matStartPos = _pStartObj->Collider3D()->GetColliderWorldMat();
	Vec3 vMonsterPos = Vec3(matStartPos._41, matStartPos._42, matStartPos._43); 

	//y <----> z ����
	vRatio = vMonsterPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX;
	vRatio.y *= m_iDivideZ;
	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 6;//���� ��ġ�� 6	
	Vec3 vStartPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);

	int ID = _pStartObj->GetID();
	init_target(ID, vStartPos, vGoalPos);

	pair<int, int> tStart = make_pair(vStartPos.z, vStartPos.x);
	pair<int, int> tGoal = make_pair(vGoalPos.z, vGoalPos.x);

	//��ã��
	astar(ID, tStart, tGoal);
}


Vec3 CNavMeshMgr::GetTargetPath(CGameObject* _pObj)
{
	int ID = _pObj->GetID();
	vector<std::pair<int, int>> vecFinalPath = {};

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(ID);

	return iter->second.vSearchFoce;
}
