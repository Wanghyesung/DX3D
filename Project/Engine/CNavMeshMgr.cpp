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
	m_mapInitTime{},
	m_vecMap{},
	m_vecClose{}
{

}

CNavMeshMgr::~CNavMeshMgr()
{

}

void CNavMeshMgr::add_monster(CGameObject* pGameObj , float _fSearchTime)
{
	int iID = pGameObj->GetID();

	map<int, tNavInfo>::iterator iter =
		m_mapInitTime.find(iID);

	if (iter != m_mapInitTime.end())
		return;

	tNavInfo tNav = {};
	tNav.iID = iID;
	tNav.fCurTime = 0.f;
	tNav.fEndTime = _fSearchTime;
	tNav.vCurPos = Vec3::Zero;
	tNav.vGoalPos = Vec3::Zero;
	tNav.bSearchOn = false;

	m_mapInitTime.insert(make_pair(iID, tNav));
}

void CNavMeshMgr::tick()
{
	map<int, tNavInfo>::iterator iter
		= m_mapInitTime.begin();

	for (iter; iter != m_mapInitTime.end(); ++iter)
	{
		if (iter->second.bSearchOn)
			continue;

		iter->second.fCurTime += DT;
		if (iter->second.fCurTime > iter->second.fEndTime)
		{
			iter->second.fCurTime = 0.f;
			iter->second.bSearchOn = true;
		}
	}

	Init_Map();
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
	Vec3 vSpaceLen = vRightTop - vLeftDown;
	//Vec3 vCapeLen 
	//40 X 40 2���� �迭
	int x = 40; //��ü ������ ���� ��
	int z = 40; 

	
	Rect tScapeRect;
	//��ĭ�� �����϶����� �� �� �ִ� ����
	float fMoveX = vScale.x / x;
	float fMoveZ = vScale.z / z;

	m_vecMap.resize((int)fMoveZ);
	m_vecClose.resize((int)fMoveZ);
	for (int i = 0; i < (int)fMoveZ; ++i)
	{
		m_vecMap[i].resize(fMoveX);
		m_vecClose[i].resize(fMoveZ);
	}

	//--- ���� ������ �� ����
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
		//��� �»�
		Vec3 vRightTop = vformPos + vformScale;
		Vec3 vLeftDown = vformPos - vformScale;

		Vec3 vRatio = vRightTop / vSpaceLen;
		Vec3 vAbsRatio = Vec3(abs(vRatio.x), abs(vRatio.y), abs(vRatio.z));


		int a = 10;
		//���� ����� ���� ������
		//Vec3 vPos = vecLandform[i]->Transform()->GetRelativePos();
		//Vec3 vScale = vecLandform[i]->Transform()->GetRelativeScale();



	}
}

void CNavMeshMgr::Init_Close()
{
	
}

const vector<std::pair<int, int>>& CNavMeshMgr::GetTargetPath(int _iID, const Vec3& _vecStartPos)
{
	vector<std::pair<int, int>> vecFinalPath = {};

	map<int, tNavInfo>::iterator iter =
		m_mapInitTime.find(_iID);

	bool bSearch = iter->second.bSearchOn;

	if (!bSearch)
		return vecFinalPath;

	//A*

	
	return vecFinalPath;
}
