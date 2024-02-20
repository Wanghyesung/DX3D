#pragma once
#include "CComponent.h"
#include <Physx\PxPhysicsAPI.h>


using namespace physx;

class CPxRigidbody : public CComponent
{
private:
	//물체 속도 업데이트
	PxRigidDynamic* m_pRigidbody;
	PxMaterial* m_pPxMaterial;
	
	PxForceMode::Enum m_eForceMode;
	Vec3 m_vVelocity;
	Vec3 m_vDecVel; //속도 감소
	Vec3 m_vForce;

	Vec3 m_vPxScale;

	float m_fMaxVelocity;
	bool m_bAccumulate;//힘들 누적해서 받을지
	bool m_bGround; //땅인지;

	bool m_bDecrease; //속도 감소

	float m_fMass;

public:
	void SetMass(float _fMass);
	void SetGround(bool _bGround);
	bool IsGround() { return m_bGround; }

	void SetVelocity(Vector3 _vVelocity) { m_vVelocity = _vVelocity; }
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; }

	void SetDecVel(Vec3 _vDecVel) { m_vDecVel = _vDecVel; }

	Vector3 GetVelocity() { return m_vVelocity; }
	void SetMaxVelocity(float _fMaxVelocity);

	void AddForce(Vector3 _vForce) { m_vForce += _vForce; }

	void SetDecrease(bool _bDecrease) { m_bDecrease = _bDecrease; }
	void SetAccumulate(bool _bAccumulate) { m_bAccumulate = _bAccumulate; }

	const Matrix& GetPosMatrix();
	const Matrix& GetRotMatrix();

	void SetForceMode(PxForceMode::Enum _eMode) { m_eForceMode = _eMode; }
private:
	void friction_force();

	void tick_force(const PxVec3& _vFoce);
	void tick_velocity(const PxVec3& _vVel);
public:

	virtual void finaltick()override;
	
	void init(const Vector3& _vPos, const Vector3& _vScale, eCollisionGroups _eGroups, eCollisionGroups _eOtherGroups);

	CLONE(CPxRigidbody);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

public:
	CPxRigidbody();
	virtual ~CPxRigidbody();
};

