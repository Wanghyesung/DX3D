#include "pch.h"
#include "CMonsterHPScript.h"
#include <Engine\CGage.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLayer.h>
#include <Engine\CPxRigidbody.h>
#include <Engine\CRespawnMgr.h>
#include <Engine\CFontMgr.h>



CMonsterHPScript::CMonsterHPScript():
	CScript(SCRIPT_TYPE::MONSTERHPSCRIPT),
	m_bActive(true),
	m_vOffsetPos(Vec3(0.f,200.f,0.f))
{

}

CMonsterHPScript::~CMonsterHPScript()
{

}

void CMonsterHPScript::tick()
{
	if(m_bBoss)
		CFontMgr::GetInst()->AddFont(m_strHangleFontName, 350, 670, 20, FONT_RGBA(255, 255, 255, 255));

	//3d 공간 2d UI 빌보드 처리
	if (!m_bBoss)
	{
		move();

		m_pHP->UpdateBillboard();
		m_pFrame->UpdateBillboard();
	}

}

void CMonsterHPScript::UpdateGage(float _fMaxGage, float _fCurGage)
{
	m_pHP->UpdateGage(_fMaxGage, _fCurGage);

	//spanwmgr에 저장 level에서 제외
	if (_fCurGage <= 0.f)
	{
		CRespawnMgr::GetInst()->AddObject(m_pFrame, 10.f, Vec3::Zero);
		CRespawnMgr::GetInst()->AddObject(m_pHP, 10.f, Vec3::Zero);
		CRespawnMgr::GetInst()->AddObject(GetOwner(), 10.f, Vec3::Zero);
	}
}

void CMonsterHPScript::begin()
{

	//const vector<CGameObject*>& vecObj =
	//	CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Camera)->GetParentObject();
	//if (vecObj[0] != nullptr)
	//	m_pTarget = vecObj[0];

}

void CMonsterHPScript::Initialize(const wstring& _strTexName, const wstring& _strName, const wstring& _strHangleName,
	const Vec3& _vScale , bool _bBoss)
{
	m_strMonsterName = _strName;
	m_strHangleFontName = _strHangleName;
	m_bBoss = _bBoss;

	wstring strFrameName = _strName + L"Frame";
	m_pFrame = new CEngineUI();
	m_pFrame->Initialize(_strTexName + L"Frame.png", strFrameName, _vScale);

	wstring strHPName = _strName + L"HP";
	m_pHP = new CGage();
	m_pHP->Initialize(_strTexName + L"HP.png", strHPName, _vScale);

	//m_pFrame->AddChild(m_pHP);

	if (m_bBoss)
	{
		//고정 좌표
		SpawnGameObject(m_pHP, Vec3(140.f,-330.f,-3.f), (int)LAYER_TYPE::UI);
		SpawnGameObject(m_pFrame, Vec3(140.f, -330.f, -2.f), (int)LAYER_TYPE::UI);
	}
	else
	{
		//몬스터에 따라 움직이기
		SpawnGameObject(m_pHP, Vec3::Zero, (int)LAYER_TYPE::Default);
		SpawnGameObject(m_pFrame, Vec3::Zero, (int)LAYER_TYPE::Default);
	}

	m_pHP->GetRenderComponent()->SetActiveShadow(false); //그림자 반영 X
	m_pFrame->GetRenderComponent()->SetActiveShadow(false); //그림자 반영 X

}

void CMonsterHPScript::move()
{
	Vec3 vPos = m_pMonster->PxRigidbody()->GetPxPosition();
	vPos += m_vOffsetPos;

	m_pFrame->Transform()->SetRelativePos(vPos);
	m_pHP->Transform()->SetRelativePos(vPos);
}



