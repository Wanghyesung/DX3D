#include "pch.h"
#include "CCollider3D.h"

#include "CTransform.h"
#include "CScript.h"
CCollider3D::CCollider3D() :
	CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Shape(COLLIDER3D_TYPE::CUBE)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
	, m_matCollider3D(Matrix::Identity)
	, m_vOffsetScale(Vec3::One)
	, m_vOffsetRot(Vec3::Zero)
{
	SetName(L"Collider3D");
}

CCollider3D::~CCollider3D()
{

}

void CCollider3D::finaltick()
{
	// �浹 ȸ���� ������ ���
	assert(0 <= m_iCollisionCount);

	//ȸ��
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vOffsetRot.x);
	matRot *= XMMatrixRotationY(m_vOffsetRot.y);
	matRot *= XMMatrixRotationZ(m_vOffsetRot.z);

	m_matCollider3D = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matCollider3D *= matRot;
	m_matCollider3D *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	const Matrix& matWorld = Transform()->GetWorldMat();

	if (m_bAbsolute)
	{
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider3D = m_matCollider3D * matParentScaleInv * matWorld;
	}
	else
	{
		// �浹ü ���� * ������Ʈ ����
		m_matCollider3D *= matWorld;
	}

	// DebugShape ��û
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	if (COLLIDER3D_TYPE::CUBE == m_Shape)
		DrawDebugCube(m_matCollider3D, vColor, 0.f, true);
	else
		DrawDebugCircle(m_matCollider3D, vColor, 0.f);
}

const Vec3& CCollider3D::GetWorldPos()
{
	//��ġ ��ĸ�
	 return Vec3(m_matCollider3D._41, m_matCollider3D._42, m_matCollider3D._43);
}

void CCollider3D::BeginOverlap(CCollider3D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	
	for (int i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider3D::OnOverlap(CCollider3D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();

	for (int i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider3D::EndOverlap(CCollider3D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();

	for (int i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
}


