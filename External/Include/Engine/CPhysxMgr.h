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

class CPxEvent;
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
	CPxEvent* m_pCollisionCallback;

	map<UINT, Vec3> m_mapObjSize;
	//PxMaterial* m_pMaterial;

public:
	void init();
	void tick();

	PxScene* GetScene() { return m_pScene; }
	PxRigidDynamic* GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups);
	PxMaterial* GetPxMaterial();

	void AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle,
		eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups);

	void AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle);
};
