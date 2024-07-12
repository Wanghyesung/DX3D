#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CFSM.h>
#include <Engine\CEngineUI.h>
#include <Engine\CGage.h>

#include <Engine\CMonsterFSM.h>
#include <Engine\CMonsterJump.h>
#include <Engine\CMonsterIdle.h>
#include <Engine\CMonsterMove.h>
#include <Engine\CMonsterAttack.h>
#include <Engine\CMonsterHit.h>
#include <Engine\CMonsterDead.h>

#include <Engine\CDemonIdle.h>
#include <Engine\CDemonHit.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CPhysxMgr.h>
#include <Engine\CNavMeshMgr.h>


#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CDemonScript.h>
#include <Script\CCameraMoveScript.h>
#include <Script\CTerrainScript.h>
#include <Script\CLandFormScript.h>
#include <Script\CEquipScript.h>
#include <Script\CObjstacleScript.h>
#include <Script\CBossStageScript.h>
#include <Script\CLandScpaeScript.h>
#include <Script\CStairsScript.h>

#include "CLevelSaveLoad.h"


#include <Engine\CCollisionMgr.h>
#include <Engine\CSetColorShader.h>


void CreateTestLevel()
{
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::Attack);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Player,(UINT)LAYER_TYPE::MonsterAttack);

	//CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Monster);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::LandScape);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::LandScape);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Stairs);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Player, (UINT)LAYER_TYPE::Obstacle);
	CPhysxMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Monster, (UINT)LAYER_TYPE::Obstacle);


	//return;

	// 컴퓨트 쉐이더 테스트
	//Ptr<CTexture> pTestTexture =
	//	CResMgr::GetInst()->CreateTexture(L"ComputeTestTex"
	//		, 200, 200, DXGI_FORMAT_R8G8B8A8_UNORM
	//		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
	//		, D3D11_USAGE_DEFAULT);
	//
	//Ptr<CSetColorShader> pCS = (CSetColorShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"SetColorCS").Get();
	//pCS->SetTargetTexture(pTestTexture);
	//pCS->SetColor(Vec3(1.f, 0.f, 1.f));
	//pCS->Execute();


	//Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\BGM_Stage1.wav");
	//pSound->Play(1, 0.5f, false);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// 오브젝트 생성
	//CGameObject* pPlayer = new CGameObject;
	//pPlayer->SetName(L"Player");
	//pPlayer->AddComponent(new CTransform);
	//pPlayer->AddComponent(new CMeshRender);
	////pPlayer->AddComponent(new CCollider3D);
	//
	//
	//pPlayer->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	//pPlayer->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	//
	////SphereMesh
	//pPlayer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//pPlayer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	//pPlayer->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	//pPlayer->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));
	////pPlayer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	////pPlayer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	////pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEX_CUBE_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));
	//
	//pPlayer->AddComponent(new CMotionBlur);
	//pPlayer->MotionBlur()->Initialize();
	//
	//SpawnGameObject(pPlayer, Vec3(0.f, 2000.f, 0.f), (int)LAYER_TYPE::Player);


	//skybox 추가
	CGameObject* pSkyBox = new CGameObject();
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightColor(Vec3(0.8f, 0.8f, 0.8f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.3f, 0.3f, 0.3f));

	SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);

	//pLightObj->SetName(L"Point Light 2");
	//
	//pLightObj->AddComponent(new CTransform);
	//pLightObj->AddComponent(new CLight3D);
	//
	//pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	//pLightObj->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	//pLightObj->Light3D()->SetRadius(500.f);
	//pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	//SpawnGameObject(pLightObj, Vec3(250.f, -750.f, 0.f), 0);


	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Plane");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//
	//pObject->Transform()->SetRelativeScale(Vec3(8000.f, 8000.f, 8000.f));
	//pObject->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	//
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));
	//
	//SpawnGameObject(pObject, Vec3(0.f, -1000.f, 0.f), L"Default");

	
	CGameObject* pAritorias = new CGameObject;
	pAritorias->SetName(L"Aritorias");
	pAritorias->AddComponent(new CTransform());
	
	CPxRigidbody* pRigi = new CPxRigidbody();
	pAritorias->AddComponent(pRigi);
	pRigi->init(Vec3(100.f, 60.f, 100.f), Vec3(115.f, 115.f, 225.f), (int)LAYER_TYPE::Player, pAritorias);
	pRigi->SetOffsetPosition(Vec3(0.f, -120.f, 0.f));
	//1591 2524 2941
	CCollider3D* pCollider = new CCollider3D();
	pAritorias->AddComponent(pCollider);
	//pCollider->SetAbsolute(true);
	pCollider->SetOffsetScale(Vec3(115.f, 115.f, 225.f));
	
	
	CPlayerScript* pScript = new CPlayerScript(); 
	pAritorias->AddComponent(pScript);
	pScript->Initialize(); //fbx load, FSM , components
	
	//pAritorias->AddComponent(new CMotionBlur());
	//pAritorias->MotionBlur()->Initialize();
	
	SpawnGameObject(pAritorias, Vec3(1000.f, 700.f, 200.f), (int)LAYER_TYPE::Player);
	//774 999

	CreateMonster();

	
	//CNavMeshMgr::GetInst()->init();

	//Vec3 vStagePos = Vec3(3000.f, 3573.f, 3000.f);// stage worldpos
	//
	////몬스터
	//Vec3 vMonsterPos = Vec3(1732.f, 120.f, 4000.f);
	//
	////플레이어
	//Vec3 vPlayerPos = Vec3(1800.f, 700.f, 2500.f);
	//
	//vMonsterPos -= vStagePos;
	//vPlayerPos -= vStagePos;

	//플레이어 역행렬, 몬스터 역행렬을 가져와야함
	
	//XMVector3TransformCoord(vMonsterPos,)

	//월드 좌표에서 + 2000 2000하고 worldInv곱해서 로컬좌표로 계산 후 다시 월드좌표를 곱하기
	//vector<Vec3> vecPos = CNavMeshMgr::GetInst()->FindPath(Vec3(224, -937.f, -1400.f), Vec3(-170.f, -973.f, -1537.f));

	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	CCameraMoveScript* pCameraScript = new CCameraMoveScript();
	pCameraScript->SetTarget(pAritorias); //카메라가 바라보는 물체 등록
	pMainCam->AddComponent(pCameraScript);
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex((int)CAMERA_TYPE::MAIN);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask((int)LAYER_TYPE::UI, false);// UI Layer 는 렌더링하지 않는다.


	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Camera);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex((int)CAMERA_TYPE::UI);		// Sub 카메라로 지정
	pUICam->Camera()->SetLayerMask((int)LAYER_TYPE::UI, true);	// 31번 레이어만 체크

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Camera);
	
	CreateLandScape();
	

	//pObject = new CGameObject;
	//pObject->SetName(L"Decal");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CDecal);
	//
	//pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	//
	//pObject->Decal()->SetDeferredDecal(true);
	//pObject->Decal()->ActivateEmissive(true);
	//
	//SpawnGameObject(pObject, Vec3(0.f, 0.f, 500.f), L"Default");

	// 광원 추가 spotlight
	//CGameObject* pLightObj = new CGameObject;
	//pLightObj->SetName(L"Point Light 1");
	//
	//pLightObj->AddComponent(new CTransform);
	//pLightObj->AddComponent(new CLight3D);
	//
	//pLightObj->Light3D()->SetLightType(LIGHT_TYPE::SPOT);
	//pLightObj->Light3D()->SetLightColor(Vec3(1.f, 0.2f, 0.2f));
	////pLightObj->Light3D()->SetLightAmbient(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light3D()->SetAngle(XM_PI / 3.f);
	//pLightObj->Light3D()->SetRadius(300.f);
	//
	//SpawnGameObject(pLightObj, Vec3(300.f, -400.f, 0.f), 0);

	//pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	//pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));	
	//pLightObj->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));

	//pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light3D()->SetLightAmbient(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light3D()->SetRadius(1000.f);
	//pLightObj->Light3D()->SetLightAmbient(Vec3(1.f, 1.f, 1.f));
	//SpawnGameObject(pLightObj, Vec3(-130.f, -130.f, 0.f), 0);

	//pObject = new CGameObject;
	//pObject->SetName(L"Tess Object");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//
	//pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	//
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TessMtrl"));
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\Sky01.png"));
	//SpawnGameObject(pObject, Vec3(0.f, 0.f, 100.f), L"Default");


	//CNavMeshMgr::GetInst()->Init_Map();
}

void CreateMonster()
{
	//navmesh
	CNavMeshMgr::GetInst()->CreatePlane({ 1000, 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
	CNavMeshMgr::GetInst()->CreatePlane({ 3000 , 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
	CNavMeshMgr::GetInst()->CreatePlane({ 1700 , 0, 5500 }, Vec3(1000.f, 0.f, 7000.f));

	//Vec3 m_vCreateScale = Vec3(400.f, 0.f, 400.f); //+= (DT * Vec3(2.f,2.f,2.f));
	//Vec3 vHalfScale = m_vCreateScale / 2.f;
	//Vec3 m_vRayPoint{ 400, 0.f, 900 };
	//float fHeight = 400.f;
	////오브젝트를 설치하기 위해서 높이값을 크기의 /2 만큼 올린다
	//float y = m_vRayPoint.y + fHeight / 2.f;
	//Vec3 vPos = Vec3(m_vRayPoint.x, y, m_vRayPoint.z);
	//
	//Vec3 vDir[4] = { Vec3(0.f,0.f,1.f), Vec3(0.f,0.f, -1.f),
	//	Vec3(1.f, 0.f, 0.f) ,Vec3(-1.f, 0.f , 0.f) };
	//
	//Vec3 vScaleDir[4] = { Vec3(1.f,0.f,0.f), Vec3(1.f,0.f,0.f),
	//	 Vec3(0.f,0.f,1.f) , Vec3(0.f,0.f,1.f) };
	//
	//for (int i = 2; i < 4; ++i)
	//{
	//	//정행진 방향으로 메쉬 위치 지정
	//	Vec3 vPlanePos = vDir[i] * vHalfScale + vPos;
	//
	//	//크기 적용
	//	Vec3 vScale = vScaleDir[i] * m_vCreateScale;
	//	vScale.y += fHeight; //높이값 적용
	//
	//	
	//	CNavMeshMgr::GetInst()->CreatePlane(vPlanePos, vScale);
	//}
	//CNavMeshMgr::GetInst()->CreatePlane({ 1275 , 350, 5490 }, Vec3(0, 700.f, 7000.f));
	//CNavMeshMgr::GetInst()->CreatePlane({ 1700 , 0, 2400 }, Vec3(1300.f, 0.f, 800.f));
	//CNavMeshMgr::GetInst()->CreatePlane({ 1000 , 100, 1000 }, Vec3(700.f, 200.f, 0.f));
	
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Balder_Knight");
	pMonster->AddComponent(new CTransform());
	pMonster->AddComponent(new CCollider3D());
	pMonster->Collider3D()->SetOffsetScale(Vec3(150.f, 340.f, 150.f));	
	{
		//수정 필요
		tBuildSettings tSetting = {};
		tSetting.agentRadius = 100.f; // 900 / 5 = 180
	    tSetting.ID = pMonster->GetID();
		CNavMeshMgr::GetInst()->BuildField(tSetting);
	
		CRDNavMeshField* pNav = new CRDNavMeshField();
		pMonster->AddComponent(pNav);
		pNav->SetRadius(tSetting.agentRadius);
	}
	
	CPxRigidbody* pRigi = new CPxRigidbody();
	pMonster->AddComponent(pRigi);
	pRigi->init(Vec3(1000.f, 120.f, 1000.f), Vec3(150.f, 340.f, 150.f), (int)LAYER_TYPE::Monster, pMonster);
	pRigi->SetOffsetPosition(Vec3(0.f, -170.f, 0.f));
	
	
	CMonsterScript* pMonsterScript = new CMonsterScript();
	pMonster->AddComponent(pMonsterScript);
	pMonsterScript->Initialize(L"Balder_Knight"); //fbx load, FSM , components
	
    pMonsterScript->AddAnimFrame(L"Hit", 431, 481);
	pMonsterScript->AddAnimFrame(L"HitDown", 482, 602);
	pMonsterScript->AddAnimFrame(L"Attack0", 603, 687);
	pMonsterScript->AddAnimFrame(L"Attack1", 759, 850);
	//pMonsterScript->AddAnimFrame(L"Dead", 1116, 1200);
	
	CMonsterIdle* pIdle = new CMonsterIdle();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 0, 95);
	
	CMonsterMove* pMove = new CMonsterMove();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::WALK,  pMove, L"Walk", 96, 140);
	
	CMonsterAttack* pAttack = new CMonsterAttack();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::ATTACK, pAttack, L"Attack");
	
	CMonsterHit* pHit = new CMonsterHit();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit");
	
	CMonsterDead* pMonsterDead = new CMonsterDead();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::DEAD, pMonsterDead, L"Dead", 1116, 1200);
	
	pMonsterScript->AddMonsterAttack(0, 0.f, 0.f, 0.6f, 629, 634, Vec3(150.f, 150.f, 150.f), 90.f, Vec3::Zero);
	pMonsterScript->AddMonsterAttack(1, 0.f, 0.f, 0.6f, 781, 785, Vec3(170.f, 170.f, 170.f), 100.f, Vec3::Zero);
	
	pMonster->Transform()->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	SpawnGameObject(pMonster, Vec3(1000.f, 120.f, 1000.f), (int)LAYER_TYPE::Monster);
	

	//pMonster = new CGameObject;
	//pMonster->SetName(L"Balder_Knight_2");
	//pMonster->AddComponent(new CTransform());
	//pMonster->AddComponent(new CCollider3D());
	//pMonster->Collider3D()->SetOffsetScale(Vec3(150.f, 340.f, 150.f));
	//{
	//	tBuildSettings tSetting = {};
	//	tSetting.agentRadius = 450.f; // 900 / 5 = 180
	//	tSetting.ID = pMonster->GetID();
	//	CNavMeshMgr::GetInst()->BuildField(tSetting);
	//
	//	CRDNavMeshField* pNav = new CRDNavMeshField();
	//	pMonster->AddComponent(pNav);
	//}
	//
	//pRigi = new CPxRigidbody();
	//pMonster->AddComponent(pRigi);
	//pRigi->init(Vec3(500.f, 120.f, 1000.f), Vec3(150.f, 340.f, 150.f), (int)LAYER_TYPE::Monster, pMonster);
	//pRigi->SetOffsetPosition(Vec3(0.f, -170.f, 0.f));
	//
	//
	//pMonsterScript = new CMonsterScript();
	//pMonster->AddComponent(pMonsterScript);
	//pMonsterScript->Initialize(L"Balder_Knight"); //fbx load, FSM , components
	//
	//pMonsterScript->AddAnimFrame(L"Hit", 431, 481);
	//pMonsterScript->AddAnimFrame(L"HitDown", 482, 602);
	//pMonsterScript->AddAnimFrame(L"Attack0", 603, 687);
	//pMonsterScript->AddAnimFrame(L"Attack1", 759, 850);
	////pMonsterScript->AddAnimFrame(L"Dead", 1116, 1200);
	//
	//pIdle = new CMonsterIdle();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 0, 95);
	//
	//pMove = new CMonsterMove();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::WALK, pMove, L"Walk", 96, 140);
	//
	//pAttack = new CMonsterAttack();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::ATTACK, pAttack, L"Attack");
	//
	//pHit = new CMonsterHit();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit");
	//
	//pMonsterDead = new CMonsterDead();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::DEAD, pMonsterDead, L"Dead", 1116, 1200);
	//
	//pMonsterScript->AddMonsterAttack(0, 0.f, 0.f, 0.6f, 629, 634, Vec3(150.f, 150.f, 150.f), 90.f, Vec3::Zero);
	//pMonsterScript->AddMonsterAttack(1, 0.f, 0.f, 0.6f, 781, 785, Vec3(170.f, 170.f, 170.f), 100.f, Vec3::Zero);
	//
	//pMonster->Transform()->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	//SpawnGameObject(pMonster, Vec3(500.f, 120.f, 1000.f), (int)LAYER_TYPE::Monster);

	
	
	CGameObject* pBoss = new CGameObject();
	pBoss->SetName(L"Taurus_Demon_Fianl");
	
	
	pBoss->AddComponent(new CTransform());
	pBoss->AddComponent(new CCollider3D());
	
	pBoss->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));
	pRigi = new CPxRigidbody();
	pBoss->AddComponent(pRigi);
	//
	//1783 300 2985
	//1254 1292
	pRigi->init(Vec3(1783.f, 120.f, 4585.f), Vec3(350.f, 600.f, 350.f), (int)LAYER_TYPE::Monster, pBoss);
	pRigi->SetOffsetPosition(Vec3(0.f, -300.f, 0.f));
	
	pBoss->Collider3D()->SetOffsetScale(Vec3(350.f, 600.f, 350.f));
	pBoss->Collider3D()->SetAbsolute(true);
	
	CDemonScript* pDemonScript = new CDemonScript();
	pBoss->AddComponent(pDemonScript);
	pDemonScript->Initialize(L"Taurus_Demon_Fianl");
	{
		//CNavMeshMgr::GetInst()->CreatePlane({ 1000, 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
		//CNavMeshMgr::GetInst()->CreatePlane({ 3000 , 0, 1000 }, Vec3(2000.f, 0.f, 2000.f));
		//CNavMeshMgr::GetInst()->CreatePlane({ 1700 , 0, 5500 }, Vec3(1000.f, 0.f, 7000.f));
		//CNavMeshMgr::GetInst()->CreatePlane({ 1275 , 350, 5490 }, Vec3(0, 700.f, 7000.f));
		//CNavMeshMgr::GetInst()->CreatePlane({ 1700 , 0, 2400 }, Vec3(1300.f, 0.f, 800.f));
		tBuildSettings tSetting = {};
		tSetting.ID = pBoss->GetID();
		tSetting.agentRadius = 900; // 900 / 5 = 180
		CNavMeshMgr::GetInst()->BuildField(tSetting);
	
		CRDNavMeshField* pNav = new CRDNavMeshField();
		pBoss->AddComponent(pNav);
		//CNavAgent* pAgent = new CNavAgent;
		//pBoss->AddComponent(pAgent);
		//pAgent->SetSpeed(2);
		//pAgent->SetRadius(0.5);
		//pAgent->AssignToNavigationField(pNav);
	}

	SpawnGameObject(pBoss, Vec3(1700.f, 120.f, 1700.f), (int)LAYER_TYPE::Monster);
}

void CreateLandScape()
{
	// LandScape Object
	CGameObject* pLandScape = new CGameObject;
	
	pLandScape->SetName(L"LandScape");
	
	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CCollider3D());
	pLandScape->Collider3D()->SetAbsolute(true);
	pLandScape->Collider3D()->SetOffsetPos(Vec3(9000.f, 0.f, 9000.f));
	pLandScape->Collider3D()->SetOffsetScale(Vec3(18000.f, 1.f, 18000.f));
	pLandScape->AddComponent(new CLandScape);
	pLandScape->AddComponent(new CLandScpaeScript);

	pLandScape->Transform()->SetRelativeScale(Vec3(1000.f, 3000.f, 1000.f));
	
	pLandScape->LandScape()->SetFace(18, 18);
	pLandScape->LandScape()->SetFrustumCheck(false);
	//pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));
	
	//CRDNavMesh* pNavMesh = new CRDNavMesh();
	//pNavMesh->NavMeshBuild();

	CPhysxMgr::GetInst()->AddActorStatic(Vec3(9000.f, 0.f, 9000.f), Vec3(18000.f, 1.f, 18000.f), Vec3::Zero, 0.f,
		(int)LAYER_TYPE::LandScape, pLandScape);

	SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::LandScape);

	
	
	//CGameObject* pLandform = InitializeFBX(L"IronGolem_Stage");
	//pLandform->AddComponent(new CTransform);
	//pLandform->AddComponent(new CCollider3D);
	//CTerrainScript* pTerrainScript = new CTerrainScript();
	//pLandform->AddComponent(pTerrainScript);
	//pLandform->AddComponent(new CLandFormScript);
	//pLandform->Transform()->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	//
	//pLandform->Collider3D()->SetOffsetScale(Vec3(3317.f, 399.f, 1.f));
	//pLandform->Collider3D()->SetOffsetPos(Vec3(-1217.f, -275.f, 64.f));
	//pLandform->Transform()->SetRelativeRot(Vec3(-XM_PI/2.f, 0.f, 0.f));
	//SpawnGameObject(pLandform, Vec3(5700.f, -120.f, -500.f), (int)LAYER_TYPE::LandScape);
	//5700

	//CGameObject* pStage = new CGameObject();
	//pStage->AddComponent(new CTransform());
	//CTerrainScript* pTerrainScript2 = new CTerrainScript();
	//pStage->AddComponent(pTerrainScript2);
	//pTerrainScript2->Initialize(L"Stage2");
	//SpawnGameObject(pStage, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Default);


	CreateStage();

	//1번 -3832 287 -3264
	//1212 , 165, 1
	//-1 -30 0


	//vecChild[1]->Collider3D()->SetA
	//for (int i = 0; i < vecChild.size(); ++i)
	//{	
	//	vecChild[i]->AddComponent(new CCollider3D);
	//}
	//3000, 3573 , 3000

	//CGameObject* pPlain = InitializeFBX(L"Plain_CastleTower_Ruin_01");
	//pPlain->AddComponent(new CTransform);
	//pPlain->AddComponent(new CCollider3D);
	////pPlain->AddComponent(new CLandScpaeScript);
	//pPlain->Collider3D()->SetOffsetScale(Vec3(300.f, 350.f, 300.f));
	//pPlain->Collider3D()->SetOffsetPos(Vec3(0.f, 180.f, 0.f));
	////pPlain->Collider3D()->SetOffsetRot(Vec3(XM_PI/4.f, 0.f, 0.f));
	//
	//CPhysxMgr::GetInst()->AddActorStatic(Vec3(1000.f, 180, 1000.f), Vec3(300.f, 350.f, 300.f), Vec3::Zero, 0.f,
	//	(int)LAYER_TYPE::Obstacle, pPlain);
	////CPhysxMgr::GetInst()->AddActor(Vec3(1000.f, 180, 1000.f), Vec3(300.f, 350.f, 300.f), Vec3::Zero ,0.f, 
	////	(int)LAYER_TYPE::Obstacle, pPlain);
	//
	//SpawnGameObject(pPlain, Vec3(1000.f, 0, 1000.f), (int)LAYER_TYPE::Obstacle);



	CreateUI();
}

void CreateStage()
{
	CGameObject* pStage = InitializeFBX(L"Boss Stage");

	//앞면 뒷면 모두 그리기
	ChanageRSType(pStage, RS_TYPE::CULL_NONE);

	pStage->AddComponent(new CTransform);
	pStage->Transform()->SetRelativeRot(Vec3(-XM_PI / 2.f, XM_PI / 2.f, 0.f));
	SpawnGameObject(pStage, Vec3(3000.f, 3573.f, 3000.f), (int)LAYER_TYPE::Default);


	float fRadian = XM_PI / 180.f;
	//px충돌체 위치
	CGameObject* pPxColl_0 = new CGameObject();
	pPxColl_0->SetName(L"Coll_0");
	pPxColl_0->AddComponent(new CTransform());
	pPxColl_0->AddComponent(new CStairsScript);

	CCollider3D* pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(230.f, 1.f, 1142.f));
	pCollider->SetOffsetRot(Vec3(fRadian* 30.f, 0.f, 0.f));
	pPxColl_0->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(2700.f, 290.f, 6874.f), Vec3(230.f, 1.f, 1142.f),
		Vec3::Right, 30.f, (int)LAYER_TYPE::Stairs, pPxColl_0);
	SpawnGameObject(pPxColl_0, Vec3(2700.f, 290.f, 6874.f), (int)LAYER_TYPE::Stairs);


	CGameObject* pPxColl_1 = new CGameObject();
	pPxColl_1->SetName(L"Coll_1");
	pPxColl_1->AddComponent(new CTransform());
	pPxColl_1->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(230.f, 1.f, 1142.f));
	pCollider->SetOffsetRot(Vec3(-fRadian * 30.f, 0.f, 0.f));
	pPxColl_1->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(2924.f, 808.f, 6929.f), Vec3(230.f, 1.f, 1142.f),
		Vec3::Right, -30.f, (int)LAYER_TYPE::Stairs, pPxColl_1);
	SpawnGameObject(pPxColl_1, Vec3(2924.f, 808.f, 6929.f), (int)LAYER_TYPE::Stairs);

	
	CGameObject* pPxColl_2 = new CGameObject();
	pPxColl_2->SetName(L"Coll_2");
	pPxColl_2->AddComponent(new CTransform());
	pPxColl_2->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(200.f, 1.f, 880.f));
	pPxColl_2->AddComponent(pCollider); //570
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(2913.f, 570.f, 6128.f), Vec3(200.f, 1.f, 880.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_2);
	SpawnGameObject(pPxColl_2, Vec3(2913.f, 570.f, 6128.f), (int)LAYER_TYPE::Stairs);

	
	CGameObject* pPxColl_3 = new CGameObject();
	pPxColl_3->SetName(L"Coll_3");
	pPxColl_3->AddComponent(new CTransform());
	pPxColl_3->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(200.f, 1.f, 672.f));
	pPxColl_3->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(2715.f, 570.f, 6041.f), Vec3(200.f, 1.f, 672.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_3);
	SpawnGameObject(pPxColl_3, Vec3(2715.f, 570.f, 6041.f), (int)LAYER_TYPE::Stairs);

	CGameObject* pPxColl_4 = new CGameObject();
	pPxColl_4->SetName(L"Coll_4");
	pPxColl_4->AddComponent(new CTransform());
	pPxColl_4->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(430.f, 1.f, 880.f));
	pPxColl_4->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3044.f, 1094.f, 7865.f), Vec3(430.f, 1.f, 880.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_4);
	SpawnGameObject(pPxColl_4, Vec3(3044.f, 1094.f, 7865.f), (int)LAYER_TYPE::Stairs);

	CGameObject* pPxColl_5 = new CGameObject();
	pPxColl_5->SetName(L"Coll_5");
	pPxColl_5->AddComponent(new CTransform());
	pPxColl_5->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(200.f, 1.f, 650.f));
	pPxColl_5->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3154.f, 1094.f, 7127.f), Vec3(200.f, 1.f, 650.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_5);
	SpawnGameObject(pPxColl_5, Vec3(3154.f, 1094.f, 7127.f), (int)LAYER_TYPE::Stairs);

	CGameObject* pPxColl_6 = new CGameObject();
	pPxColl_6->SetName(L"Coll_6");
	pPxColl_6->AddComponent(new CTransform());
	pPxColl_6->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(230.f, 1.f, 1500.f));
	pCollider->SetOffsetRot(Vec3(fRadian * 30.f, 0.f, 0.f));
	pPxColl_6->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3130.f, 1370.f, 6280.f), Vec3(230.f, 1.f, 1500.f),
		Vec3::Right, 30.f, (int)LAYER_TYPE::Stairs, pPxColl_6);
	SpawnGameObject(pPxColl_6, Vec3(3130.f, 1370.f, 6280.f), (int)LAYER_TYPE::Stairs);

	
	CGameObject* pPxColl_7 = new CGameObject();
	pPxColl_7->SetName(L"Coll_7");
	pPxColl_7->AddComponent(new CTransform());
	pPxColl_7->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(200.f, 1.f, 880.f));
	pPxColl_7->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3149.f, 1743.f, 5196.f), Vec3(200.f, 1.f, 880.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_7);
	SpawnGameObject(pPxColl_7, Vec3(3149.f, 1743.f, 5196.f), (int)LAYER_TYPE::Stairs);

	//3130 2036 4653
	CGameObject* pPxColl_8 = new CGameObject();
	pPxColl_8->SetName(L"Coll_8");
	pPxColl_8->AddComponent(new CTransform());
	pPxColl_8->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(230.f, 1.f, 1500.f));
	pCollider->SetOffsetRot(Vec3(fRadian * 30.f, 0.f, 0.f));
	pPxColl_8->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3130.f, 2036.f, 4653.f), Vec3(230.f, 1.f, 1500.f),
		Vec3::Right, 30.f, (int)LAYER_TYPE::Stairs, pPxColl_8);
	SpawnGameObject(pPxColl_8, Vec3(3130.f, 2036.f, 4653.f), (int)LAYER_TYPE::Stairs);

	//3150 2411 3333
	//200 1 1340
	CGameObject* pPxColl_9 = new CGameObject();
	pPxColl_9->SetName(L"Coll_9");
	pPxColl_9->AddComponent(new CTransform());
	pPxColl_9->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(200.f, 1.f, 1340.f));
	pPxColl_9->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(3150.f, 2411.f, 3333.f), Vec3(200.f, 1.f, 1340.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_9);
	SpawnGameObject(pPxColl_9, Vec3(3150.f, 2411.f, 3333.f), (int)LAYER_TYPE::Stairs);

	//1738 2411 2450
	//3000 1 1215
	CGameObject* pPxColl_10 = new CGameObject();
	pPxColl_10->SetName(L"Coll_10");
	pPxColl_10->AddComponent(new CTransform());
	pPxColl_10->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(3000.f, 1.f, 1215.f));
	pPxColl_10->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(1738.f, 2411.f, 2450.f), Vec3(3000.f, 1.f, 1215.f),
		Vec3::Zero, 0.f, (int)LAYER_TYPE::Stairs, pPxColl_10);
	SpawnGameObject(pPxColl_10, Vec3(1738.f, 2411.f, 2450.f), (int)LAYER_TYPE::Stairs);

	//1740 2410 3036
	//2650 1 200
	//90
	CGameObject* pPxColl_11 = new CGameObject();
	pPxColl_11->SetName(L"Coll_11");
	pPxColl_11->AddComponent(new CTransform());
	pPxColl_11->AddComponent(new CStairsScript);

	pCollider = new CCollider3D();
	pCollider->SetOffsetScale(Vec3(2650.f, 1.f, 200.f));
	pCollider->SetOffsetRot(Vec3(fRadian * 90.f, 0.f, 0.f));

	pPxColl_11->AddComponent(pCollider);
	CPhysxMgr::GetInst()->AddActorStatic(Vec3(1740.f, 2410.f, 3036.f), Vec3(2650.f, 1.f, 200.f),
		Vec3::Right, 90.f, (int)LAYER_TYPE::Obstacle, pPxColl_11);
	SpawnGameObject(pPxColl_11, Vec3(1740.f, 2410.f, 3036.f), (int)LAYER_TYPE::Obstacle);
	pPxColl_11->AddComponent(new CObjstacleScript);
		
	//기둥
	//CGameObject* Pillar = new CGameObject();
	//Pillar->SetName(L"Pillar_1");
	//Pillar->AddComponent(new CTransform());
	//Pillar->AddComponent(new CStairsScript);
	//
	//pCollider = new CCollider3D();
	//pCollider->SetOffsetScale(Vec3(2650.f, 1.f, 200.f));
	//
	//Pillar->AddComponent(pCollider);
	//CPhysxMgr::GetInst()->AddActorStatic(Vec3(1740.f, 500.f, 6000.f), Vec3(2650.f, 1.f, 200.f),
	//	Vec3::Zero, 0.f, (int)LAYER_TYPE::Obstacle, Pillar);
	//SpawnGameObject(Pillar, Vec3(1740.f, 500.f, 6000.f), (int)LAYER_TYPE::Obstacle);
}

void CreateUI()
{
	//-540 272
	CEngineUI* pPlayerIcon = new CEngineUI();
	pPlayerIcon->Initialize(L"texture\\GameTexture\\playerIcon.png", Vec3(100.f,100.f,0.f), L"PlayerIcon");
	SpawnGameObject(pPlayerIcon, Vec3(-540.f, 272.f, -1.f), (int)LAYER_TYPE::UI);

	//-85 272
	CEngineUI* pPlayerFrame = new CEngineUI();
	pPlayerFrame->Initialize(L"texture\\GameTexture\\playerframe.png", Vec3(800.f, 48.f, 0.f),L"PlayerFrame");
	SpawnGameObject(pPlayerFrame, Vec3(-85, 272.f, 5.f), (int)LAYER_TYPE::UI);

	//hp heal playerscript로

	//menu 1.5배

	//-520 -200
	CEngineUI* pLeftMenu = new CEngineUI();
	pLeftMenu->Initialize(L"texture\\GameTexture\\LeftRightMenu.png", Vec3(120.f, 150.f, 0.f), L"LeftRightMenu0");
	SpawnGameObject(pLeftMenu, Vec3(-520.f, -200.f, -1.f), (int)LAYER_TYPE::UI);

	//-280 -200
	CEngineUI* pRightMenu = new CEngineUI();
	pRightMenu->Initialize(L"texture\\GameTexture\\LeftRightMenu.png", Vec3(120.f, 150.f, 0.f), L"LeftRightMenu1");
	SpawnGameObject(pRightMenu, Vec3(-280.f, -200.f, -1.f), (int)LAYER_TYPE::UI);

	//-400 -90
	CEngineUI* pTopMenu = new CEngineUI();
	pTopMenu->Initialize(L"texture\\GameTexture\\TopBottomMenu.png", Vec3(99.f, 150.f, 0.f), L"TopBottomMenu0");
	SpawnGameObject(pTopMenu, Vec3(-400.f, -90.f, -1.f), (int)LAYER_TYPE::UI);
	//-400 -300
	CEngineUI* pBottomMenu = new CEngineUI();
	pBottomMenu->Initialize(L"texture\\GameTexture\\TopBottomMenu.png", Vec3(99.f, 150.f, 0.f), L"TopBottomMenu1");
	SpawnGameObject(pBottomMenu, Vec3(-400.f, -300.f, -1.f), (int)LAYER_TYPE::UI);
	
}
