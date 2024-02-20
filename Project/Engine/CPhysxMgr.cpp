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
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����

    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
   
    //sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;//Multi Box Pruning
    //sceneDesc.solverType = PxDynamicsSolverType::eTGS;
    
    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pCollisionCallback = new CPxEvent();
    m_pScene->setSimulationEventCallback(m_pCollisionCallback);

    //m_pScene->setGravity(PxVec3(0.0f, -980.f, 0.0f));//�߷�
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, true); // CCD Ȱ��ȭ

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
    PxBoxGeometry geometry(vScale/2.f); // ���� ũ��
 
    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
  
    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false); // CCD ����

   
    PxFilterData  filterData;
    filterData.word0 = _eGroups;
    filterData.word1 = _eOtherGroups;

  
    // �浹 �׷� �� �浹 ����ũ ����
    pShape->setSimulationFilterData(filterData); // �浹 �׷� ����
    pShape->setQueryFilterData(filterData); // �浹 ����ũ ����

    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // �ð�ȭ �÷��� ����
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);

    return pRigidbody;

}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.f, 0.f, 0.f); // �浹ü ������,Dynamic������, ź����

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle,
    eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    //m_mapObjSize.insert(make_pair());

    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale/2.f); // ���� ũ��

    //�ʱ�ȭ�� ��ġ
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);
   
    //ȸ��
    if (_vAxis != Vec3::Zero)
    {
        // ȸ�� ���� ���� (45���� �������� ��ȯ)
        float angle = XMConvertToRadians(_fAngle);

        PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // ȸ���� ��

        // ���ʹϾ����� ȸ���� ��Ÿ���� �κ�
        PxQuat rotation(angle, axis);

        // ������ ���ʹϾ��� ���ο� ȸ������ ������Ʈ
        pose.q = rotation * pose.q;
    }

    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);
   
    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
   
    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);
  
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true); // CCD ����

    PxFilterData  filterData;
    filterData.word0 = _eGroups;
    filterData.word1 = _eOtherGroups;


    // �浹 �׷� �� �浹 ����ũ ����
    pShape->setSimulationFilterData(filterData); // �浹 �׷� ����
    pShape->setQueryFilterData(filterData); // �浹 ����ũ ����
    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // �ð�ȭ �÷��� ����

    pRigidbody->setContactReportThreshold(0.01f); 
    
    m_pScene->addActor(*pRigidbody);

}

void CPhysxMgr::AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale / 2.f); // ���� ũ��

    //�ʱ�ȭ�� ��ġ
    PxTransform pose(vPos);
    //PxRigidStatic* pRigidStatic = createRigidStatic(, pose, geometry, *material, 1.0f);

    //ȸ��
    if (_vAxis != Vec3::Zero)
    {
        // ȸ�� ���� ���� (45���� �������� ��ȯ)
        float angle = XMConvertToRadians(_fAngle);

        PxVec3 axis(_vAxis.x, _vAxis.y, _vAxis.z); // ȸ���� ��

        // ���ʹϾ����� ȸ���� ��Ÿ���� �κ�
        PxQuat rotation(angle, axis);

        // ������ ���ʹϾ��� ���ο� ȸ������ ������Ʈ
        pose.q = rotation * pose.q;
    }

    PxRigidStatic* pRigidbody = m_pPhysics->createRigidStatic(pose);
      
    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

    //PxShape* shape = nullptr;
    pRigidbody->getShapes(&pShape, 1);

    m_pScene->addActor(*pRigidbody);

}

