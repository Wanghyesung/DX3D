#include "pch.h"
#include "CRigidbody.h"

#include "CTimeMgr.h"
#include "CTransform.h"
CRigidbody::CRigidbody() :
	CComponent(COMPONENT_TYPE::RIGIDBODY),
	m_fMass(1.f),
	m_vAccelation(Vec3::Zero),
	m_vMaxVelocity(Vec3(500.f, 500.f, 500.f)),
	m_vGravity(Vec3(0.f, -9.6f, 0.f)),
	m_bGround(true),
	m_bAccumulate(false)
{

}



CRigidbody::~CRigidbody()
{

}

void CRigidbody::SaveToLevelFile(FILE* _File)
{

}

void CRigidbody::LoadFromLevelFile(FILE* _FILE)
{

}

void CRigidbody::finaltick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	float fLen = m_vForce.Length();

	if (fLen != 0.f)
	{
		//f = ma
		// a = f/m
		m_vForce.Normalize();
		float fAcc = fLen / m_fMass;

		m_vAccelation = (m_vForce * fAcc);
	}
	else
	{
		m_vAccelation = Vec3::Zero;
	}


	if (m_bGround)
	{
		if (m_bAccumulate)
		{
			m_vVelocity += (m_vAccelation * DT);
			m_vVelocity.y = 0.f;
		}
		else
			m_vVelocity = (m_vAccelation * DT);

		//내적
		Vec3 vGravity = m_vGravity;
		vGravity.Normalize();

		float dot = m_vVelocity.Dot(vGravity);
		m_vVelocity += vGravity * dot;

		vPos += m_vVelocity;
	}

	//점프
	else
	{
		//중력 가속도
		m_vVelocity += (m_vAccelation * DT);
		m_vVelocity.y += (m_vGravity.y * DT);

		vPos += (m_vVelocity * DT);
	}

	//if (m_vMaxVelocity.Length() <= m_vVelocity.Length())
	//{
	//	m_vVelocity = m_vMaxVelocity;
	//}
	//

	Transform()->SetRelativePos(vPos);

	m_vAccelation = Vec3::Zero;
	m_vForce = Vec3::Zero;
}
