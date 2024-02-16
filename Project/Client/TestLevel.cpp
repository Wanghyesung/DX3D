#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CFSM.h>
#include <Engine\CIdleState.h>
#include <Engine\CWalkState.h>
#include <Engine\CRunState.h>

#include <Engine\CMonsterIdle.h>
#include <Engine\CMonsterMove.h>
#include <Engine\CMonsterAttack.h>
#include <Engine\CMonsterHit.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>
#include <Script\CTerrainScript.h>
#include <Script\CLandFormScript.h>
#include <Script\CEquipScript.h>
#include "CLevelSaveLoad.h"

#include <Engine\CCollisionMgr.h>
#include <Engine\CSetColorShader.h>


void CreateTestLevel()
{
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::Attack, (UINT)LAYER_TYPE::Monster);
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER_TYPE::MonsterAttack, (UINT)LAYER_TYPE::Player);

	//return;

	// 컴퓨트 쉐이더 테스트
	Ptr<CTexture> pTestTexture =
		CResMgr::GetInst()->CreateTexture(L"ComputeTestTex"
			, 200, 200, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"SetColorCS").Get();
	pCS->SetTargetTexture(pTestTexture);
	pCS->SetColor(Vec3(1.f, 0.f, 1.f));
	pCS->Execute();


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

	
	//CGameObject* pAritorias = new CGameObject;
	//pAritorias->SetName(L"Aritorias");
	//pAritorias->AddComponent(new CTransform());
	//pAritorias->AddComponent(new CRigidbody());
	////CPxRigidbody* pRigi = new CPxRigidbody();
	////pRigi->init();
	////pAritorias->AddComponent(pRigi);
	//CCollider3D* pCollider = new CCollider3D();
	//pCollider->SetAbsolute(true);
	//pCollider->SetOffsetScale(Vec3(120.f, 150.f, 120.f));
	//pCollider->SetOffsetPos(Vec3(0.f, 0.f, 125.f));
	//pAritorias->AddComponent(pCollider);
	//
	//CPlayerScript* pScript = new CPlayerScript(); 
	//pAritorias->AddComponent(pScript);
	//pScript->Initialize(); //fbx load, FSM , components
	//
	//pAritorias->GetChild().at(15)->AddComponent(new CCollider3D());
	//
	////pAritorias->AddComponent(new CMotionBlur());
	////pAritorias->MotionBlur()->Initialize();
	//
	//SpawnGameObject(pAritorias, Vec3(1000.f, 0.f, 100.f), (int)LAYER_TYPE::Player);
	

	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Taurus_Demon");
	//pMonster->AddComponent(new CTransform());
	//pMonster->AddComponent(new CRigidbody());
	//CMonsterScript* pMonsterScript = new CMonsterScript();
	//pMonster->AddComponent(pMonsterScript);
	//pMonsterScript->Initialize(L"Taurus_Demon_Fianl"); //fbx load, FSM , components
	//SpawnGameObject(pMonster, Vec3(400.f, 0.f, 400.f), L"Default");

	CreateMonster();
	

	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	CCameraMoveScript* pCameraScript = new CCameraMoveScript();
	//pCameraScript->SetTarget(pAritorias); //카메라가 바라보는 물체 등록
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
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Balder_Knight");
	//pMonster->AddComponent(new CTransform());
	//pMonster->AddComponent(new CRigidbody());
	//pMonster->AddComponent(new CCollider3D());
	//pMonster->AddComponent(new CNavMesh);
	//pMonster->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 340.f));
	//pMonster->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 170.f));
	//
	//pMonster->Collider3D()->SetAbsolute(true);
	//CMonsterScript* pMonsterScript = new CMonsterScript();
	//pMonster->AddComponent(pMonsterScript);
	//
	//pMonsterScript->Initialize(L"Balder_Knight"); //fbx load, FSM , components
	//
	////pMonsterScript->AddAnimFrame(L"Idle", 0, 95);
	////pMonsterScript->AddAnimFrame(L"Walk", 96, 140);
	////pMonsterScript->AddAnimFrame(L"Walk_Back", 141, 186);
	////pMonsterScript->AddAnimFrame(L"Run", 270, 304);
    //pMonsterScript->AddAnimFrame(L"Hit", 431, 481);
	//pMonsterScript->AddAnimFrame(L"HitDown", 482, 602);
	//pMonsterScript->AddAnimFrame(L"Attack0", 603, 687);
	//pMonsterScript->AddAnimFrame(L"Attack1", 759, 850);
	////pMonsterScript->AddAnimFrame(L"Dead", 1116, 1200);
	//
	//CMonsterIdle* pIdle = new CMonsterIdle();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 0, 95);
	//
	//CMonsterMove* pMove = new CMonsterMove();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::RUN,  pMove, L"Walk", 96, 140);
	//
	//CMonsterAttack* pAttack = new CMonsterAttack();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::ATTACK, pAttack, L"Attack");
	//
	//CMonsterHit* pHit = new CMonsterHit();
	//pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit");
	//
	//pMonsterScript->AddMonsterAttack(0, 5.f, 0.f, 0.6f, 629, 631, Vec3(150.f, 150.f, 150.f), 90.f, Vec3::Zero);
	//pMonsterScript->AddMonsterAttack(1, 5.f, 0.f, 0.6f, 882, 885, Vec3(170.f, 170.f, 170.f), 100.f, Vec3::Zero);
	//
	//SpawnGameObject(pMonster, Vec3(3000.f, 0.f, 3000.f), (int)LAYER_TYPE::Monster);
	//300

	CGameObject* pWeapon = InitializeFBX(L"TaurusDemon_Axe");;
	pWeapon->AddComponent(new CTransform());
	pWeapon->AddComponent(new CEquip());
	//pWeapon->AddComponent(new CMeshRender());
	pWeapon->Transform()->SetRelativeScale(Vec3(3.1f,3.1f,3.1f));
	

	
	float fRadian = XM_PI / 180.f;
	pWeapon->Transform()->SetRelativeRot(-90.f * fRadian, 180.f * fRadian, -15.f * fRadian);
	;
	pWeapon->Transform()->SetAbsolute(true);
	//-90 270 -9

	SpawnGameObject(pWeapon, Vec3(-330.f, -50.f, 360.f), (int)LAYER_TYPE::Monster);
	//-330 -50 360 
	

	// -90, 280 , 100
	// -15.f, 162.f, 173.f
	CGameObject* pBoss = new CGameObject();
	pBoss->SetName(L"Taurus_Demon_Fianl");
	pBoss->AddComponent(new CTransform());
	pBoss->AddComponent(new CRigidbody());
	pBoss->AddComponent(new CCollider3D());
	pBoss->AddComponent(new CNavMesh);
	pBoss->Collider3D()->SetAbsolute(true);

	CMonsterScript* pMonsterScript = new CMonsterScript();
	pBoss->AddComponent(pMonsterScript);
	pMonsterScript->Initialize(L"Taurus_Demon_Fianl");
	
	CGameObject* pHand = pBoss->GetChild().at(1);
	pWeapon->Equip()->SetChar(pHand);
	SpawnGameObject(pBoss, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Monster);
	
	CMonsterIdle* pIdle = new CMonsterIdle();
	pMonsterScript->AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 651, 792);

	//pMonster = new CGameObject();
	//pMonster = InitializeFBX(L"Heavy_Knight");
	//pMonster->AddComponent(new CTransform());
	//pMonster->AddComponent(new CRigidbody());
	//pMonster->AddComponent(new CCollider3D());
	//pMonster->AddComponent(new CNavMesh);
	//
	//pMonster->Collider3D()->SetAbsolute(true);
	//pMonsterScript = new CMonsterScript();
	//pMonster->AddComponent(pMonsterScript);
	//SpawnGameObject(pMonster, Vec3(1000.f, 0.f, 1000.f), (int)LAYER_TYPE::Monster);
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
	pLandScape->Collider3D()->SetOffsetScale(Vec3(18000.f, 0.f, 18000.f));
	pLandScape->AddComponent(new CLandScape);
	
	pLandScape->Transform()->SetRelativeScale(Vec3(1000.f, 3000.f, 1000.f));
	
	pLandScape->LandScape()->SetFace(18, 18);
	pLandScape->LandScape()->SetFrustumCheck(false);
	//pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));
	
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


}
