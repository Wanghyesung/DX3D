#pragma once
#include "CComponent.h"

class CRigidbody : public CComponent
{
public:
	CRigidbody();
	~CRigidbody();


	CLONE(CRigidbody);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

public:
	virtual void finaltick();

	void SetMass(float _fMass) { m_fMass = _fMass; }
	void SetGround(bool _bGround) { m_bGround = _bGround; }

	bool IsGround() { return m_bGround; }

	void SetVelocity(Vector3 _vVelocity) { m_vVelocity = _vVelocity; }
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; }
	Vector3 GetVelocity() { return m_vVelocity; }
	void SetMaxVelocity(Vector3 _vMax) { m_vMaxVelocity = _vMax; }

	void AddForce(Vector3 _vForce) { m_vForce = _vForce; }

	void SetAcumulate(bool _bAcc) { m_bAccumulate = _bAcc; }
private:
	Vec3 m_vMaxVelocity;
	Vec3 m_vVelocity;
	Vec3 m_vAccelation;
	Vec3 m_vForce;

	Vec3 m_vGravity;

	bool m_bAccumulate;//힘들 누적해서 받을지
	bool m_bGround;

	float m_fMass;

};

