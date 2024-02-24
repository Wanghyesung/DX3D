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

struct PxCollisionEvent
{
	CGameObject* pEventObj;
	UINT eLayerBit; // 0 << 1 
};


class CPxEvent;
class CPhysxMgr
	: public CSingleton<CPhysxMgr>
{
	SINGLE(CPhysxMgr);

private:
	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;
	PxFoundation* m_pFoundation;

	// PxFoundation이 있어야 gPhysics를 생성할 수 있다.
	PxPhysics* m_pPhysics;
	PxScene* m_pScene;

	// CPU 리소스를 효율적으로 공유할 수 있도록 하기 위해 구현
	PxDefaultCpuDispatcher* m_pDispatcher;
	CPxEvent* m_pCollisionCallback;

	UINT	m_matrix[MAX_LAYER];
	map<string, PxCollisionEvent> m_mapEventObj;//충돌됐을 때 이벤트 호출
	//PxMaterial* m_pMaterial;

	

public:
	void init();
	void tick();
	
	

	PxScene* GetScene() { return m_pScene; }
	PxRigidDynamic* GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, CGameObject* _pCollEventObj);
	PxMaterial* GetPxMaterial();
	CPxEvent* GetEvent() { return m_pCollisionCallback; }

	void AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, CGameObject* _pCollEventObj);
	void AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle);


	//collision
	void AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj);
	PxCollisionEvent FIndEventObj(string _strActorName);
	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}
	void LayerCheck(UINT _left, UINT _right); // UINT(LAYER_TYPE) 
	void CollisionCheck(string _left, string _right);

};
