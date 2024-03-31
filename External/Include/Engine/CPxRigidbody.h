#pragma once
#include "CComponent.h"
#include <Physx\PxPhysicsAPI.h>


using namespace physx;

//실제 움직임은 physx에서 하고 렌더링 위치는 transform

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
	Vec3 m_vTransformOffset;//실제 충돌에 영향을 주는 PXTransform과 그릴 위치를 나타내는 transform으로 구분

	PxVec3 m_vAddGravityForce;//땅에서 떨어졌을때 중력 가속도 영향을 받게

	float m_fMaxVelocity;
	
	bool m_bAccumulate;//힘들 누적해서 받을지
	bool m_bGround; //땅인지;
	bool m_bBasicGravity;

	bool m_bPass;

	bool m_bDecrease; //속도 감소

	float m_fMass;

public:
	void SetMass(float _fMass);
	void SetGround(bool _bGround , bool _bBasicGravity = true);
	void SetPass(bool _bPass) { m_bPass = _bPass; }

	bool IsGround() { return m_bGround; }
	bool IsPass() { return m_bPass; }

	void ChanageMaterial(UINT _iStaticCoef, UINT _iDynamicCoef);

	void SetVelocity(Vec3 _vVelocity) { m_vVelocity = _vVelocity; }
	void AddVelocity(Vec3 _vVelocity) { m_vVelocity += _vVelocity; }
	void SetDecVel(Vec3 _vDecVel) { m_vDecVel = _vDecVel; }
	void ClearFoce();

	Vec3 GetVelocity() { return m_vVelocity; }
	void SetMaxVelocity(float _fMaxVelocity);

	void AddForce(Vec3 _vForce) { m_vForce += _vForce; }

	void SetDecrease(bool _bDecrease) { m_bDecrease = _bDecrease; }
	void SetAccumulate(bool _bAccumulate) { m_bAccumulate = _bAccumulate; }

	const Matrix& GetPosMatrix();
	const Matrix& GetRotMatrix();

	const Vec3& GetPxPosition();
	const Vec3& GetPxRotate();

	void SetOffsetPosition(const Vec3& _vOffsetPos) { m_vTransformOffset = _vOffsetPos; }
	const Vec3& GetOffsetPosition() { return m_vTransformOffset; }

	void AddPxPosition(Vec3 _vPos);

	void SetForceMode(PxForceMode::Enum _eMode) { m_eForceMode = _eMode; }

	void SetPxTransform(const Vec3& _vPos);
	void SetPxRotate(const PxQuat& _pQuat);
	

private:
	void friction_force();

	void tick_force(const PxVec3& _vFoce);
	void tick_velocity(const PxVec3& _vVel);
public:

	virtual void finaltick()override;
	
	void init(const Vector3& _vPos, const Vector3& _vScale, int _iLayer, CGameObject* _pCollEventObj = nullptr);

	CLONE(CPxRigidbody);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

public:
	CPxRigidbody();
	virtual ~CPxRigidbody();
};

