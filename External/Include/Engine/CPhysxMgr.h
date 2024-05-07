#pragma once

#include "CSingleton.h"
#include <Physx\PxPhysicsAPI.h>
#include <Physx\PxPhysics.h>

#ifdef _DEBUG
#pragma comment(lib, "Physx/debug/PhysX_64")
#pragma comment(lib, "Physx/debug/PhysXFoundation_64")
#pragma comment(lib, "Physx/debug/PhysXExtensions_static_64")
#else
#pragma comment(lib, "Physx/release/PhysX_64"))
#pragma comment(lib, "Physx/release/PhysXFoundation_64")
#pragma comment(lib, "Physx/release/PhysXExtensions_static_64")
#endif


using namespace physx;

union PxCollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};

struct PxCheckColl
{
	bool bOnColl;//�浹 üũ
	bool bCheck;//�̹������ӿ��� ������Ʈ�� �޾Ҵ���

	CGameObject* pLeftObj;
	CGameObject* pRightObj;
};

struct PxCollisionEvent
{
	CGameObject* pEventObj;
	UINT eLayerBit; // 0 << 1

	bool bPass;
};


class CPxCollisionEvent;
class CPhysxMgr
	: public CSingleton<CPhysxMgr>
{
	SINGLE(CPhysxMgr);

private:
	// Foundation�� �����ϴµ� �ʿ��� ����
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;
	PxFoundation* m_pFoundation;

	// PxFoundation�� �־�� gPhysics�� ������ �� �ִ�.
	PxPhysics* m_pPhysics;
	PxScene* m_pScene;

	// CPU ���ҽ��� ȿ�������� ������ �� �ֵ��� �ϱ� ���� ����
	PxDefaultCpuDispatcher* m_pDispatcher;
	CPxCollisionEvent* m_pCollisionCallback;

	UINT	m_matrix[MAX_LAYER];
	map<UINT, PxCollisionEvent> m_mapEventObj;//�浹 �ɽÿ� �̺�Ʈ ȣ��� ������Ʈ��
	map<UINT_PTR, PxCheckColl> m_mapCol;//��ü�� �浹�ƴ��� �ȵƴ��� Ȯ�ο�
	//PxMaterial* m_pMaterial;

	bool m_bUseCCD;

public:
	void init();
	void tick();
	void tick_collision();

	PxScene* GetScene() { return m_pScene; }
	PxRigidDynamic* GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, int _iLayer, CGameObject* _pCollEventObj = nullptr);
	PxMaterial* GetPxMaterial();
	CPxCollisionEvent* GetEvent() { return m_pCollisionCallback; }

	bool UseCCD() { return m_bUseCCD; }
	const PxVec3& GetGravity() { return m_pScene->getGravity(); }

	void AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, int _iLayer,
		CGameObject* _pCollEventObj = nullptr);

	void AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, int _iLayer,
		CGameObject* _pCollEventObj = nullptr);


	//collision
	void CollisionCheck(CGameObject* _pLeftObj, CGameObject* _pRightObj);
	void ResetCollisionCheck();//PxCheckColl�� bCheck���� false��
	void AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj, int _iLayer);
	PxCollisionEvent FIndEventObj(UINT _iID);
	void DeleteEventObj(UINT _iID);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}
	void LayerCheck(UINT _left, UINT _right); // UINT(LAYER_TYPE) 
	bool CollisionCheck(UINT _ileftLayer, UINT _irightLayer);

};
