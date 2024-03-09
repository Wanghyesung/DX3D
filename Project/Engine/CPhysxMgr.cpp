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
        //		printf("pairFound\n");

        if (s0->userData || s1->userData)	// See createTriggerShape() function
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

            if (CPhysxMgr::GetInst()->UseCCD())
                pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eNOTIFY_TOUCH_CCD;
        }
        else
            pairFlags = PxPairFlag::eCONTACT_DEFAULT;
       

        return PxFilterFlags();
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
}gTriggersFilterCallback;

static	PxFilterFlags triggersUsingFilterCallback(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    //내가 체크한 레이어들끼리 충돌했는지 검사
    const bool isTriggerPair = CPhysxMgr::GetInst()->CollisionCheck(filterData0.word0, filterData1.word0);

    // If we have a trigger, replicate the trigger codepath from PxDefaultSimulationFilterShader
    if (isTriggerPair)
    {
        //* pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS 기존 코드

        //이벤트 트리거
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT | 
            PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eCONTACT_DEFAULT;// | PxPairFlag::eNOTIFY_TOUCH_LOST;

        if (CPhysxMgr::GetInst()->UseCCD())
            pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
        
        return PxFilterFlag::eDEFAULT;
    }

    else
    {
        //기본 트리거
        // Otherwise use the default flags for regular pairs
        //pairFlags = PxPairFlag::eCONTACT_DEFAULT; 기존 코드

        pairFlags = PxPairFlag::eNOTIFY_TOUCH_LOST;// 뚫리게 할라면

        return PxFilterFlag::eDEFAULT;
    }
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

    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);
    if (!m_pPhysics)
        assert(nullptr);


    PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
    //sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅

    
    m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDispatcher;

    
    sceneDesc.filterShader = triggersUsingFilterCallback;
    sceneDesc.filterCallback = &gTriggersFilterCallback;

    m_pCollisionCallback = new CPxCollisionEvent();
    sceneDesc.simulationEventCallback = m_pCollisionCallback;
   
     //PxFilterFlag::eCALLBACK;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;//Multi Box Pruning
  
    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pScene->setGravity(PxVec3(0.0f, -981.f, 0.0f));//중력
    m_pScene->setFlag(PxSceneFlag::eENABLE_CCD, m_bUseCCD); // CCD 활성화
}
void CPhysxMgr::tick()
{
    //ResetCollisionCheck();

    m_pScene->simulate(1.f / 60.f);
    m_pScene->fetchResults(true);
}

void CPhysxMgr::tick_collision()
{
    map<UINT_PTR, PxCheckColl>::iterator iter = m_mapCol.begin();
    for (iter; iter != m_mapCol.end(); ++iter)
    {
        CGameObject* pLeftObj = iter->second.pLeftObj;
        CGameObject* pRightObj = iter->second.pRightObj;


        if (iter->second.bCheck)
        {
            //이전에도 충돌이고 지금도 충돌
            if (iter->second.bOnColl)
            {
                pLeftObj->Collider3D()->OnOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->OnOverlap(pLeftObj->Collider3D());
            }

            //이전에는 충돌하지 않음
            else
            {
                pLeftObj->Collider3D()->BeginOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->BeginOverlap(pLeftObj->Collider3D());
                iter->second.bOnColl = true;
            }

            iter->second.bCheck = false;
        }


        else
        {
            //이번 프레임에서 충돌체크를 받지 않았는데 onColl이 true면 exit
            if (iter->second.bOnColl)
            {
                pLeftObj->Collider3D()->EndOverlap(pRightObj->Collider3D());
                pRightObj->Collider3D()->EndOverlap(pLeftObj->Collider3D());

                iter->second.bOnColl = false;
            }
        }
    }

}

PxRigidDynamic* CPhysxMgr::GetRigidDynamic(Vec3 _vPos, Vec3 _vScale, int _iLayer, CGameObject* _pCollEventObj)
{
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);


    PxMaterial* material = GetPxMaterial();

    PxBoxGeometry geometry(vScale / 2.f); // 상자 크기

    PxTransform pose(vPos);
    PxRigidDynamic* pRigidbody = PxCreateDynamic(*m_pPhysics, pose, geometry, *material, 1.0f);

    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);


    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
    //pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, m_bUseCCD); // CCD 설정
    

    //pRigidbody->getActorFlags().setAll(PxActorFlag::eVISUALIZATION); // 시각화 플래그 설정
    pRigidbody->setContactReportThreshold(0.01f);

    m_pScene->addActor(*pRigidbody);


    if (nullptr != _pCollEventObj)
    {
        AddCollEventObj(pShape, _pCollEventObj, _iLayer);
    }

    material->release();
    pShape->release();

    return pRigidbody;

}

PxMaterial* CPhysxMgr::GetPxMaterial()
{

    PxMaterial* pMaterial =
        m_pPhysics->createMaterial(0.f, 0.f, 0.f); // 정지 마찰력, 운동 마찰력, 탄성력

    return pMaterial;
}

void CPhysxMgr::AddActor(const Vec3& _vPos, const Vec3& _vScale, Vec3 _vAxis, float _fAngle, int _iLayer,
    CGameObject* _pCollEventObj)
{
   
    PxVec3 vScale = PxVec3(_vScale.x, _vScale.y, _vScale.z);
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxMaterial* material = GetPxMaterial();
    PxBoxGeometry geometry(vScale / 2.f); // 상자 크기

    //초기화할 위치
    PxTransform pose(vPos);
    
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
    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);

    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
    //pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    pRigidbody->attachShape(*pShape);
    pRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, m_bUseCCD); // CCD 설정
    
   
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

    PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
    PxShape* pShape = m_pPhysics->createShape(geometry, *material, true, shapeFlags);
    pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);//중력 안받게
   
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

void CPhysxMgr::CollisionCheck(CGameObject* _pLeftObj, CGameObject* _pRightObj)
{
    PxCollisionID id = {};
    id.LeftID = _pLeftObj->Collider3D()->GetID();
    id.RightID = _pRightObj->Collider3D()->GetID();

    map<UINT_PTR, PxCheckColl>::iterator iter = m_mapCol.find(id.id);
   
    if (iter == m_mapCol.end())
    {
        PxCheckColl pxColl = PxCheckColl{ false, false ,_pLeftObj, _pRightObj};
        m_mapCol.insert(make_pair(id.id, pxColl));
        iter = m_mapCol.find(id.id);
    }

    iter->second.bCheck = true;
}

void CPhysxMgr::ResetCollisionCheck()
{
    map<UINT_PTR, PxCheckColl>::iterator iter = m_mapCol.begin();

    for (iter; iter != m_mapCol.end(); ++iter)
    {
        iter->second.bCheck = false;//충돌검사 전에 전부 false로 충돌이 됐다면 eventcallback에서 true로 전환
    }
}

void CPhysxMgr::AddCollEventObj(PxShape* _pShape, CGameObject* _pGameObj, int _iLayer)
{
    UINT ID = _pGameObj->GetID();
    
    PxCollisionEvent tEvent = FIndEventObj(ID);

    if (tEvent.pEventObj == nullptr)
    {
        PxFilterData filterData;
        //filterData = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
        filterData.word0 = ID; 

       // 충돌 그룹 및 충돌 마스크 설정
        _pShape->setSimulationFilterData(filterData); // 충돌 그룹 설정

        tEvent.eLayerBit = _iLayer;
        tEvent.pEventObj = _pGameObj;

        m_mapEventObj.insert(make_pair(ID, tEvent));
    }
}

PxCollisionEvent CPhysxMgr::FIndEventObj(UINT _iID)
{
    map<UINT, PxCollisionEvent>::iterator iter = m_mapEventObj.find(_iID);
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


bool CPhysxMgr::CollisionCheck(UINT _ileft, UINT _iright)
{
    PxCollisionEvent pLeftEvent = FIndEventObj(_ileft);
    PxCollisionEvent pRightEvent = FIndEventObj(_iright);

    UINT iRow = pLeftEvent.eLayerBit;
    UINT iCol = pRightEvent.eLayerBit;

    if (!(m_matrix[iRow] & (1 << iCol)))
        return false;

    return true;
}
