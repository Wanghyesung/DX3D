#include "pch.h"
#include "CPxRigidbody.h"
#include "CPhysxMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
CPxRigidbody::CPxRigidbody():
	CComponent(COMPONENT_TYPE::PXRIGIDBODY),
    m_bGround(true),
    m_bAccumulate(false)
{
    m_eForceMode = PxForceMode::eACCELERATION;
}

CPxRigidbody::~CPxRigidbody()
{
   
}



void CPxRigidbody::finaltick()
{
	CTransform* pTrasnform = GetOwner()->Transform();

    if (m_bDecrease)
    {
        m_vVelocity -=m_vDecVel;
    }

	PxVec3 vFoce = PxVec3(m_vForce.x, m_vForce.y, m_vForce.z);
	PxVec3 Vvel = PxVec3(m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);

    //누적해서 들어갈지
   if(m_bAccumulate)
        tick_force(vFoce);
   
   else
        tick_velocity(Vvel);

   PxTransform transform = m_pRigidbody->getGlobalPose();
   
   Vec3 vPos = Vec3(transform.p.x, transform.p.y, transform.p.z);
   pTrasnform->SetRelativePos(vPos);
   
    m_vForce = Vec3::Zero;
    m_vVelocity = Vec3::Zero;
}

void CPxRigidbody::init(const Vector3& _vPos, const Vector3& _vScale, eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups)
{
    m_pRigidbody = CPhysxMgr::GetInst()->GetRigidDynamic(_vPos, _vScale, _eGroups, _eOtherGroups);
   
    m_pRigidbody->setMass(1.f);  
}

void CPxRigidbody::SetMass(float _fMass)
{
    m_fMass = _fMass;
    m_pRigidbody->setMass(_fMass);
}

void CPxRigidbody::SetGround(bool _bGround)
{
    m_bGround = _bGround; 

    m_pRigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_bGround);
}

void CPxRigidbody::SetMaxVelocity(float _fMaxVelocity)
{
    m_pRigidbody->setMaxLinearVelocity(_fMaxVelocity);
}

const Matrix& CPxRigidbody::GetPosMatrix()
{
    PxTransform transform = m_pRigidbody->getGlobalPose(); // actor는 PxRigidActor* 포인터입니다.

    Matrix mat = XMMatrixTranslation(transform.p.x, transform.p.y, transform.p.z);

    return mat;
}

const Matrix& CPxRigidbody::GetRotMatrix()
{
    PxTransform rotation = m_pRigidbody->getGlobalPose(); // actor는 PxRigidActor* 포인터입니다.
    //회전
    Matrix matRot = XMMatrixIdentity();
    matRot = XMMatrixRotationX(rotation.q.x);
    matRot *= XMMatrixRotationY(rotation.q.y);
    matRot *= XMMatrixRotationZ(rotation.q.z);

    return matRot;
}

void CPxRigidbody::friction_force()
{

}

void CPxRigidbody::tick_force(const PxVec3& _vFoce)
{
    m_pRigidbody->addForce(_vFoce, m_eForceMode);
}

void CPxRigidbody::tick_velocity(const PxVec3& _vVel)
{
    m_pRigidbody->setLinearVelocity(_vVel);
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

