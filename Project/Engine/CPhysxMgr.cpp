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

    if(m_pCollisionCallback)
        delete m_pCollisionCallback;
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
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅

    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
   
    //sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;//Multi Box Pruning
    //sceneDesc.solverType = PxDynamicsSolverType::eTGS;
    
    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pCollisionCallback = new CPxEvent();
    m_pScene->setSimulationEventCallback(m_pCollisionCallback);

    //m_pScene->setGravity(PxVec3(0.0f, -980.f, 0.0f));//중력
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, true); // CCD 활성화

}
void CPhysxMgr::tick()
{
    m_pScene->simulate(1.f/60.f);
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

    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
  
    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false); // CCD 설정

   
    PxFilterData  filterData;
    filterData.word0 = _eGroups;
    filterData.word1 = _eOtherGroups;

  
    // 충돌 그룹 및 충돌 마스크 설정
    pShape->setSimulationFilterData(filterData); // 충돌 그룹 설정
    pShape->setQueryFilterData(filterData); // 충돌 마스크 설정

    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // 시각화 플래그 설정
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);

    return pRigidbody;

}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.f, 0.f, 0.f); // 충돌체 마찰력,Dynamic마찰력, 탄성력

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle,
    eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    //m_mapObjSize.insert(make_pair());

    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale/2.f); // 상자 크기

    //초기화할 위치
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);
   
    //회전
    if (_vAxis != Vec3::Zero)
    {
        // 회전 각도 설정 (45도를 라디안으로 변환)
        float angle = XMConvertToRadians(_fAngle);

        PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // 회전할 축

        // 쿼터니언으로 회전을 나타내는 부분
        PxQuat rotation(angle, axis);

        // 기존의 쿼터니언을 새로운 회전으로 업데이트
        pose.q = rotation * pose.q;
    }

    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);
   
    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
   
    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);
  
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD 설정

    PxFilterData  filterData;
    filterData.word0 = _eGroups;
    filterData.word1 = _eOtherGroups;


    // 충돌 그룹 및 충돌 마스크 설정
    pShape->setSimulationFilterData(filterData); // 충돌 그룹 설정
    pShape->setQueryFilterData(filterData); // 충돌 마스크 설정
    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // 시각화 플래그 설정

    pRigidbody->setContactReportThreshold(0.01f); 
    
    m_pScene->addActor(*pRigidbody);

}

void CPhysxMgr::AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale / 2.f); // 상자 크기

    //초기화할 위치
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);

    //회전
    if (_vAxis != Vec3::Zero)
    {
        // 회전 각도 설정 (45도를 라디안으로 변환)
        float angle = XMConvertToRadians(_fAngle);

        PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // 회전할 축

        // 쿼터니언으로 회전을 나타내는 부분
        PxQuat rotation(angle, axis);

        // 기존의 쿼터니언을 새로운 회전으로 업데이트
        pose.q = rotation * pose.q;
    }

    PxRigidStatic* pRigidbody = m_pPhysics->createRigidStatic(pose);
      
    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);

    m_pScene->addActor(*pRigidbody);

}

