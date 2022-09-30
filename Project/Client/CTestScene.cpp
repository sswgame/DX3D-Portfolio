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
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight2D.h>
#include <Engine/CLight3D.h>
#include <Engine/CSkyBox.h>
#include <Engine/CDecal.h>
#include <Engine/CLandScape.h>

#include <Engine/CSceneFile.h>
#include <Engine/CSound.h>

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>

#include <Script/CSceneSaveLoad.h>


void CTestScene::CreateTestScene()
{
	CScene* pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);

	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png", L"texture\\MagicCircle.png");

	Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
	                                                            L"texture\\tile\\TILE_01.tga");
	Ptr<CTexture> pTileNTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga",
	                                                             L"texture\\tile\\TILE_01_N.tga");

	Ptr<CTexture> pSkyTex_01 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky01.png",
	                                                              L"texture\\skybox\\Sky01.png");
	Ptr<CTexture> pSkyTex_02 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg",
	                                                              L"texture\\skybox\\Sky02.jpg");
	Ptr<CTexture> pSkyTex_03 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds",
	                                                              L"texture\\skybox\\SkyDawn.dds");
	Ptr<CTexture> pSkyTex_04 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyWater.dds",
	                                                              L"texture\\skybox\\SkyWater.dds");


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();
	pCamObj->Camera()->SetShowFrustum(true);

	pCurScene->AddObject(pCamObj, L"Default");

	// Directional Light
	CGameObject* pLight3D = new CGameObject;
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
	/*CGameObject* pLight3D = new CGameObject;
	pLight3D->SetName(L"Point Light");

	pLight3D->AddComponent(new CTransform);
	pLight3D->AddComponent(new CLight3D);

	pLight3D->Transform()->SetRelativePos(Vec3(0.f, 500.f, 500.f));

	pLight3D->Light3D()->SetLightType(LIGHT_TYPE::POINT);	
	pLight3D->Light3D()->SetRange(1000.f);

	pLight3D->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight3D->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	pLight3D->Light3D()->SetAmbient(Vec3(0.f, 0.f, 0.f));

	pCurScene->AddObject(pLight3D, L"Default");*/


	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;

	pSkyBox->SetName(L"SkyBox");
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

	pSkyBox->SkyBox()->SetSkyboxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_CUBE_0, pSkyTex_03);
	pSkyBox->SkyBox()->SetFrustumCulling(false);

	pCurScene->AddObject(pSkyBox, L"Default");


	// LandScape
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"LandScape");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLandScape);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pObject->Transform()->SetRelativeScale(2000.f, 2000.f, 2000.f);
	pObject->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	pObject->LandScape()->SetDynamicShadow(false);
	pObject->LandScape()->SetFrustumCulling(false);
	pObject->LandScape()->SetFaceCount(8, 8);
	pObject->LandScape()->Create();

	pCurScene->AddObject(pObject, L"Default");


	// Tessellation Object
	pObject = new CGameObject;
	pObject->SetName(L"Tessellation Object");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(0.f, 500.f, 500.f);
	pObject->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->MeshRender()->SetFrustumCulling(true);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TessMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTileTex);
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, pTileNTex);

	pCurScene->AddObject(pObject, L"Default");


	// 충돌 레이어 설정
	pCurScene->SetResKey(L"scene\\TestScene.scene");
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
	CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");

	//pCurScene->start();	
	pCurScene->SetSceneState(SCENE_STATE::STOP);
}
