#include "pch.h"
#include "CPhysxMgr.h"
#include "CTimeMgr.h"
#include "CPxEvent.h"
CPhysxMgr::CPhysxMgr()
    : m_pDispatcher(nullptr)
    , m_pFoundation(nullptr)
    , m_pPhysics(nullptr)
{

}

CPhysxMgr::~CPhysxMgr()
{
    m_pDispatcher->release();
    m_pPhysics->release();
    m_pFoundation->release();
}


void CPhysxMgr::init()
{
    m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator,
        gErrorCallback);
    if (!m_pFoundation)
        assert(nullptr);

    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);
    if (!m_pPhysics)
        assert(nullptr);


    PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
    //sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅

    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_pScene = m_pPhysics->createScene(sceneDesc);
    m_pScene->setGravity(PxVec3(0.0f, -9.8f, 0.0f));//중력
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, true); // CCD 활성화

    CPxEvent collisionCallback;
    m_pScene->setSimulationEventCallback(&collisionCallback);
}
void CPhysxMgr::tick()
{
    m_pScene->simulate(DT);
    m_pScene->fetchResults(true);
}

PxRigidDynamic* CPhysxMgr::GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);


    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale/2.f); // 상자 크기
 
    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
     
    PxShape* shape = nullptr;
    pRigidbody->getShapes(&shape, 1);
    m_pScene->addActor(*pRigidbody);

    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD 설정

    // 충돌 그룹 및 충돌 마스크 설정
    shape->setSimulationFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // 충돌 그룹 설정
    shape->setQueryFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // 충돌 마스크 설정


    return pRigidbody;
}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f); // 충돌체 마찰력,Dynamic마찰력, 탄성력

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle,
    eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale/2.f); // 상자 크기

    //초기화할 위치
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);
    // 
    // 회전 각도 설정 (45도를 라디안으로 변환)
    float angle = XMConvertToRadians(45.0f);

    PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // 회전할 축

    // 쿼터니언으로 회전을 나타내는 부분
    PxQuat rotation(angle, axis);

    // 기존의 쿼터니언을 새로운 회전으로 업데이트
    pose.q = rotation * pose.q;

    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);
  
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
        
    PxShape* shape = nullptr;
    pRigidbody->getShapes(&shape, 1);
    m_pScene->addActor(*pRigidbody);

    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD 설정

    // 충돌 그룹 및 충돌 마스크 설정
    shape->setSimulationFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // 충돌 그룹 설정
    shape->setQueryFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // 충돌 마스크 설정

    pRigidbody->setContactReportThreshold(0.01f); 
}

