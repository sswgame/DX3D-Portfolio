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
#include <Engine/CNaviMap.h>
#include <Engine/CNaviAgent.h>

#include <Engine/CRigidBody.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CUIBase.h>

// [SCRIPT TYPE]
#include <Engine/CRenderMgr.h>
#include <Script/GameDefine.h>
#include <Script/PlayerScript.h>
#include <Script/CameraMoveScript.h>
#include <Script/PlayerCamScript.h>
#include <Script/BossJugCombatMgrScript.h>
#include <Script/CSceneSaveLoad.h>
#include <Script/TrailScript.h>
#include <Script/SwordTrailScript.h>
#include <Script/FieldMonsteScript.h>
#include <Script/ItemScript.h>
#include <Script/PaperBurnScript.h>
#include <Script/CObjectManager.h>
#include <Script/CinemaCamScript.h>


//TEST
#include <Script/TestDeadScript.h>

namespace
{
	CGameObject* AddCamera(CScene* _pScene);
	CGameObject* AddCinemaCamera(CScene* _pScene);


	void         AddDirectionalLight(CScene* _pScene);
	void         AddSkybox(CScene* _pScene);

	void AddPointLight(CScene* _pScene);
	void AddParticle(CScene* _pScene);
	void AddLandScape(CScene* _pScene);
	void AddDecal(CScene* _pScene);
	void AddSphere(CScene* _pScene);
	void AddTessellation(CScene* _pScene);

	void AddFogTexture(CScene* _pScene);
	void AddShakeScreenTest(CScene* _pScene);

	void AddItem(CScene* _pScene);
	void AddPlayer(CScene* _pScene, CGameObject* _pCamera);
	void AddBoss(CScene* _pScene);
	void AddHomonculus(CScene* _pScene);
	void AddDeuxiemie(CScene* _pScene);

	void TestNavi(CScene* _pScene);

	void AddDefaultUIObjects(CScene* _pScene);
	void Map01(CScene* _pScene);
	void Map02(CScene* _pScene);
	void TestBossUI(CScene* _pScene);

	void SetLayer(CScene* _pScene);
	void SetCollision();
	void LoadScene();
	void SaveScene(CScene* _pScene, const std::wstring& _relativePath);

	void TestSineDistortion(CScene* _pScene)
	{
		CGameObject* pGameObject = new CGameObject{};
		pGameObject->SetName(L"DOOR");
		pGameObject->AddComponent(new CTransform{});
		pGameObject->AddComponent(new CMeshRender{});
		pGameObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pGameObject->MeshRender()->
		             SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SineWaveMtrl.mtrl"), 0);
		pGameObject->Transform()->SetRelativePos(0.f, 290.f, 0.f);
		pGameObject->Transform()->SetRelativeScale(500.f, 580.f, 1.f);
		pGameObject->MeshRender()
		           ->GetMaterial(0)
		           ->SetTexParam(TEX_PARAM::TEX_0,
		                         CResMgr::GetInst()->
		                         FindRes<CTexture>(L"texture\\FBXTexture\\CardianlSwords_BaseColorAlpha.png"));

		CGameObject* pGate = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\gate.mdat")->Instantiate();
		pGate->SetName(L"GATE");
		pGate->AddChild(pGameObject);
		pGate->Transform()->SetRelativePos(0.f, 0.f, 2000.f);
		_pScene->AddObject(pGate, 0);
	}

	void CreateScene()
	{
		const auto pCurScene = new CScene;
		CSceneMgr::GetInst()->ChangeScene(pCurScene);
		SetLayer(pCurScene);

		CGameObject* pCamObj = AddCamera(pCurScene);
		AddDirectionalLight(pCurScene);
		AddSkybox(pCurScene);

		//AddCinemaCamera(pCurScene);
		AddPlayer(pCurScene, pCamObj);
		//AddDeuxiemie(pCurScene);
		//AddHomonculus(pCurScene);
		AddDefaultUIObjects(pCurScene);

		//TestSineDistortion(pCurScene);
		//Map01(pCurScene);
		Map02(pCurScene);

		SetCollision();

		SaveScene(pCurScene, L"scene\\TestScene.scene");

		pCurScene->SetSceneState(SCENE_STATE::STOP);
	}
}

void CTestScene::CreateTestScene()
{
	CreateScene();
}

//IMPLEMENTATION
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

	void SetCollision()
	{
		/// 충돌 레이어 설정
		CCollisionMgr::GetInst()->CollisionCheck(GAME::LAYER::PLAYER, GAME::LAYER::MONSTER);
		CCollisionMgr::GetInst()->CollisionCheck(GAME::LAYER::PLAYER, GAME::LAYER::ITEM);
		CCollisionMgr::GetInst()->CollisionCheck(GAME::LAYER::PLAYER, GAME::LAYER::MONSTER_PARRING_ATTACK);
		CCollisionMgr::GetInst()->CollisionCheck(GAME::LAYER::PLAYER, GAME::LAYER::MONSTER_NON_PARRING_ATTACK);
	}

	void SaveScene(CScene* _pScene, const std::wstring& _relativePath)
	{
		_pScene->SetResKey(_relativePath);
		const wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
		CSceneSaveLoad::SaveScene(_pScene, strSceneFilePath + _relativePath);
	}

	CGameObject* AddCamera(CScene* _pScene)
	{
		const auto pCamera = new CGameObject;
		pCamera->SetName(L"MainCamera");
		pCamera->AddComponent(new CTransform);
		pCamera->AddComponent(new CCamera);
		//pCamera->AddComponent(new CameraMoveScript);
		pCamera->AddComponent(new PlayerCamScript);

		pCamera->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pCamera->Camera()->SetCameraAsMain();
		pCamera->Camera()->CheckLayerMaskAll();
		pCamera->Camera()->CheckLayerMask(L"UI_STATIC", false);
		pCamera->Camera()->CheckLayerMask(L"UI_INTERACTIVE", false);
		pCamera->Camera()->SetShowFrustum(true);

		_pScene->AddObject(pCamera, L"CAMERA");
		CObjectManager::GetInst()->AddToDontDestroy(pCamera);
		return pCamera;
	}

	CGameObject* AddCinemaCamera(CScene* _pScene)
	{
		const auto pCamera = new CGameObject;
		pCamera->SetName(L"CinemaCamera");
		pCamera->AddComponent(new CTransform);
		pCamera->AddComponent(new CCamera);
		pCamera->AddComponent(new CinemaCamScript);

		pCamera->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		pCamera->Camera()->SetCameraAsMain();
		pCamera->Camera()->CheckLayerMaskAll();
		pCamera->Camera()->CheckLayerMask(L"UI_STATIC", false);
		pCamera->Camera()->CheckLayerMask(L"UI_INTERACTIVE", false);
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

		CObjectManager::GetInst()->AddToDontDestroy(pDirectionalLight);
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

		_pScene->AddObject(pParticleObj, L"BG");
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
		CObjectManager::GetInst()->AddToDontDestroy(pSkyBox);
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
		//Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
		//                                                            L"texture\\tile\\TILE_01.tga");
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
		//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);

		_pScene->AddObject(pObject, L"Default");
	}

	void AddTessellation(CScene* _pScene)
	{
		//Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga",
		//                                                            L"texture\\tile\\TILE_01.tga");
		//Ptr<CTexture> pTileNTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga",
		//                                                             L"texture\\tile\\TILE_01_N.tga");

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
		//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTileTex);
		//pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1, pTileNTex);

		_pScene->AddObject(pObject, L"Default");
	}

	void AddFogTexture(CScene* _pScene)
	{
		Ptr<CTexture> fogTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\particle\\smokeparticle.png",
		                                                          L"texture\\particle\\smokeparticle.png");

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Fog");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativePos(-12, 246, -4085);
		pObject->Transform()->SetRelativeScale(2500, 2500, 1.f);

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\fog.mtrl"), 0);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, fogTex);
		pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1,
		                                                   (CResMgr::GetInst()->FindRes<
			                                                   CTexture>(L"PositionTargetTex")));

		_pScene->AddObject(pObject, L"BG");

		CGameObject* pObject1;
		pObject1 = pObject->Clone();
		pObject1->Transform()->SetRelativePos(-12, 3, -1615);
		pObject1->Transform()->SetRelativeScale(10000, 10000, 1.f);
		pObject1->Transform()->SetRelativeRotation(XM_PI / 2.f, 0.f, 0.f);
		_pScene->AddObject(pObject1, L"BG");
	}

	void AddItem(CScene* _pScene)
	{
		CPrefab* pPrefabBall = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\item_circle.pref",
		                                                         L"prefab\\item_circle.pref").Get();
		CGameObject* parti = pPrefabBall->Instantiate();
		parti->SetName(L"HP Cure Ball");
		parti->AddComponent(new CCollider3D);
		parti->AddComponent(new ItemScript);

		parti->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		parti->Collider3D()->SetOffsetPos(Vec3(0, 0.f, 0.f));
		parti->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));
		parti->Transform()->SetRelativePos(Vec3(500.f, 00.f, 0.f));
		parti->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
		parti->ParticleSystem()->SetLifeTime(-1.f);
		parti->ParticleSystem()->SetMaterial(L"material\\item_circle.mtrl");
		parti->GetScript<ItemScript>()->SetItemType(ItemType::ITEM_HEALING);
		_pScene->AddObject(parti, L"ITEM");


		CPrefab*     pPrefab     = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\item.pref", L"prefab\\item.pref").Get();
		CGameObject* partiEffect = pPrefab->Instantiate();
		partiEffect->SetName(L"HP Cure Effect");
		partiEffect->Transform()->SetRelativePos(Vec3(500.f, 0.f, 0.f));
		partiEffect->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		partiEffect->ParticleSystem()->SetLifeTime(-1.f);
		partiEffect->ParticleSystem()->SetMaterial(L"material\\item.mtrl");
		_pScene->AddObject(partiEffect, L"BG");
	}

	void AddPlayer(CScene* _pScene, CGameObject* _pCamera)
	{
		CGameObject* pObj = nullptr;

		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword0.mdat",
		                                                               L"meshdata\\player_sword0.mdat");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"player");
		pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pObj->Animator3D()->Play(L"test", true);
		pObj->Animator3D()->GetCurAnim()->SetPlay(false);
		pObj->Animator3D()->MakeAnimationFromTXT_Extended("PlayerAnimInfo2.txt");
		pObj->Animator3D()->SetPlayWithChild(true);

		pObj->MeshRender()->SetDynamicShadow(true);


		pObj->AddComponent(new CFSM);
		pObj->AddComponent(new CRigidBody);
		pObj->AddComponent(new CCollider3D);
		pObj->AddComponent(new CNaviAgent);
		pObj->AddComponent(new TrailScript);
		pObj->AddComponent(new PaperBurnScript);

		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObj->Collider3D()->SetOffsetPos(Vec3(0.f, 92.f, 0.f));
		pObj->Collider3D()->SetOffsetScale(Vec3(75.f, 175.f, 75.f));

		pObj->NaviAgent()->SetOffsetPos(Vec3(0.f, 92.f, 0.f));
		pObj->NaviAgent()->SetOffsetSize(Vec3(75.f, 175.f, 75.f));

		PlayerScript* pPlayerScript = new PlayerScript;
		pPlayerScript->SetCamera(_pCamera);
		pObj->AddComponent(pPlayerScript);

		CGameObject* pPlayer = pObj;
		_pScene->AddObject(pObj, L"PLAYER");

		//ADD TO DON DESTROY
		CObjectManager::GetInst()->AddToDontDestroy(pObj);
		CObjectManager::GetInst()->SetPlayer(pObj);
		//pObj->Deactivate();
		CObjectManager::GetInst()->SetSceneObject(pObj, MAP_TYPE::_01);

		Ptr<CMeshData> pMeshDataWeapon = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\player_sword1.mdat",
		                                                                     L"meshdata\\player_sword1.mdat");

		CGameObject* pObjWeapon = pMeshDataWeapon->Instantiate();
		pObjWeapon->SetName(L"player_sword1");
		pObjWeapon->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pObjWeapon->Animator3D()->Play(L"test", true);

		pObjWeapon->MeshRender()->SetDynamicShadow(true);

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

		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\player_sword0.mdat");
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
		_pScene->AddObject(pBoss, GAME::LAYER::OBJECT_MGR);
		pBoss->GetScript<BossJugCombatMgrScript>()->SpawnStage();

		CObjectManager::GetInst()->SetSceneObject(pBoss, MAP_TYPE::_02);
	}

	void AddHomonculus(CScene* _pScene)
	{
		wstring        sMeshName = L"meshdata\\homonculus0.mdat";
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(sMeshName);
		CGameObject*   pMonster;
		pMonster = pMeshData->Instantiate();
		pMonster->SetName(L"Homonculus");
		pMonster->AddComponent(new FieldMonsteScript);
		pMonster->AddComponent(new CFSM);
		pMonster->AddComponent(new CCollider3D);
		pMonster->AddComponent(new CRigidBody);
		pMonster->AddComponent(new CNaviAgent);

		pMonster->MeshRender()->SetDynamicShadow(true);

		pMonster->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
		pMonster->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));

		pMonster->Transform()->SetRelativePos(Vec3(0.f, 5.f, 100.f));
		pMonster->NaviAgent()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));
		pMonster->NaviAgent()->SetOffsetSize(Vec3(15.f, 200.f, 15.f));

		pMonster->GetScript<FieldMonsteScript>()->SetFieldMonsterType(FieldMonsterType::HOMONCULUS);
		pMonster->GetScript<FieldMonsteScript>()->SetDetectRange(300.f);
		pMonster->GetScript<FieldMonsteScript>()->SetAttackRange(30.f);

		_pScene->AddObject(pMonster, L"MONSTER");
		CGameObject* pMonsterHP = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MONSTER_HP.pref")->Instantiate();
		pMonsterHP->GetUIBaseComponenent()->SetTarget(pMonsterHP);
		_pScene->AddObject(pMonsterHP, L"UI_INTERACTIVE");

		//TODO::MUST DELETE
		pMonster->AddComponent(new TestDeadScript{});
	}

	void AddDeuxiemie(CScene* _pScene)
	{
		wstring        sMeshName = L"meshdata\\deuxiemie_SmallSword0.mdat";
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(sMeshName);

		CGameObject* pMonster = pMeshData->Instantiate();
		pMonster->SetName(L"Deuxiemie");
		pMonster->AddComponent(new FieldMonsteScript);
		pMonster->AddComponent(new CFSM);
		pMonster->AddComponent(new CCollider3D);
		pMonster->Collider3D()->SetOffsetScale(Vec3(50.f, 100.f, 50.f));
		pMonster->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		pMonster->GetScript<FieldMonsteScript>()->SetFieldMonsterType(FieldMonsterType::DEUXIEMIE);
		_pScene->AddObject(pMonster, L"MONSTER");

		CGameObject* pMonsterHP = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MONSTER_HP.pref")->Instantiate();
		pMonsterHP->GetUIBaseComponenent()->SetTarget(pMonsterHP);
		_pScene->AddObject(pMonsterHP, L"UI_INTERACTIVE");

		//TODO::MUST DELETE
		pMonster->AddComponent(new TestDeadScript{});
	}

	void TestNavi(CScene* _pScene)
	{
		/// Navi Mesh Test
		//CGameObject* pNMesh = new CGameObject;
		//pNMesh->SetName(L"NaviMesh");
		//pNMesh->AddComponent(new CTransform);
		//pNMesh->AddComponent(new CNaviMap);

		//CNaviMapData* pNaviMap = CResMgr::GetInst()->Load<CNaviMapData>(L"navimap\\arena.map", L"navimap\\arena.map").
		//                                             Get();
		//pNMesh->NaviMap()->SetNaviMapData(pNaviMap);
		//pNMesh->Transform()->SetRelativeScale(30.f, 30.f, 30.f);
		//pNMesh->Transform()->SetRelativePos(0.f, -200.f, 0.f);

		//_pScene->AddObject(pNMesh, GAME::LAYER::BG_OBJ);

		/// Player NaviAgent Test
		//CGameObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(GAME::LAYER::PLAYER)->
		//                                             FindRootObject(L"player");
		//pPlayer->NaviAgent()->SetNaviMap(pNMesh->NaviMap());
		//pPlayer->NaviAgent()->SetOffsetSize(Vec3(10.f, 200.f, 100.f));
		//pPlayer->NaviAgent()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));

		/// NaviAgent Test
		//CGameObject* pAgent = new CGameObject;
		//pAgent->SetName(L"Agent");
		//pAgent->AddComponent(new CTransform);
		//pAgent->AddComponent(new CNaviAgent);

		//pAgent->Transform()->SetRelativePos(-300.f, 0.f, 11.f);
		//pAgent->Transform()->SetRelativeScale(300.f, 300.f, 300.f);
		//pAgent->NaviAgent()->SetNaviMap(pNMesh->NaviMap());

		//_pScene->AddObject(pAgent, GAME::LAYER::PLAYER);


		/// Save
		//CNaviMapData* pNaviMap = new CNaviMapData;
		//pNaviMap->CreateFromObj(L"arena.obj", L"mesh\\Navi_arena.mesh");
		//wstring strPath = L"navimap\\arena.map";
		//pNaviMap->Save(CPathMgr::GetInst()->GetContentPath() + strPath);

		/// Load
		//CNaviMapData* pNaviMap = CResMgr::GetInst()->Load<CNaviMapData>(L"navimap\\arena.map", L"navimap\\arena.map").Get();
	}

	void AddDefaultUIObjects(CScene* _pScene)
	{
		//UI CAMERA
		CGameObject* pUICamera = new CGameObject{};
		pUICamera->SetName(L"UICamera");
		pUICamera->AddComponent(new CTransform{});
		pUICamera->AddComponent(new CCamera{});
		pUICamera->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICamera->Camera()->CheckLayerMask(L"UI_STATIC");
		pUICamera->Camera()->CheckLayerMask(L"UI_INTERACTIVE");
		_pScene->AddObject(pUICamera, L"CAMERA");
		CObjectManager::GetInst()->AddToDontDestroy(pUICamera);

		//PLAYER UI
		CGameObject* pPlayerUI = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\PLAYER_UI_PANEL.pref")->Instantiate();
		CObjectManager::GetInst()->SetPlayerUI(pPlayerUI);
		_pScene->AddObject(pPlayerUI, L"UI_INTERACTIVE");
		//CObjectManager::GetInst()->AddToDontDestroy(pPlayerUI);

		//MAIN_MENU
		////CGameObject* pMainUI = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MAIN_MENU.pref")->Instantiate();
		//_pScene->AddObject(pMainUI, L"UI_INTERACTIVE");

		//BOSS_UI
		CGameObject* pBossUI = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BOSS_HP_PANEL.pref")->Instantiate();
		_pScene->AddObject(pBossUI, L"UI_INTERACTIVE");
		CObjectManager::GetInst()->SetBossUI(pBossUI);
		//pBossUI->Deactivate();
		//CObjectManager::GetInst()->SetSceneObject(pBossUI, MAP_TYPE::_02);
	}

	void Map01(CScene* _pScene)
	{
		CGameObject* pMap01 = new CGameObject;
		pMap01->SetName(L"Map01");
		pMap01->AddComponent(new CTransform);
		pMap01->Transform()->SetRelativePos(Vec3(0.f, -200.f, 0.f));
		for (size_t i = 0; i < 4; i++)
		{
			wstring path = L"meshdata\\arene_stage" + std::to_wstring(i) + L".mdat";
			wstring name = L"arene_stage" + std::to_wstring(i);

			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(path, path);

			CGameObject* pStage = pMeshData->Instantiate();
			pStage->SetName(name);
			pMap01->AddChild(pStage);
		}

		CGameObject* pNMesh = new CGameObject;
		pNMesh->SetName(L"NaviMesh");
		pNMesh->AddComponent(new CTransform);
		pNMesh->AddComponent(new CNaviMap);

		CNaviMapData* pNaviMap = CResMgr::GetInst()->FindRes<CNaviMapData>(L"navimap\\arena.map").Get();
		pNMesh->NaviMap()->SetNaviMapData(pNaviMap);
		pNMesh->Transform()->SetRelativeScale(18.2f, 18.2f, 18.2f);
		pNMesh->Transform()->SetRelativePos(0.f, -5.f, 0.f);
		Vec3 rot = Vec3(0.f, 180.4f, 0.f);
		rot.ToRadian();
		pNMesh->Transform()->SetRelativeRotation(rot);

		pMap01->AddChild(pNMesh);

		_pScene->AddObject(pMap01, GAME::LAYER::BG);

		//SCENE OBJECT
		CObjectManager::GetInst()->SetSceneObject(pMap01, MAP_TYPE::_01);
	}

	void Map02(CScene* _pScene)
	{
		AddBoss(_pScene);
	}

	void AddShakeScreenTest(CScene* _pScene)
	{
		CGameObject* pScreenShakeObj = new CGameObject;
		pScreenShakeObj->AddComponent(new CTransform);
		pScreenShakeObj->AddComponent(new CMeshRender);

		Ptr<CMesh>     pMesh     = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ScreenShakeMtrl.mtrl");
		pScreenShakeObj->MeshRender()->SetMesh(pMesh);
		pScreenShakeObj->MeshRender()->SetSharedMaterial(pMaterial, 0);


		_pScene->AddObject(pScreenShakeObj, GAME::LAYER::PLAYER);
	}

}
