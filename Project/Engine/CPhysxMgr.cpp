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
    //sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����

    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_pScene = m_pPhysics->createScene(sceneDesc);
    m_pScene->setGravity(PxVec3(0.0f, -9.8f, 0.0f));//�߷�
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, true); // CCD Ȱ��ȭ

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
    PxBoxGeometry geometry(vScale/2.f); // ���� ũ��
 
    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
     
    PxShape* shape = nullptr;
    pRigidbody->getShapes(&shape, 1);
    m_pScene->addActor(*pRigidbody);

    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD ����

    // �浹 �׷� �� �浹 ����ũ ����
    shape->setSimulationFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // �浹 �׷� ����
    shape->setQueryFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // �浹 ����ũ ����


    return pRigidbody;
}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f); // �浹ü ������,Dynamic������, ź����

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle,
    eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale/2.f); // ���� ũ��

    //�ʱ�ȭ�� ��ġ
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);
    // 
    // ȸ�� ���� ���� (45���� �������� ��ȯ)
    float angle = XMConvertToRadians(45.0f);

    PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // ȸ���� ��

    // ���ʹϾ����� ȸ���� ��Ÿ���� �κ�
    PxQuat rotation(angle, axis);

    // ������ ���ʹϾ��� ���ο� ȸ������ ������Ʈ
    pose.q = rotation * pose.q;

    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);
  
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
        
    PxShape* shape = nullptr;
    pRigidbody->getShapes(&shape, 1);
    m_pScene->addActor(*pRigidbody);

    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD ����

    // �浹 �׷� �� �浹 ����ũ ����
    shape->setSimulationFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // �浹 �׷� ����
    shape->setQueryFilterData(PxFilterData(_eGroups, _eOtherGroups, 0, 0)); // �浹 ����ũ ����

    pRigidbody->setContactReportThreshold(0.01f); 
}

