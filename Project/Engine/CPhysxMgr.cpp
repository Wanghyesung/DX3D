#include "pch.h"
#include "CPhysxMgr.h"
#include "CTimeMgr.h"
#include "CPxEvent.h"
#include "CCollider3D.h"
class TriggersFilterCallback : public PxSimulationFilterCallback
{
    virtual		PxFilterFlags	pairFound(PxU64 pairID,
        PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
        PxPairFlags& pairFlags)	PX_OVERRIDE
    {
        //		printf("pairFound\n");

        if (s0->userData || s1->userData)	// See createTriggerShape() function
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

            //if (usesCCD())
            //    pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eNOTIFY_TOUCH_CCD;
        }
        else
            pairFlags = PxPairFlag::eCONTACT_DEFAULT;

        //�̺�Ʈ �浹
        if (s0->getName() != NULL && s1->getName() != NULL)
        {
            string strName0 = s0->getName();
            string strName1 = s1->getName();

            CPhysxMgr::GetInst()->CollisionCheck(strName0, strName1);
        }
        
        //�Ѵ� NULL�� �ƴϸ� layercheck

        return PxFilterFlags();
    }

    virtual		void	pairLost(PxU64 pairID,
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        bool objectRemoved)	PX_OVERRIDE
    {
        
        int a = 10;
        //printf("pairLost\n");
    }

    virtual		bool	statusChange(PxU64& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags)	PX_OVERRIDE
    {
        printf("statusChange\n");
        return false;
    }
}gTriggersFilterCallback;

static	PxFilterFlags triggersUsingFilterCallback(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    //	printf("contactReportFilterShader\n");

    //pairFlags = PxPairFlag::eCONTACT_EVENT_POSE;
    
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    //pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT|PxPairFlag::eNOTIFY_TOUCH_CCD;
    return PxFilterFlag::eCALLBACK;
}


CPhysxMgr::CPhysxMgr()
    : m_pDispatcher(nullptr)
    , m_pFoundation(nullptr)
    , m_pPhysics(nullptr)
    , m_mapEventObj{}
{

}

CPhysxMgr::~CPhysxMgr()
{
    m_pDispatcher->release();
    m_pPhysics->release();
    m_pFoundation->release();

    if (m_pCollisionCallback)
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
    //sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����

    //m_pCollisionCallback = new CPxEvent();
    //m_pScene->setSimulationEventCallback(m_pCollisionCallback);

    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;

    //sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    sceneDesc.filterShader = triggersUsingFilterCallback;
    sceneDesc.filterCallback = &gTriggersFilterCallback;
    //sceneDesc.simulationEventCallback = m_pCollisionCallback;
   
     //PxFilterFlag::eCALLBACK;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;//Multi Box Pruning
    //sceneDesc.broadPhaseCallback = 
    //sceneDesc.solverType = PxDynamicsSolverType::eTGS;

    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pScene->setGravity(PxVec3(0.0f, -980.f, 0.0f));//�߷�
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, false); // CCD Ȱ��ȭ

   
}
void CPhysxMgr::tick()
{
    m_pScene->simulate(1.f / 60.f);
    m_pScene->fetchResults(true);
}

PxRigidDynamic* CPhysxMgr::GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, CGameObject* _pCollEventObj)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);


    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale / 2.f); // ���� ũ��

    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    //m_pPhysics->createShape(geometry,*material)
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    pShape->setName("0");

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

    //PxShape* shape = nullptr;
    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, false); // CCD ����
    

    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // �ð�ȭ �÷��� ����
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);


    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj);
    }

    return pRigidbody;

}

PxMaterial* CPhysxMgr::GetPxMaterial()
{
    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.f, 0.f, 0.f); // �浹ü ������,Dynamic������, ź����

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, CGameObject* _pCollEventObj)
{
    //m_mapObjSize.insert(make_pair());

    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale / 2.f); // ���� ũ��

    //�ʱ�ȭ�� ��ġ
    PxTransform pose(vPos);
    
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

   
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true);
    //pShape->setName("0");

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, false); // CCD ����
    pRigidbody->setName("0"); //�浹 �̺�Ʈ�� ����

    //PxFilterData  filterData;
    //filterData.word0 = 1 << (int)_eLayer;
    //filterData.word1 = 1 << (int)_eOtherLayer;


    // �浹 �׷� �� �浹 ����ũ ����
    //pShape->setSimulationFilterData(filterData); // �浹 �׷� ����
    //pShape->setQueryFilterData(filterData); // �浹 ����ũ ����
   
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);

    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj);
    }
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
   
    //PxShape* shape = nullptr;
    //pRigidbody->getShapes(&pShape, 1);
    pRigidbody->attachShape(*pShape);

    m_pScene->addActor(*pRigidbody);

}

void CPhysxMgr::AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj)
{
    UINT ID = _pGameObj->GetID();
    string strName = std::to_string(ID);

    PxCollisionEvent tEvent = FIndEventObj(strName);
    if (tEvent.pEventObj == nullptr)
    {
        _pShape->setName(strName.c_str()); //������Ʈ ID == actor name

        int iLayerBit = _pGameObj->GetLayerIndex();
        tEvent.eLayerBit = iLayerBit;
        tEvent.pEventObj = _pGameObj;

        m_mapEventObj.insert(make_pair(strName, tEvent));
    }
}

PxCollisionEvent CPhysxMgr::FIndEventObj(string _strActorName)
{
    map<string, PxCollisionEvent>::iterator iter = m_mapEventObj.find(_strActorName);
    if (iter == m_mapEventObj.end())
    {
        return PxCollisionEvent();
    }
    
    return iter->second;
}

void CPhysxMgr::LayerCheck(UINT _left, UINT _right)
{
    UINT iRow = _left;
    UINT iCol = _right;

    if (iRow > iCol)
    {
        UINT iTemp = iCol;
        iCol = iRow;
        iRow = iTemp;
    }

    m_matrix[iRow] |= (1 << iCol);

}


void CPhysxMgr::CollisionCheck(string _left, string _right)
{
    PxCollisionEvent pLeftEvent = FIndEventObj(_left);
    PxCollisionEvent pRightEvent = FIndEventObj(_left);

    UINT iRow = pLeftEvent.eLayerBit;
    UINT iCol = pRightEvent.eLayerBit;

    if (!(m_matrix[iRow] & (1 << iCol)))
        return;

    CCollider3D* pLeftCollider = pLeftEvent.pEventObj->Collider3D();
    CCollider3D* pRightCollider = pRightEvent.pEventObj->Collider3D();

    if (pLeftCollider && pRightCollider)
    {
        pLeftCollider->BeginOverlap(pRightCollider);
        pRightCollider->BeginOverlap(pLeftCollider);
    }

}
