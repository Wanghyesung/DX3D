#pragma once

#include "CSingleton.h"
#include <Physx\PxPhysicsAPI.h>
#include <Physx\PxPhysics.h>

#ifdef _DEBUG
#pragma comment(lib, "Physx/debug/PhysX_64")
#pragma comment(lib, "Physx/debug/PhysXFoundation_64")
#pragma comment(lib, "Physx/debug/PhysXExtensions_static_64")
#else
#pragma comment(lib, "Physx/release/PhysX_64")
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

struct PxCollisionPair
{
	bool bCheck;//이번프레임에서 업데이트를 받았는지
	bool bOnColl;//충돌 체크

	CGameObject* pLeftObj;
	CGameObject* pRightObj;
};

struct PxCollisionEvent
{
	CGameObject* pEventObj;
	
	bool bCollision;
	bool bPass;
};


class CPxCollisionEvent;
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
	CPxCollisionEvent* m_pCollisionCallback;

	UINT	m_matrix[MAX_LAYER];
	map<UINT, PxCollisionEvent> m_mapEventObj;//충돌 될시에 이벤트 호출될 오브젝트들
	map<UINT_PTR, PxCollisionPair> m_mapCollisionPair;//// 충돌한 객체 ID 저장 (이미 충돌 중인지 확인하기 위함)

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
	void AddCollisionPair(CGameObject* _pLeftObj, CGameObject* _pRightObj);

	void AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj, int _iLayer);
	PxCollisionEvent FindEventObj(UINT _iID);
	void DeleteEventObj(UINT _iID);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}
	void LayerCheck(UINT _left, UINT _right); // UINT(LAYER_TYPE) 
	bool CollisionCheck(UINT _ileftLayer, UINT _irightLayer);

};
