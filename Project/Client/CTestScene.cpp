#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CGameObject.h>

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight3D.h>
#include <Engine/CSkyBox.h>
#include <Engine/CDecal.h>
#include <Engine/CLandScape.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CMeshData.h>

// [SCRIPT TYPE]
#include <Script/PlayerScript.h>
#include <Script/CameraMoveScript.h>
#include <Script/PlayerCamScript.h>
#include <Script/BossJugCombatMgrScript.h>
#include <Script/CSceneSaveLoad.h>
#include <Script/RigidBodyScript.h>
#include <Script/GravityScript.h>
#include <Script/TrailScript.h>
#include <Script/SwordTrailScript.h>

//UI
#include <Script/UIPanelScript.h>
#include <Script/UITextScript.h>
#include <Script/UIImageScript.h>
#include <Script/UIProgressBarScript.h>
#include <Script/ButtonScript.h>
#include <Script/TestScript.h>

namespace
{
	void SetLayer(CScene* _pScene)
	{
		_pScene->SetLayerName(0, L"BG");
		_pScene->SetLayerName(1, L"BG_OBJ");
		_pScene->SetLayerName(2, L"ITEM");
		_pScene->SetLayerName(3, L"PLAYER");
		_pScene->SetLayerName(4, L"PLAYER_ATTACK");
		_pScene->SetLayerName(5, L"MONSTER");
		_pScene->SetLayerName(6, L"MONSTER_NON-PARRING-ATTACK");
		_pScene->SetLayerName(7, L"MONSTER_PARRING-ATTACK");
		_pScene->SetLayerName(8, L"OBJECT-MGR");
		_pScene->SetLayerName(9, L"CAMERA");
		_pScene->SetLayerName(30, L"UI_STATIC");
		_pScene->SetLayerName(31, L"UI_INTERACTIVE");
	}

	void LoadTextures()
	{
		// Texture 한장 로딩해보기

		//Ptr<CTexture> pSkyTex_01 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky01.png", L"texture\\skybox\\Sky01.png");
		//Ptr<CTexture> pSkyTex_02 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg", L"texture\\skybox\\Sky02.jpg");
		//Ptr<CTexture> pSkyTex_04 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyWater.dds",L"texture\\skybox\\SkyWater.dds");
	}

	CGameObject* AddCamera(CScene* _pScene)
	{
		const auto pCamera = new CGameObject;
		pCamera->SetName(L"MainCamera");
		pCamera->AddComponent(new CTransform);
		pCamera->AddComponent(new CCamera);
		//pCamObj->AddComponent(new CameraMoveScript);
		pCamera->AddComponent(new PlayerCamScript);

		pCamera->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pCamera->Camera()->SetCameraAsMain();
		pCamera->Camera()->CheckLayerMaskAll();
		pCamera->Camera()->CheckLayerMask(L"UI_STATIC");
		pCamera->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
		pCamera->Camera()->SetShowFrustum(true);

		_pScene->AddObject(pCamera, L"CAMERA");

		return pCamera;
	}

	void AddDirectionalLight(CScene* _pScene)
	{
		const auto pDirectionalLight = new CGameObject;
		pDirectionalLight->SetName(L"Directional Light");

		pDirectionalLight->AddComponent(new CTransform);
		pDirectionalLight->AddComponent(new CLight3D);

		pDirectionalLight->Transform()->SetRelativePos(Vec3(-1000.f, 1000.f, 0.f));

		pDirectionalLight->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		pDirectionalLight->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
		pDirectionalLight->Light3D()->SetRange(1000.f);
		pDirectionalLight->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		pDirectionalLight->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
		pDirectionalLight->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

		_pScene->AddObject(pDirectionalLight, L"BG_OBJ");
	}

	void AddPointLight(CScene* _pScene)
	{
		CGameObject* pPointLight = new CGameObject;
		pPointLight->SetName(L"Point Light");

		pPointLight->AddComponent(new CTransform);
		pPointLight->AddComponent(new CLight3D);

		pPointLight->Transform()->SetRelativePos(Vec3(0.f, 500.f, 500.f));

		pPointLight->Light3D()->SetLightType(LIGHT_TYPE::POINT);
		pPointLight->Light3D()->SetRange(1000.f);
		pPointLight->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		pPointLight->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		pPointLight->Light3D()->SetAmbient(Vec3(0.f, 0.f, 0.f));

		_pScene->AddObject(pPointLight, L"Default");
	}

	void AddParticle(CScene* _pScene)
	{
		CGameObject* pParticleObj = new CGameObject;
		pParticleObj->SetName(L"Test Particle");

		pParticleObj->AddComponent(new CTransform);
		pParticleObj->AddComponent(new CParticleSystem);

		pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 100.f, 0.f));

		_pScene->AddObject(pParticleObj, L"Default");
	}

	void AddSkybox(CScene* _pScene)
	{
		const Ptr<CTexture> pSkyTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds",
		                                                                 L"texture\\skybox\\SkyDawn.dds");
		const auto pSkyBox = new CGameObject;

		pSkyBox->SetName(L"SkyBox");
		pSkyBox->AddComponent(new CTransform);
		pSkyBox->AddComponent(new CSkyBox);

		pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

		pSkyBox->SkyBox()->SetSkyboxType(SKYBOX_TYPE::CUBE);
		pSkyBox->SkyBox()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_CUBE_0, pSkyTex);
		pSkyBox->SkyBox()->SetFrustumCulling(false);

		_pScene->AddObject(pSkyBox, L"BG");
	}

	void AddLandScape(CScene* _pScene)
	{
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

		_pScene->AddObject(pObject, L"Default");
	}

	void AddDecal(CScene* _pScene)
	{
		const Ptr<CTexture> pMagicCircle = CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png",
		                                                                      L"texture\\MagicCircle.png");
		CGameObject* pDecal = new CGameObject;
		pDecal->SetName(L"Decal");

		pDecal->AddComponent(new CTransform);
		pDecal->AddComponent(new CDecal);

		pDecal->Transform()->SetRelativePos(0.f, 290.f, 0.f);
		pDecal->Transform()->SetRelativeScale(300.f, 1.f, 300.f);
		pDecal->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

		pDecal->Decal()->SetDecalType(DECAL_TYPE::CUBE);
		pDecal->Decal()->SetDecalTexture(pMagicCircle);
		pDecal->Decal()->SetDeferredLighting(false);

		_pScene->AddObject(pDecal, L"Default");
	}

	void AddSphere(CScene* _pScene)
	{
		Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
		                                                            L"texture\\tile\\TILE_01.tga");
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Sphere");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
		pObject->Transform()->SetRelativeScale(600.f, 600.f, 600.f);
		pObject->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

		const Ptr<CMesh>     pMesh     = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		const Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3D_DeferredMtrl.mtrl");
		pObject->MeshRender()->SetMesh(pMesh);
		pObject->MeshRender()->SetSharedMaterial(pMaterial, 0);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);

		_pScene->AddObject(pObject, L"Default");
	}

	void AddTessellation(CScene* _pScene)
	{
		Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
		                                                            L"texture\\tile\\TILE_01.tga");
		Ptr<CTexture> pTileNTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga",
		                                                             L"texture\\tile\\TILE_01_N.tga");

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Tessellation Object");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativePos(0.f, 500.f, 500.f);
		pObject->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

		pObject->MeshRender()->SetDynamicShadow(true);
		pObject->MeshRender()->SetFrustumCulling(true);
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TessMtrl.mtrl"), 0);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1, pTileNTex);

		_pScene->AddObject(pObject, L"Default");
	}

	void AddPlayer(CScene* _pScene, CGameObject* _pCamera)
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject*   pObj      = nullptr;

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword0.mdat",
		                                                L"meshdata\\player_sword0.mdat");
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
		pObj->AddComponent(new TrailScript);
		pObj->AddComponent(new CCollider3D);

		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObj->Collider3D()->SetOffsetPos(Vec3(0.f, 92.f, 0.f));
		pObj->Collider3D()->SetOffsetScale(Vec3(75.f, 175.f, 75.f));

		PlayerScript* pPlayerScript = new PlayerScript;
		pPlayerScript->SetCamera(_pCamera);
		pObj->AddComponent(pPlayerScript);

		CGameObject* pPlayer = pObj;
		_pScene->AddObject(pObj, L"PLAYER");


		Ptr<CMeshData> pMeshDataWeapon = nullptr;
		CGameObject*   pObjWeapon      = nullptr;

		pMeshDataWeapon = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword1.mdat",
		                                                      L"meshdata\\player_sword1.mdat");

		pObjWeapon = pMeshDataWeapon->Instantiate();
		pObjWeapon->SetName(L"player_sword1");
		pObjWeapon->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pObjWeapon->Animator3D()->Play(L"test", true);

		pObj->AddChild(pObjWeapon);
		pObj->Animator3D()->CopyAllAnimToChild();


		// 무기 충돌체 
		CGameObject* m_pBoneCollider = new CGameObject;
		m_pBoneCollider->SetName(L"Sword_Bone_Collider");

		m_pBoneCollider->AddComponent(new CTransform);
		m_pBoneCollider->AddComponent(new CCollider3D);

		m_pBoneCollider->Collider3D()->SetOffsetScale(Vec3(30.f, 10.f, 200.f));
		CSceneMgr::GetInst()->GetCurScene()->AddObject(m_pBoneCollider, L"PLAYER");

		pObjWeapon->AddChild(m_pBoneCollider);

		// Sword Trail Test
		CGameObject* pGameObj = new CGameObject;
		pGameObj->SetName(L"Sword_Trail");
		pGameObj->AddComponent(new CTransform);

		SwordTrailScript* pSwordTrailScript = new SwordTrailScript;
		pGameObj->AddComponent(pSwordTrailScript);
		pGameObj->Transform()->SetIgnoreParent(true);

		pObjWeapon->AddChild(pGameObj);

		pObj = new CGameObject;
		pObj->SetName(L"Trail");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		auto pAnimator = new CAnimator3D;
		pObj->AddComponent(pAnimator);

		//TrailScript* pTrailScript = new TrailScript;
		//pTrailScript->SetOriginObject(pPlayer);
		//pObj->AddComponent(pTrailScript);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword0.mdat",
		                                                L"meshdata\\player_sword0.mdat");
		pObj->MeshRender()->SetMesh(pMeshData->GetMesh());
		for (int i = 0; i < 4; ++i)
		{
			pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TrailMtrl.mtrl"),
			                                      i);
		}

		pAnimator->SetBones(pMeshData->GetMesh()->GetBones());
		pAnimator->SetAnimClip(pMeshData->GetMesh()->GetAnimClip());

		_pScene->AddObject(pObj, L"PLAYER");
	}

	void AddBoss(CScene* _pScene)
	{
		CGameObject* pBoss = new CGameObject;
		pBoss->SetName(L"BOSS_COMBAT");
		pBoss->AddComponent(new CTransform);
		pBoss->AddComponent(new BossJugCombatMgrScript);
		_pScene->AddObject(pBoss, L"MONSTER");
		//pBoss->GetScript<BossJugCombatMgrScript>()->SpawnStage();
	}


	void LoadScene()
	{
		wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
		CScene* pCurScene        = CSceneSaveLoad::LoadScene(strSceneFilePath + L"scene\\GUN.scene");
		CSceneMgr::GetInst()->ChangeScene(pCurScene);
	}

	void TestUI(CScene* _pScene)
	{
		Ptr<CMesh>    pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		Ptr<CTexture> pPanelTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\UI_NameBox_Tex.png",
		                                                             L"texture\\UI\\UI_NameBox_Tex.png");
		Ptr<CTexture> pTexture_1 = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\StageBGUI_Tex.png",
		                                                              L"texture\\UI\\StageBGUI_Tex.png");
		Ptr<CTexture> pTexture_2 = CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\Cursor.png",
		                                                              L"texture\\UI\\Cursor.png");
		Ptr<CMaterial> pUIMaterial = CResMgr::GetInst()->Load<CMaterial>(L"material\\UIMtrl.mtrl",
		                                                                 L"material\\UIMtrl.mtrl");
		//UI CAMERA
		CGameObject* pUICamera = new CGameObject{};
		pUICamera->SetName(L"UICamera");
		pUICamera->AddComponent(new CTransform{});
		pUICamera->AddComponent(new CCamera{});
		pUICamera->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICamera->Camera()->CheckLayerMask(L"UI_STATIC");
		pUICamera->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
		pUICamera->AddComponent(new CameraMoveScript{});
		_pScene->AddObject(pUICamera, L"CAMERA");

		//PANEL UI
		CGameObject* pPanelUI = new CGameObject{};
		pPanelUI->SetName(L"UI_PANEL");
		pPanelUI->AddComponent(new CTransform{});
		pPanelUI->Transform()->SetRelativeScale(500.f, 500.f, 1.f);
		pPanelUI->AddComponent(new CMeshRender{});
		pPanelUI->MeshRender()->SetMesh(pRectMesh);
		pPanelUI->MeshRender()->SetSharedMaterial(pUIMaterial, 0);
		pPanelUI->AddComponent(new UIPanelScript{});
		pPanelUI->GetScript<UIPanelScript>()->SetTexture(L"texture\\UI\\UI_NameBox_Tex.png");
		_pScene->AddObject(pPanelUI, L"UI_STATIC");

		//IMAGE UI_1
		CGameObject* pImageUI_1 = new CGameObject{};
		pImageUI_1->SetName(L"UI_1");
		pImageUI_1->AddComponent(new CTransform{});
		pImageUI_1->Transform()->SetRelativeScale(400.f, 400.f, 1.f);
		pImageUI_1->AddComponent(new CMeshRender{});
		pImageUI_1->MeshRender()->SetMesh(pRectMesh);
		pImageUI_1->MeshRender()->SetSharedMaterial(pUIMaterial, 0);
		pImageUI_1->AddComponent(new UIImageScript{});
		pImageUI_1->GetScript<UIImageScript>()->SetPreserveRatio(true);
		pImageUI_1->GetScript<UIImageScript>()->SetTexture(L"texture\\UI\\StageBGUI_Tex.png");
		pPanelUI->AddChild(pImageUI_1);

		//IMAMGE_UI_2
		CGameObject* pImageUI_2 = new CGameObject{};
		pImageUI_2->SetName(L"UI_2");
		pImageUI_2->AddComponent(new CTransform{});
		pImageUI_2->Transform()->SetRelativeScale(5.f, 5.f, 1.f);
		pImageUI_2->AddComponent(new CMeshRender{});
		pImageUI_2->MeshRender()->SetMesh(pRectMesh);
		pImageUI_2->MeshRender()->SetSharedMaterial(pUIMaterial, 0);
		pImageUI_2->AddComponent(new UIImageScript{});
		pImageUI_2->GetScript<UIImageScript>()->SetPreserveRatio(true);
		//pImageUI_2->GetScript<UIImageScript>()->SetAnchorH(ANCHOR_HORIZONTAL::LEFT);
		pImageUI_2->GetScript<UIImageScript>()->SetTexture(L"texture\\UI\\Cursor.png");
		auto pScript = new TestScript{};
		pImageUI_2->AddComponent(pScript);
		pImageUI_2->GetScript<UIImageScript>()->SetMouseHoverCallback(pScript, &TestScript::MouseHover);
		pImageUI_2->AddComponent(new ButtonScript{});
		pImageUI_2->GetScript<ButtonScript>()->SetCallback(pScript,
		                                                   &TestScript::OneClick,
		                                                   MOUSE_TYPE::LEFT,
		                                                   CLICK_TYPE::ONE);
		pPanelUI->AddChild(pImageUI_2);
	}

	void CreateScene()
	{
		auto pCurScene = new CScene;
		CSceneMgr::GetInst()->ChangeScene(pCurScene);
		SetLayer(pCurScene);
		LoadTextures();

		CGameObject* pCamObj = AddCamera(pCurScene);
		AddDirectionalLight(pCurScene);
		AddSkybox(pCurScene);
		//AddPointLight(pCurScene);
		//AddParticle(pCurScene);
		//AddLandScape(pCurScene);
		//AddDecal(pCurScene);
		//AddSphere(pCureScene);
		//AddTessellation(pCurScene);

		AddPlayer(pCurScene, pCamObj);
		AddBoss(pCurScene);

		//TestUI(pCurScene);

		// 충돌 레이어 설정
		CCollisionMgr::GetInst()->CollisionCheck(1, 1);

		pCurScene->SetResKey(L"scene\\TestScene.scene");
		wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
		CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");
		pCurScene->SetSceneState(SCENE_STATE::STOP);
	}
}

void CTestScene::CreateTestScene()
{
	CreateScene();
}
