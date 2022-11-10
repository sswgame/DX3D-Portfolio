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

#define TEST_SAVE 1

void CTestScene::CreateTestScene()
{
#pragma region SAVE
#if TEST_SAVE == 1
	auto pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);

	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");

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

	pCurScene->AddObject(pCamObj, L"Default");

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

	pCurScene->AddObject(pLight3D, L"Default");

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

	pCurScene->AddObject(pSkyBox, L"Default");


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

	CGameObject* pBoss = new CGameObject;
	pBoss->SetName(L"BOSS_COMBAT");
	pBoss->AddComponent(new CTransform);
	pBoss->AddComponent(new BossJugCombatMgrScript);
	pCurScene->AddObject(pBoss, 1);
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
