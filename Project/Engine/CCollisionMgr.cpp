#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
CCollisionMgr::CCollisionMgr()
	: m_matrix{}
{

}


CCollisionMgr::~CCollisionMgr()
{

}



void CCollisionMgr::tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
			CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
		}
	}
}


void CCollisionMgr::CollisionBtwLayer(CLayer* _Left, CLayer* _Right)
{
	const vector<CGameObject*>& vecLeft = _Left->GetObjects();
	const vector<CGameObject*>& vecRight = _Right->GetObjects();

	if (_Left == _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwObject(vecLeft[i], vecRight[j]);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtwObject(vecLeft[i], vecRight[j]);
			}
		}
	}
}

void CCollisionMgr::CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	if (!(_LeftObject->Collider3D() && _RightObject->Collider3D()))
		return;

	// 충돌체 ID 생성
	CollisionID id = {};
	id.LeftID = _LeftObject->Collider3D()->GetID();
	id.RightID = _RightObject->Collider3D()->GetID();

	// ID 검색
	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		iter = m_mapColID.find(id.id);
	}

	// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}

	//pxrigidbody에서 내 오브젝트들 아이디 주고 충돌했는지 반환값을 가져옴 true면 이벤트 호출
	// 두 충돌체가 지금 충돌 중인지 확인
	if (CollisionBtwCollider(_LeftObject->Collider3D(), _RightObject->Collider3D()))
	{
		// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
		if (bDead && iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
			iter->second = false;
		}
		else if (iter->second)
		{
			// 계속 충돌 중
			_LeftObject->Collider3D()->OnOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->OnOverlap(_LeftObject->Collider3D());
		}
		else
		{
			// 이번 프레임에 충돌
			if (!bDead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
			{
				_LeftObject->Collider3D()->BeginOverlap(_RightObject->Collider3D());
				_RightObject->Collider3D()->BeginOverlap(_LeftObject->Collider3D());
				iter->second = true;
			}
		}
	}

	else
	{
		// 충돌 해제
		if (iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
			iter->second = false;
		}
	}


}

// 두 충돌체의 충돌 검사 진행
bool CCollisionMgr::CollisionBtwCollider(CCollider3D* _pLeft, CCollider3D* _pRight)
{

	//     
	// 
	//  4 -- 5
	//  |    | 
	//  6 -- 7
	// 0 -- 1
	// |    |
	// 3 -- 2

	Vec3 arrLocal[8] =
	{
		//앞면
		Vec3(-0.5f, 0.5f, 0.5f),
		Vec3(0.5f, 0.5f, 0.5f),
		Vec3(0.5f, -0.5f, 0.5f),
		Vec3(-0.5f, -0.5f, 0.5f),

		//뒷면

		Vec3(-0.5f, 0.5f, -0.5f),
		Vec3(0.5f, 0.5f, -0.5f),
		Vec3(0.5f, -0.5f, -0.5f),
		Vec3(-0.5f, -0.5f, -0.5f)
	};

	// 두 충돌체의 각 표면 벡터 2개씩 구함
	Vec3 arrProj[6] = {};

	arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
	arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
	arrProj[2] = XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());


	arrProj[3] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
	arrProj[4] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
	arrProj[5] = XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

	// 두 충돌체의 중심점을 구함
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


	// 분리축 테스트
	for (int i = 0; i < 6; ++i)
	{
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
		float fProjDist = 0.f;
		for (int j = 0; j < 6; ++j)
		{
			fProjDist += fabsf(arrProj[j].Dot(vProj));
		}
		fProjDist /= 2.f;

		float fCenter = fabsf(vCenter.Dot(vProj));

		if (fProjDist < fCenter)
			return false;
	}

	return true;
}



void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	LayerCheck(pLeft->GetLayerIndex(), pRight->GetLayerIndex());
}
