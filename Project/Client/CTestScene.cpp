#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight2D.h>
#include <Engine/CLight3D.h>
#include <Engine/CSkyBox.h>
#include <Engine/CDecal.h>
#include <Engine/CLandScape.h>
#include <Engine/CSceneFile.h>
#include <Engine/CSound.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include <Engine/CMeshData.h>


// [SCRIPT TYPE]
#include <Script/PlayerScript.h>
#include <Script/CameraMoveScript.h>
#include <Script/MissileScript.h>
#include <Script/PlayerCamScript.h>
#include <Script/BossJugCombatMgrScript.h>
#include <Script/CSceneSaveLoad.h>
#include <Script/RigidBodyScript.h>
#include <Script/GravityScript.h>
#include <Script/TrailScript.h>
#define TEST_SAVE 1

void CTestScene::CreateTestScene()
{
#pragma region SAVE
#if TEST_SAVE == 1
	auto pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);

	pCurScene->SetLayerName(0, L"BG");
	pCurScene->SetLayerName(1, L"BG_OBJ");
	pCurScene->SetLayerName(2, L"ITEM");
	pCurScene->SetLayerName(3, L"PLAYER");
	pCurScene->SetLayerName(4, L"PLAYER_ATTACK");
	pCurScene->SetLayerName(5, L"MONSTER");
	pCurScene->SetLayerName(6, L"MONSTER_NON-PARRING-ATTACK");
	pCurScene->SetLayerName(7, L"MONSTER_PARRING-ATTACK");
	pCurScene->SetLayerName(8, L"OBJECT-MGR");
	pCurScene->SetLayerName(9, L"CAMERA");
	pCurScene->SetLayerName(30, L"UI_STATIC");
	pCurScene->SetLayerName(31, L"UI_INTERACTIVE");


	// Texture 한장 로딩해보기
	//CResMgr::GetInst()->Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	Ptr<CTexture> pMagicCircle = CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png",
	                                                                L"texture\\MagicCircle.png");

	Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
	                                                            L"texture\\tile\\TILE_01.tga");
	//Ptr<CTexture> pTileNTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga",
	//                                                             L"texture\\tile\\TILE_01_N.tga");

	//Ptr<CTexture> pSkyTex_01 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky01.png",
	//                                                              L"texture\\skybox\\Sky01.png");
	//Ptr<CTexture> pSkyTex_02 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg",
	//                                                              L"texture\\skybox\\Sky02.jpg");
	Ptr<CTexture> pSkyTex_03 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds",
	                                                              L"texture\\skybox\\SkyDawn.dds");
	//Ptr<CTexture> pSkyTex_04 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyWater.dds",
	//                                                              L"texture\\skybox\\SkyWater.dds");


	// Camera Object 추가
	auto pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	//pCamObj->AddComponent(new CameraMoveScript);
	pCamObj->AddComponent(new PlayerCamScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();
	pCamObj->Camera()->SetShowFrustum(true);

	pCurScene->AddObject(pCamObj, L"CAMERA");

	// Directional Light
	auto pLight3D = new CGameObject;
	pLight3D->SetName(L"Directional Light");

	pLight3D->AddComponent(new CTransform);
	pLight3D->AddComponent(new CLight3D);

	pLight3D->Transform()->SetRelativePos(Vec3(-1000.f, 1000.f, 0.f));

	pLight3D->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight3D->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pLight3D->Light3D()->SetRange(1000.f);

	pLight3D->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight3D->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pLight3D->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	pCurScene->AddObject(pLight3D, L"BG_OBJ");

	// Point Light
	//pLight3D = new CGameObject;
	//pLight3D->SetName(L"Point Light");

	//pLight3D->AddComponent(new CTransform);
	//pLight3D->AddComponent(new CLight3D);

	//pLight3D->Transform()->SetRelativePos(Vec3(0.f, 500.f, 500.f));

	//pLight3D->Light3D()->SetLightType(LIGHT_TYPE::POINT);	
	//pLight3D->Light3D()->SetRange(1000.f);

	//pLight3D->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	//pLight3D->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	//pLight3D->Light3D()->SetAmbient(Vec3(0.f, 0.f, 0.f));

	//pCurScene->AddObject(pLight3D, L"Default");

	// particle
	//CGameObject* pParticleObj = new CGameObject;
	//pParticleObj->SetName(L"Test Particle");

	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 100.f, 0.f));

	//pCurScene->AddObject(pParticleObj, L"Default");


	// SkyBox 추가
	auto pSkyBox = new CGameObject;

	pSkyBox->SetName(L"SkyBox");
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

	pSkyBox->SkyBox()->SetSkyboxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_CUBE_0, pSkyTex_03);
	pSkyBox->SkyBox()->SetFrustumCulling(false);

	pCurScene->AddObject(pSkyBox, L"BG");


	// LandScape
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"LandScape");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLandScape);

	//pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pObject->Transform()->SetRelativeScale(2000.f, 2000.f, 2000.f);
	//pObject->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	//pObject->LandScape()->SetDynamicShadow(false);
	//pObject->LandScape()->SetFrustumCulling(false);
	//pObject->LandScape()->SetFaceCount(8, 8);
	//pObject->LandScape()->Create();

	//pCurScene->AddObject(pObject, L"Default");

	//// Sphere
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Sphere");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pObject->Transform()->SetRelativeScale(600.f, 600.f, 600.f);
	//pObject->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	//const Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
	//const Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3D_DeferredMtrl.mtrl");
	//pObject->MeshRender()->SetMesh(pMesh);
	//pObject->MeshRender()->SetSharedMaterial(pMaterial, 0);
	//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);

	//pCurScene->AddObject(pObject, L"Default");


	// Decal
	//CGameObject* pDecal = new CGameObject;
	//pDecal->SetName(L"Decal");
	//
	//pDecal->AddComponent(new CTransform);
	//pDecal->AddComponent(new CDecal);

	//pDecal->Transform()->SetRelativePos(0.f, 290.f, 0.f);
	//pDecal->Transform()->SetRelativeScale(300.f, 1.f, 300.f);
	//pDecal->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
	//
	//pDecal->Decal()->SetDecalType(DECAL_TYPE::CUBE);
	//pDecal->Decal()->SetDecalTexture(pMagicCircle);
	//pDecal->Decal()->SetDeferredLighting(false);

	//pCurScene->AddObject(pDecal, L"Default");

	// Tessellation Object
	//pObject = new CGameObject;
	//pObject->SetName(L"Tessellation Object");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetRelativePos(0.f, 500.f, 500.f);
	//pObject->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

	//pObject->MeshRender()->SetDynamicShadow(true);
	//pObject->MeshRender()->SetFrustumCulling(true);
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TessMtrl.mtrl"),0);
	//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, pTileNTex);

	//pCurScene->AddObject(pObject, L"Default");

	// ============
	// FBX Loading
	// ============	
	{
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;

		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Monster.fbx");
		//pMeshData->Save(wstring(CPathMgr::GetInst()->GetContentPath()) + pMeshData->GetRelativePath());		

		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\Monster.mdat", L"meshdata\\Monster.mdat");

		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"Monster");
		//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		//pCurScene->AddObject(pObj, 0);
	}

	// ANIMATION TEST 

	//CGameObject* pBody{};
	////0
	//{
	//	const std::wstring path      = L"meshdata\\Deuxiemie\\deuxiemie_SphereShield0.mdat";
	//	Ptr<CMeshData>     pMeshData = CResMgr::GetInst()->Load<CMeshData>(path.c_str(), path.c_str());

	//	CGameObject* pObj = pMeshData->Instantiate();
	//	pObj->SetName(L"DEUXIEME_SHIELD");
	//	pObj->Animator3D()->Play(L"test", true);
	//	pObj->Animator3D()->GetCurAnim()->SetPlay(false);
	//	//pObj->AddComponent(new CFSM);
	//	pCurScene->AddObject(pObj, 0);

	//	pBody = pObj;
	//}
	////1
	//{
	//	const std::wstring path      = L"meshdata\\Deuxiemie\\deuxiemie_SphereShield1.mdat";
	//	Ptr<CMeshData>     pMeshData = CResMgr::GetInst()->Load<CMeshData>(path.c_str(), path.c_str());

	//	CGameObject* pObj = pMeshData->Instantiate();
	//	pObj->SetName(L"DEUXIEME_SHIELD_WEAPON_0");
	//	pObj->Animator3D()->Play(L"test", true);
	//	pObj->Animator3D()->GetCurAnim()->SetPlay(false);
	//	//pObj->AddComponent(new CFSM);
	//	CSceneMgr::GetInst()->AddChild(pBody, pObj);
	//}
	////2
	//{
	//	const std::wstring path      = L"meshdata\\Deuxiemie\\deuxiemie_SphereShield2.mdat";
	//	Ptr<CMeshData>     pMeshData = CResMgr::GetInst()->Load<CMeshData>(path.c_str(), path.c_str());

	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;


	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.FBX");
	//pMeshData->Save(wstring(CPathMgr::GetInst()->GetContentPath()) + pMeshData->GetRelativePath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword0.mdat", L"meshdata\\player_sword0.mdat");

	pObj = pMeshData->Instantiate();
	pObj->SetName(L"player");
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Animator3D()->Play(L"test", true);
	pObj->Animator3D()->GetCurAnim()->SetPlay(false);

	pObj->Animator3D()->MakeAnimationFromTXT_Extended("PlayerAnimInfo2.txt");
	pObj->Animator3D()->SetPlayWithChild(true);


	pObj->AddComponent(new CFSM);
	pObj->AddComponent(new RigidBodyScript);
	pObj->AddComponent(new GravityScript);
	pObj->AddComponent(new CCollider3D);

	pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObj->Collider3D()->SetOffsetPos(Vec3(0.f, 125.f, 0.f));
	pObj->Collider3D()->SetOffseetScale(Vec3(200.f, 250.f, 200.f));

	PlayerScript* pPlayerScript = new PlayerScript;
	pPlayerScript->SetCamera(pCamObj);
	pObj->AddComponent(pPlayerScript);

	CGameObject* pPlayer = pObj;
	pCurScene->AddObject(pObj, L"PLAYER");


	Ptr<CMeshData> pMeshDataWeapon = nullptr;
	CGameObject* pObjWeapon = nullptr;

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.FBX");
	//pMeshData->Save(wstring(CPathMgr::GetInst()->GetContentPath()) + pMeshData->GetRelativePath());
	pMeshDataWeapon = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword1.mdat", L"meshdata\\player_sword1.mdat");

	pObjWeapon = pMeshDataWeapon->Instantiate();
	pObjWeapon->SetName(L"player_sword1");
	pObjWeapon->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObjWeapon->Animator3D()->Play(L"test", true);

	pObj->AddChild(pObjWeapon);
	pObj->Animator3D()->CopyAllAnimToChild();

	//CGameObject* pGameObject = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\DEUXIEME_SHIELD.pref",
	//                                                             L"prefab\\DEUXIEME_SHIELD.pref")->Instantiate();
	//pCurScene->AddObject(pGameObject, 0);


	//pObj = new CGameObject;
	//pObj->SetName(L"Particle");
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CParticleSystem);
	//pCurScene->AddObject(pObj, 0);

	CGameObject* pObjFoot = new CGameObject;
	pObjFoot->SetName(L"Foot");
	pObjFoot->AddComponent(new CTransform);
	pObjFoot->AddComponent(new CCollider3D);

	pObjFoot->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
	pObjFoot->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObjFoot->Collider3D()->SetOffseetScale(Vec3(100.f, 100.f, 100.f));
	pObj->AddChild(pObjFoot);

	pObj = new CGameObject;
	pObj->SetName(L"Trail");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	auto pAnimator = new CAnimator3D;
	pObj->AddComponent(pAnimator);

	//TrailScript* pTrailScript = new TrailScript;
	//pTrailScript->SetOriginObject(pPlayer);
	//pObj->AddComponent(pTrailScript);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword0.mdat", L"meshdata\\player_sword0.mdat");
	pObj->MeshRender()->SetMesh(pMeshData->GetMesh());
	for (int i = 0; i < 4; ++i)
	{
		pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TrailMtrl.mtrl"), i);
	}

	pAnimator->SetBones(pMeshData->GetMesh()->GetBones());
	pAnimator->SetAnimClip(pMeshData->GetMesh()->GetAnimClip());

	// todo
	//int MaxFrameIdx = pAnimator->GetAnimClip(0).iFrameLength - 1;
	//pAnimator->CreateAnimByFrame(L"TrailMotion", 0, 100, 100);
	//pAnimator->Play(L"TrailMotion", false);
	//pAnimator->SetLerpTime(0.f);

	pCurScene->AddObject(pObj, L"PLAYER");

	pObj = new CGameObject;
	pObj->SetName(L"Land");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider3D);

	pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObj->Collider3D()->SetOffsetPos(Vec3(0.f, -150.f, 0.f));
	pObj->Collider3D()->SetOffseetScale(Vec3(200.f, 250.f, 200.f));

	pCurScene->AddObject(pObj, 1);


	CGameObject* pBoss = new CGameObject;
	pBoss->SetName(L"BOSS_COMBAT");
	pBoss->AddComponent(new CTransform);
	pBoss->AddComponent(new BossJugCombatMgrScript);
	pCurScene->AddObject(pBoss, L"MONSTER");
	//pBoss->GetScript<BossJugCombatMgrScript>()->SpawnStage();
	
	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionCheck(1, 1);

	pCurScene->SetResKey(L"scene\\TestScene.scene");
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
	CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");
#endif // TEST_SAVE

#pragma endregion

#pragma region LOAD
#if TEST_SAVE==0
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
	CScene* pCurScene        = CSceneSaveLoad::LoadScene(strSceneFilePath + L"scene\\GUN.scene");
	CSceneMgr::GetInst()->ChangeScene(pCurScene);
#endif // !TEST_SAVE==0

#pragma endregion

	//pCurScene->start();
	pCurScene->SetSceneState(SCENE_STATE::STOP);
}
