#include "pch.h"
#include "CPxRigidbody.h"
#include "CPhysxMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CCollider3D.h"
CPxRigidbody::CPxRigidbody():
	CComponent(COMPONENT_TYPE::PXRIGIDBODY),
    m_bGround(true),
    m_bAddGravity(true),
    m_bAccumulate(false),
    m_bPass(false),
    m_bActive(true),
    m_vTransformOffset(0.f,0.f,0.f),
    m_vDecVel(Vec3::Zero)
{
    m_eForceMode = PxForceMode::eACCELERATION;
}

CPxRigidbody::~CPxRigidbody()
{
   
}



void CPxRigidbody::finaltick()
{
    /*if (!m_bActive)
        return;*/

	CTransform* pTrasnform = GetOwner()->Transform();

    if (m_bDecrease)
    {
        m_vVelocity -=m_vDecVel;
    }

	PxVec3 vFoce = PxVec3(m_vForce.x, m_vForce.y, m_vForce.z);
	PxVec3 Vvel = PxVec3(m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);

   if(!m_bGround)
      tick_force(vFoce);
   
   else
      tick_velocity(Vvel);
    

    PxTransform transform = m_pRigidbody->getGlobalPose();
    //m_pRigidbody->setGlobalPose(transform);
    
    Vec3 vPos = Vec3(transform.p.x, transform.p.y, transform.p.z);
    pTrasnform->SetRelativePos(vPos + m_vTransformOffset);

    m_vForce = Vec3::Zero;
    m_vVelocity = Vec3::Zero;
}

void CPxRigidbody::init(const Vector3& _vPos, const Vector3& _vScale, int _iLayer, CGameObject* _pCollEventObj)
{
    m_pRigidbody = CPhysxMgr::GetInst()->GetRigidDynamic(_vPos, _vScale, _iLayer, _pCollEventObj);
   
    //이벤트
    m_pRigidbody->setMass(1.f);  
}

void CPxRigidbody::SetMass(float _fMass)
{
    m_fMass = _fMass;
    m_pRigidbody->setMass(_fMass);
}

void CPxRigidbody::SetGround(bool _bGround, bool _bAddGravity)
{
    m_bGround = _bGround; 
    m_bAddGravity = _bAddGravity;

    if (m_bGround)
        m_vAddGravityForce = CPhysxMgr::GetInst()->GetGravity(); //속도 중력
    else
        m_vAddGravityForce = PxVec3(0.f, 9.6f, 0.f); //가속도 중력

    m_pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, _bGround);
}


void CPxRigidbody::ChanageMaterial(UINT _iStaticCoef, UINT _iDynamicCoef)
{
    PxShape* pShape;
    m_pRigidbody->getShapes(&pShape, 1);

    PxMaterial* pMaterial;
    pShape->getMaterials(&pMaterial, 1);

    pMaterial->setStaticFriction(_iStaticCoef);
    pMaterial->setDynamicFriction(_iDynamicCoef);
}

void CPxRigidbody::ClearFoce()
{
    m_pRigidbody->clearForce(m_eForceMode);
}


void CPxRigidbody::SetMaxVelocity(float _fMaxVelocity)
{
    m_pRigidbody->setMaxLinearVelocity(_fMaxVelocity);
}


const Matrix& CPxRigidbody::GetPosMatrix()
{
    PxVec3 vPos = m_pRigidbody->getGlobalPose().p; // actor는 PxRigidActor* 포인터입니다.

    return XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
}

const Matrix& CPxRigidbody::GetRotMatrix()
{
    PxTransform rotation = m_pRigidbody->getGlobalPose();
    PxMat33 rotationMatrix(rotation.q);

    Matrix matFinal = {};
    matFinal._11 = rotationMatrix.column0.x; matFinal._21 = rotationMatrix.column1.x; matFinal._31 = rotationMatrix.column2.x; matFinal._41 = 0.0f;
    matFinal._12 = rotationMatrix.column0.y; matFinal._22 = rotationMatrix.column1.y; matFinal._32 = rotationMatrix.column2.y; matFinal._42 = 0.0f;
    matFinal._13 = rotationMatrix.column0.z; matFinal._23 = rotationMatrix.column1.z; matFinal._33 = rotationMatrix.column2.z; matFinal._43 = 0.0f;
    matFinal._14 = 0.0f;                     matFinal._24 = 0.0f;                     matFinal._34 = 0.0f;                     matFinal._44 = 1.0f;


    Matrix matRot = GetOwner()->Transform()->GetRotateMat();


    return matFinal;
}

const Matrix& CPxRigidbody::GetPxWorldMatrix()
{
   
    //R * T
    Matrix matIdentiy = XMMatrixIdentity();
    //matScale *= GetOwner()->Transform()->GetRelativeScale();

    //회전
    Matrix matRot = GetRotMatrix();

    //위치
    Matrix matPos = GetPosMatrix();

    return (matIdentiy * matRot * matPos);
}

const Vec3& CPxRigidbody::GetPxPosition()
{
   PxVec3 vPos = m_pRigidbody->getGlobalPose().p;

   return Vec3(vPos.x, vPos.y, vPos.z);
}

const Vec3& CPxRigidbody::GetPxRotate()
{
    PxQuat vRot = m_pRigidbody->getGlobalPose().q;
   
    return Vec3(vRot.x, vRot.y, vRot.z);
}

void CPxRigidbody::AddPxPosition(Vec3 _vPos)
{
    PxVec3 vOffsetPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);

    PxTransform transform = m_pRigidbody->getGlobalPose();
    transform.p += vOffsetPos;

    m_pRigidbody->setGlobalPose(transform);
}

void CPxRigidbody::SetPxTransform(const Vec3& _vPos)
{
    PxVec3 vPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);
  
    PxTransform transform = m_pRigidbody->getGlobalPose();
    transform.p = vPos;

    m_pRigidbody->setGlobalPose(transform);
}

void CPxRigidbody::SetPxRotate(const PxQuat& _pQuat)
{
    PxTransform transform = m_pRigidbody->getGlobalPose();

    PxQuat rotation = transform.q;
    rotation = _pQuat;

    transform.q = rotation;
    //simulate() 실행 중에 setGlobalPose()를 부르면 안 됨 
    m_pRigidbody->setGlobalPose(transform);
}

void CPxRigidbody::friction_force()
{

}

void CPxRigidbody::tick_force(const PxVec3& _vFoce)
{
    //m_vAddGravityForce *= DT;
    m_pRigidbody->addForce(_vFoce + m_vAddGravityForce, m_eForceMode);
}

void CPxRigidbody::tick_velocity(const PxVec3& _vVel)
{
    PxVec3 vFinalVel = _vVel;
    if (m_bAddGravity)
    {
        m_vAddGravityForce += m_vAddGravityForce * 0.04f;
    
        vFinalVel = _vVel + m_vAddGravityForce;
    }
    
    m_pRigidbody->setLinearVelocity(vFinalVel);
}

void CPxRigidbody::SaveToLevelFile(FILE* _File)
{
    //m_pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    //m_pRigidbody->setAngularVelocity(PxVec3(0.f, 0.f, 5.f)); //각속도
    //m_pRigidbody->setAngularDamping(0.f);
}

void CPxRigidbody::LoadFromLevelFile(FILE* _FILE)
{
}

