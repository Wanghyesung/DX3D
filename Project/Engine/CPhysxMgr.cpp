#include "pch.h"
#include "CPhysxMgr.h"

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
    
}
void CPhysxMgr::tick()
{

}

PxRigidDynamic* CPhysxMgr::GetRigidDynamic()
{
    PxRigidDynamic*  m_pRigidbody = m_pPhysics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.f, 0.f)));
    m_pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
     
    m_pScene->addActor(*m_pRigidbody);

    return m_pRigidbody;
}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f); // 충돌체 마찰력,Dynamic마찰력, 탄성력

    return pMaterial;
}

