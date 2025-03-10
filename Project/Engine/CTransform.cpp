#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f) }
	, m_vUp(Vec3(0.f, 1.f, 0.f))
	, m_vFoward(Vec3(0.f, 0.f, 1.f))
	, m_vRight(Vec3(1.f, 0.f, 0.f))
	, m_bDependent(false)
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}


Matrix& CTransform::GetDynamicRotate()
{
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	return matRot;
}

Vec3 CTransform::GetDynamicFoward()
{
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	Vec3 fForward = Vec3::TransformNormal(Vec3::Forward, matRot);

	return fForward;
}



Vec3 CTransform::GetDynamicUp()
{
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	Vec3 fUp = Vec3::TransformNormal(Vec3::Up, matRot);

	return fUp;
}

void CTransform::finaltick()
{
	if (m_bDependent)
		return;

	//이전프레임 월드행렬 저장
	m_matPrevWorld = m_matWorld;

	//크기
	m_matWorldScale = XMMatrixIdentity();
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	//회전
	m_matRot = XMMatrixIdentity();
	m_matRot = XMMatrixRotationX(m_vRelativeRot.x);
	m_matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	m_matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	//위치
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	//방향벡터 구하기
	//m_vUp = Vec3::TransformNormal(Vec3::Up, matRot);
	//m_vFoward = Vec3::TransformNormal(Vec3::Forward, matRot);
	//m_vRight = Vec3::TransformNormal(Vec3::Right, matRot);


	m_matWorld = m_matWorldScale * m_matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matRot);
	}

	// 부모 오브젝트 확인
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale = pParent->Transform()->m_matWorldScale; //부모가 크기를 조절하면 자식도 동일한 비율로 변화
			m_matWorld *= pParent->Transform()->m_matWorld;
		}


		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	// 위치값을 상수버퍼에 전달 및 바인딩		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	
	g_transform.matPrevWorld = m_matPrevWorld;

	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}
