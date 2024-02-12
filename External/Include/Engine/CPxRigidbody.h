#pragma once
#include "CComponent.h"
#include <Physx\PxPhysicsAPI.h>


using namespace physx;

class CPxRigidbody : public CComponent
{
private:
	//��ü �ӵ� ������Ʈ
	PxRigidDynamic* m_pRigidbody;
	PxMaterial* m_pPxMaterial;
	
	Vec3 m_vVelocity;
	Vec3 m_vForce;

	float m_fMaxVelocity;
	bool m_bAccumulate;//���� �����ؼ� ������
	bool m_bGround; //������;

	float m_fFricCoeff;//������ ����

	bool m_bFricoeff;

public:
	void SetMass(float _fMass);
	void SetGround(bool _bGround);
	bool IsGround() { return m_bGround; }

	void SetFricoeff(bool _bFricoeff) { m_bFricoeff = _bFricoeff; }

	void SetVelocity(Vector3 _vVelocity) { m_vVelocity = _vVelocity; }
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; }

	Vector3 GetVelocity() { return m_vVelocity; }
	void SetMaxVelocity(float _fMaxVelocity);

	void AddForce(Vector3 _vForce) { m_vForce += _vForce; }

	void SetAcumulate(bool _bAcc) { m_bAccumulate = _bAcc; }

private:
	void friction_force();

	void tick_force(const PxVec3& _vFoce);
	void tick_velocity(const PxVec3& _vVel);
public:
	virtual void finaltick()override;
	
	void init();

	CLONE(CPxRigidbody);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

public:
	CPxRigidbody();
	virtual ~CPxRigidbody();
};

