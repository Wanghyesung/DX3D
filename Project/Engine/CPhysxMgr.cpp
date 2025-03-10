#include "pch.h"
#include "CPhysxMgr.h"
#include "CTimeMgr.h"
#include "CPxCollisionEvent.h"
#include "CCollider3D.h"
class TriggersFilterCallback : public PxSimulationFilterCallback
{
    virtual		PxFilterFlags	pairFound(PxU64 pairID,
        PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
        PxPairFlags& pairFlags)	PX_OVERRIDE
    {
        if (s0->userData || s1->userData)	// See createTriggerShape() function
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

            if (CPhysxMgr::GetInst()->UseCCD())
                pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eNOTIFY_TOUCH_CCD;
        }
        else
            pairFlags = PxPairFlag::eCONTACT_DEFAULT;                          
       

        return PxFilterFlag::eDEFAULT;
    }

    virtual		void	pairLost(PxU64 pairID,
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        bool objectRemoved)	PX_OVERRIDE
    {
        //printf("pairLost\n");
    }

    virtual		bool	statusChange(PxU64& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags)	PX_OVERRIDE
    {
        printf("statusChange\n");
        return false;
    }
}gTriggersFilterCallback;//��� ����


//�浹 ���ɼ��� �ִ� �ָ� ���� AABB
static	PxFilterFlags triggersUsingFilterCallback(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    //�� ID�� ã��
    if (filterData0.word0 == 0 || filterData1.word0 == 0)
        return PxFilterFlag::eKILL;  
    
    //���� üũ�� ���̾�鳢�� �浹�ߴ��� �˻�
    const bool isTriggerPair = CPhysxMgr::GetInst()->CollisionCheck(filterData0.word1, filterData1.word1);
    
    if (isTriggerPair)
    {
        PxCollisionEvent pLeftEvent = CPhysxMgr::GetInst()->FindEventObj(filterData0.word0);
        PxCollisionEvent pRightEvent = CPhysxMgr::GetInst()->FindEventObj(filterData1.word0);

        //�̺�Ʈ Ʈ����
        pairFlags = PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eTRIGGER_DEFAULT;

        if (!pLeftEvent.bPass && !pRightEvent.bPass)
        {
            pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
        }
     
        if (CPhysxMgr::GetInst()->UseCCD())
            pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
    }
    else
        return PxFilterFlag::eKILL;//�ʿ� ���� �浹 ��� ����

    return PxFilterFlag::eDEFAULT;
}


CPhysxMgr::CPhysxMgr()
    : m_pDispatcher(nullptr)
    , m_pFoundation(nullptr)
    , m_pPhysics(nullptr)
    , m_mapEventObj{}
    , m_bUseCCD(true)
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


    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true/*, pvd*/);
    if (!m_pPhysics)
        assert(nullptr);


    PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
    //sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����

  
    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;

    
    sceneDesc.filterShader = triggersUsingFilterCallback;
    //sceneDesc.filterCallback = &gTriggersFilterCallback;

    m_pCollisionCallback = new CPxCollisionEvent();
    sceneDesc.simulationEventCallback = m_pCollisionCallback;
   
     //PxFilterFlag::eCALLBACK;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;//Multi Box Pruning
  
    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pScene->setGravity(PxVec3(0.0f, -981.f, 0.0f));//�߷�
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, m_bUseCCD); // CCD Ȱ��ȭ


    //����� �ð�ȭ�� Ȱ��ȭ�Ϸ��� ���� ���� �ð�ȭ ������ ��� ������ �����ؾ� �մϴ�.
    // m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    //�׷� ���� �ð�ȭ�Ǿ�� �ϴ� ���� �Ӽ��� �ٽ� ��� ���� ����Ͽ� Ȱ��ȭ�� �� �ֽ��ϴ�.
    //m_pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);

}
void CPhysxMgr::tick()
{
    //���� �ð� ���� ��� (Variable Time Step)

    m_pScene->simulate(1.f / 60.f);
    m_pScene->fetchResults(true);
}

void CPhysxMgr::tick_collision()
{
    map<UINT_PTR, PxCollisionPair>::iterator iter = m_mapCollisionPair.begin();
    for (iter; iter != m_mapCollisionPair.end(); )
    {
        CGameObject* pLeftObj = iter->second.pLeftObj;
        CGameObject* pRightObj = iter->second.pRightObj;

        if (!pLeftObj->Collider3D()->GetIsActive() ||
            !pRightObj->Collider3D()->GetIsActive())
        {
            ++iter;
            continue;
        }

        if (iter->second.bCheck)
        {
            //�������� �浹�̰� ���ݵ� �浹
            if (iter->second.bOnColl)
            {
                pLeftObj->Collider3D()->OnOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->OnOverlap(pLeftObj->Collider3D());
            }
            //�������� �浹���� ����
            else
            {
                pLeftObj->Collider3D()->BeginOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->BeginOverlap(pLeftObj->Collider3D());
                iter->second.bOnColl = true;
            }

            //���� �����ӿ��� �ٽ� üũ�ϱ� ���ؼ� false�� ��ȯ
            iter->second.bCheck = false;
        }
        else
        {
            //�̹� �����ӿ��� �浹üũ�� ���� �ʾҴµ� onColl�� true�� exit
            if (iter->second.bOnColl)
            {
                pLeftObj->Collider3D()->EndOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->EndOverlap(pLeftObj->Collider3D());

                iter->second.bOnColl = false;
            }
        }

        ++iter;
    }

}


PxRigidDynamic* CPhysxMgr::GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, int _iLayer, CGameObject* _pCollEventObj)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();

    PxBoxGeometry geometry(vScale / 2.f); // ���� ũ��

    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);


    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    //pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, m_bUseCCD); // CCD ����
    

    pRigidbody->setContactReportThreshold(0.01f);
        
    m_pScene->addActor(*pRigidbody);


    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj, _iLayer);
    }

    material->release();
    pShape->release();

    //pRigidbody->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    return pRigidbody;

}

PxMaterial* CPhysxMgr::GetPxMaterial()
{

    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.f, 0.f, 0.f); // ���� ������, � ������, ź����

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, int _iLayer,
    CGameObject* _pCollEventObj)
{
   
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
    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
    //pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, m_bUseCCD); // CCD ����
    
   
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);

    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj, _iLayer);
    }

    material->release();
    pShape->release();
}

void CPhysxMgr::AddActorStatic(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, int _iLayer,
    CGameObject* _pCollEventObj)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    //material->setStaticFriction(0.5f);
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

    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//�߷� �ȹް�
   
    //pRigidbody->getShapes(&pShape, 1);
    pRigidbody->attachShape(*pShape);

    m_pScene->addActor(*pRigidbody);

    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj, _iLayer);
    }

    material->release();
    pShape->release();
}

void CPhysxMgr::AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj, int _iLayer)
{
    UINT ID = _pGameObj->GetID();
    
    PxCollisionEvent tEvent = FindEventObj(ID);

    if (tEvent.pEventObj == nullptr)
    {
        PxFilterData filterData = {};
       
        //�� ���� ������Ʈ�� ID�� ���
        filterData.word0 = ID; 
        filterData.word1 = _iLayer; //���̾� ���

       // �浹 �׷� �� �浹 ����ũ ����
        _pShape->setSimulationFilterData(filterData); 

        tEvent.pEventObj = _pGameObj; //�̺�Ʈ�� ȣ���ų ���� ������Ʈ ���

        m_mapEventObj.insert(make_pair(ID, tEvent));//�ش�
    }
}

PxCollisionEvent CPhysxMgr::FindEventObj(UINT _iID)
{
    map<UINT, PxCollisionEvent>::iterator iter = m_mapEventObj.find(_iID);
    if (iter == m_mapEventObj.end())
    {
        return PxCollisionEvent();
    }
    
    //�������� ������ �� �ְ� üũ
    CPxRigidbody* pRigid = iter->second.pEventObj->PxRigidbody();
    if (pRigid)
    {
        bool b = pRigid->IsPass();
        iter->second.bPass = b;
    }

    return iter->second;
}

void CPhysxMgr::DeleteEventObj(UINT _iID)
{
    map<UINT, PxCollisionEvent>::iterator iter = m_mapEventObj.find(_iID);
    if (iter == m_mapEventObj.end())
        return;

    m_mapEventObj.erase(_iID);
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

void CPhysxMgr::AddCollisionPair(CGameObject* _pLeftObj, CGameObject* _pRightObj)
{
    PxCollisionID id = {};
    id.LeftID = _pLeftObj->GetID();
    id.RightID = _pRightObj->GetID();

    const auto& iter = m_mapCollisionPair.find(id.id);
    if (iter == m_mapCollisionPair.end())
    {
        PxCollisionPair tPxPair = PxCollisionPair{ true, false ,_pLeftObj, _pRightObj };
        m_mapCollisionPair.insert(make_pair(id.id, tPxPair));
        return;
    }

    //�浹 �˻� üũ
    iter->second.bCheck = true;
}


bool CPhysxMgr::CollisionCheck(UINT _ileft, UINT _iright)
{
    UINT iRow = _ileft;
    UINT iCol = _iright;

    if (iRow > iCol)
    {
        UINT iTemp = iCol;
        iCol = iRow;
        iRow = iTemp;
    }

    if (!(m_matrix[iRow] & (1 << iCol)))
        return false;

    return true;
}
