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
#include "CEngineUI.h"
#include "CResMgr.h"

CEngineUI* CEquip::m_pEquipIcon = nullptr;
UINT CEquip::m_iEquipIdx = 0;
vector<wstring> CEquip::m_vecWeaponName = { L"default",L"axe" };

CEquip::CEquip() :
	CComponent(COMPONENT_TYPE::EQUIP),
	m_iIndex(38),
	m_matFinalBone{},
	m_pChar(nullptr),
	m_vFixedPos(Vec3::Zero)
{
	//처음 실행될 때만 weaponUI 생성
	if (m_pEquipIcon == nullptr)
	{
		m_pEquipIcon = new CEngineUI();

		wstring strWeapon = L"texture\\GameTexture\\" + m_vecWeaponName[m_iEquipIdx] + L".png";

		m_pEquipIcon->Initialize(strWeapon, m_vecWeaponName[m_iEquipIdx], Vec3(120.f, 150.f, 0.f));
		SpawnGameObject(m_pEquipIcon, Vec3(-280.f, -200.f, 0.f), (int)LAYER_TYPE::UI);
	}
}

CEquip::~CEquip()
{
	
}


void CEquip::SetEquipUI(int _iIdx)
{
	m_iEquipIdx = _iIdx;
	wstring strWeapon = m_vecWeaponName[m_iEquipIdx];
	strWeapon = L"texture\\GameTexture\\" + strWeapon + L".png";
	
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(strWeapon);
	m_pEquipIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, pTex);
}

void CEquip::begin()
{
	
	
}

void CEquip::SetFixedPos(Vec3 _vPos)
{
	//물체 주인이 없다면 assert

	CGameObject* pOwner = GetOwner();
	if (!m_pChar)
		assert(nullptr);

	m_vFixedPos = _vPos;

	Vec3 vScale = pOwner->Transform()->GetRelativeScale();
	Vec3 vRotate = pOwner->Transform()->GetRelativeRot();

	//크기
	Matrix matIdentity = XMMatrixIdentity();
	Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	//회전
	Matrix matRotate;
	matRotate = XMMatrixIdentity();
	matRotate = XMMatrixRotationX(vRotate.x);
	matRotate *= XMMatrixRotationY(vRotate.y);
	matRotate *= XMMatrixRotationZ(vRotate.z);

	//위치
	Matrix matTranslation = XMMatrixTranslation(_vPos.x, _vPos.y, _vPos.z);

	m_matStaticWorld = matScale * matRotate * matTranslation;
}

void CEquip::finaltick()
{	
	if (!m_pChar)
		return;

	if (!check_matrix())
		return;

	tick_equip();
}

bool CEquip::check_matrix()
{
	if (m_pChar->IsDead())
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

void CEquip::tick_equip()
{
	//기존 캐릭터의 위치 offset 값만큼 더하고 위치값 재계산
	Vec3 vPxPos = m_pChar->PxRigidbody()->GetPxPosition();
	Vec3 vPxOffset = m_pChar->PxRigidbody()->GetOffsetPosition();
	vPxPos += vPxOffset;

	//캐릭터 회전행랼
	Matrix matCharRotate = m_pChar->Transform()->GetDynamicRotate();
	
	Matrix PxPosMatrix = XMMatrixTranslation(vPxPos.x, vPxPos.y, vPxPos.z);
	Matrix matCharWorldScale = m_pChar->Transform()->GetWorldScaleMat();//캐릭터 크기 역행렬
	Matrix matCharSclaeInv = XMMatrixInverse(nullptr, matCharWorldScale);
	Matrix matCharWorld = matCharWorldScale * matCharRotate * PxPosMatrix; //새로 계산된 월드행렬
	
	Matrix matWeapon = matCharSclaeInv * m_matStaticWorld * m_matFinalBone * matCharWorld;

	//trasnform 행렬을 고정시켜서 움직여야함 (초기 위치를 기반으로 움직이게)
	Transform()->SetDependent(true);

	//여기서 만든 행렬로 transform에 전달
	Transform()->SetWorldMat(matWeapon);
}

void CEquip::SetDead(bool _bDelete)
{
	if (_bDelete)
	{
		DestroyObject(GetOwner()); //내 무기 삭제
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

