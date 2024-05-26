#include "pch.h"
#include "CEquip.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CCollider3D.h"
#include "CPxRigidbody.h"
CEquip::CEquip() :
	CComponent(COMPONENT_TYPE::EQUIP),
	m_iIndex(38),
	m_matFinalBone{},
	m_pChar(nullptr),
	m_vFixedPos(Vec3::Zero)
{
	
}

CEquip::~CEquip()
{
}


void CEquip::begin()
{
	
	
}

void CEquip::SetFixedPos(Vec3 _vPos)
{
	//��ü�� ���� ����� �ʱ⿡ ����
	//�� ����� ������� ���Ⱑ �����̰�
	
	//��ü ������ ���ٸ� assert

	CGameObject* pOwner = GetOwner();
	if (!m_pChar)
		assert(nullptr);

	m_vFixedPos = _vPos;

	Vec3 vScale = pOwner->Transform()->GetRelativeScale();
	Vec3 vRotate = pOwner->Transform()->GetRelativeRot();

	//ũ��
	Matrix matIdentity = XMMatrixIdentity();
	Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	//ȸ��
	Matrix matRotate;
	matRotate = XMMatrixIdentity();
	matRotate = XMMatrixRotationX(vRotate.x);
	matRotate *= XMMatrixRotationY(vRotate.y);
	matRotate *= XMMatrixRotationZ(vRotate.z);

	//��ġ
	Matrix matTranslation = XMMatrixTranslation(_vPos.x, _vPos.y, _vPos.z);

	m_matStaticWorld = matScale * matRotate * matTranslation;
}

void CEquip::finaltick()
{	
	if (!m_pChar)
		return;

	if (!check_matrix())
		return;

	const vector<CGameObject*> vecChild = GetOwner()->GetChild();

	if (vecChild.size() == 0)
		single_tick();
	else
		child_tick();
}

bool CEquip::check_matrix()
{
	if (m_pChar == nullptr)
		return false;
	
	else if (m_pChar->IsDead())
	{
		m_pChar = nullptr;
		return false;
	}

	CAnimator3D* pAnim = m_pChar->GetChild().at(0)->Animator3D();

	bool bOn = pAnim->IsFinalMatUpdate();
	if (bOn)
	{
		UINT iBoneCount = pAnim->GetBoneCount();
		m_vecFinalBone.resize(iBoneCount);

		pAnim->GetFinalBoneMat()->GetData(m_vecFinalBone.data());

		m_matFinalBone = m_vecFinalBone[m_iIndex];
		m_matFinalBone.m[3][3] = 1;

		m_matFinalBone = XMMatrixTranspose(m_matFinalBone);

		return true;
	}

	return false;
}

void CEquip::single_tick()
{
	//Matrix matWeaponWorld = Transform()->GetWorldMat();//����

	Matrix matCharWorldScale = m_pChar->Transform()->GetWorldScaleMat();//ĳ���� ũ�� �����
	Matrix matCharSclaeInv = XMMatrixInverse(nullptr, matCharWorldScale);
	Matrix matCharWorld = m_pChar->PxRigidbody()->GetPxWorldMatrix(); //R*T GetWorldMat(); //ĳ���� ����
	Matrix matWeapon = matCharSclaeInv * m_matStaticWorld * m_matFinalBone * matCharWorld;

	//trasnform ����� �������Ѽ� ����������(��ġ�� ��� ����)
	Transform()->SetDependent(true);

	Transform()->SetWorldMat(matWeapon);
}

void CEquip::child_tick()
{
	Matrix matWeaponWorld = Transform()->GetWorldMat();//����

	Matrix matCharWorldScale = m_pChar->Transform()->GetWorldScaleMat();//ĳ���� ũ�� �����
	Matrix matCharSclaeInv = XMMatrixInverse(nullptr, matCharWorldScale);
	Matrix matCharWorld = m_pChar->Transform()->GetWorldMat(); //ĳ���� ����
	//Vec3 vCharPos = m_pChar->PxRigidbody()->GetPxPosition();
	//Matrix matCharWorld = matCharWorldScale * m_pChar->Transform()->GetRotateMat() *
	//	XMMatrixTranslation(vCharPos.x, vCharPos.y, vCharPos.z);
	
	//temCharWorld *= m_pChar->PxRigidbody()->GetPosMatrix();

	Matrix matWeapon = matCharSclaeInv * matWeaponWorld * m_matFinalBone * matCharWorld;

	const vector<CGameObject*> vecChild = GetOwner()->GetChild();
	//���������� �θ� trasnform�� �ǵ����� ����
	for (int i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->Transform()->SetDependent(true);//final_tick���� ������� ���� ������ �ʰ�
		vecChild[i]->Transform()->SetWorldMat(matWeapon);
	}
}

void CEquip::SetDead(bool _bDelete)
{
	if (_bDelete)
	{
		DestroyObject(GetOwner()); //�� ���� ����
		return;
	}
	m_pChar = nullptr;
}

void CEquip::SaveToLevelFile(FILE* _File)
{

}

void CEquip::LoadFromLevelFile(FILE* _FILE)
{

}

