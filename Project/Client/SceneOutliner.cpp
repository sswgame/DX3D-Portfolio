#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>
#include <Engine/CTransform.h>

#include <Engine/CEventMgr.h>
#include <Engine/CRenderComponent.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CDevice.h>
#include <Engine/CMesh.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "IconsFontAwesome5.h"
#include "ListUI.h"
#include "TransformUI.h"

#include "ImGuizmo/ImGuizmo.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui_internal.h"

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutliner");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);
	m_TreeUI->UseDragDropOuter(true);
	m_TreeUI->UseDragDropSelf(true);


	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, static_cast<CLICKED>(&SceneOutliner::ObjectClicked));

	// Drag and Drop Delegate 등록
	m_TreeUI->SetDragAndDropDelegate(this, static_cast<DRAG_DROP>(&SceneOutliner::DragAndDropDelegate));

	// Key Delegate 등록
	m_TreeUI->SetKeyBinding(KEY::DEL, this, static_cast<CLICKED>(&SceneOutliner::PressDelete));


	//Reset();
}

SceneOutliner::~SceneOutliner() {}

void SceneOutliner::update()
{
	m_pSelectedScene = CSceneMgr::GetInst()->GetCurScene();

	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();
	}

	UI::update();
}

void SceneOutliner::render_update()
{
	//마우스 우측 클릭으로 마우스 위치에 팝업 만들기
	const ImVec2   windowLeftTop = ImGui::GetCursorScreenPos();
	const ImVec2   windowSize    = ImGui::GetContentRegionAvail();
	const ImGuiIO& io            = ImGui::GetIO();
	const ImVec2   mousePos      = io.MousePos - windowLeftTop;

	static bool bIsOpenPopUp = false;
	if (0 < mousePos.x && mousePos.x < windowSize.x
	    && 0 < mousePos.y && mousePos.y < windowSize.y)
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("SUB_TOOL_POPUP");
			bIsOpenPopUp = true;
		}
	}

	static bool bOpenOverwritePopUp = false;

	if (bIsOpenPopUp)
	{
		static const std::string menus[] = {
			"Make as Prefab",
			"Create GameObject"
		};
		if (ImGui::BeginPopup("SUB_TOOL_POPUP"))
		{
			for (int i = 0; i < std::size(menus); ++i)
			{
				if (ImGui::Selectable(menus[i].c_str()))
				{
					if (menus[i] == "Make as Prefab")
					{
						static bool bResult{};
						bResult = MakePrefab();
						if (false == bResult && m_pSelectedGameObject)
						{
							bOpenOverwritePopUp = true;
						}
					}
					else if (menus[i] == "Create GameObject")
					{
						CreateGameObject();
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	if (bOpenOverwritePopUp)
	{
		ImGui::OpenPopup("OVERWRITE");
		if (ImGui::BeginPopupModal("OVERWRITE", &bOpenOverwritePopUp))
		{
			ImGui::Text(u8"덮어쓰시겠습니까?");

			if (ImGui::Button("OK##OVERWRITE"))
			{
				m_bOverwritePrefab  = true;
				bOpenOverwritePopUp = false;
				MakePrefab();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	RenderAddObject();
	RenderAddLayer();
}

void SceneOutliner::Reset()
{
	ResetTreeUI();

	// InspectorUI 를 얻어옴
	auto pInspectorUI = static_cast<InspectorUI*>(CImGuiMgr::GetInst()->FindUI("Inspector"));
	pInspectorUI->SetTargetObject(nullptr);
	pInspectorUI->SetTargetLayer(nullptr);
	pInspectorUI->SetTargetScene(nullptr);

	// SceneOutlinerUI 갱신
	m_pSelectedScene      = nullptr;
	m_pSelectedLayer      = nullptr;
	m_pSelectedGameObject = nullptr;
}

void SceneOutliner::ResetTreeUI()
{
	m_TreeUI->Clear();

	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// 1. SCENE NODE 
	TreeNode* pSceneNode = AddSceneToTree(pCurScene, m_TreeUI->GetDummyNode());

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		if (pLayer->GetName() == L"") continue;

		// 2. LAYER NODE
		TreeNode* pLayerNode = AddLayerToTree(pLayer, pSceneNode);

		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();
		for (size_t k = 0; k < vecRoots.size(); ++k)
		{
			// 3. GAMEOBJECT NODE
			AddGameObjectToTree(vecRoots[k], pLayerNode);
		}
	}
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	auto pNode = (TreeNode*)_dw;

	string    strKey       = pNode->GetName();
	NODE_TYPE NodeType     = pNode->GetNodeType();
	DWORD_PTR pData        = pNode->GetData();
	auto      pInspectorUI = static_cast<InspectorUI*>(CImGuiMgr::GetInst()->FindUI("Inspector"));

	switch (NodeType)
	{
	case NODE_TYPE::ENGINE_SCENE:
		{
			auto pScene = (CScene*)pData;
			assert(pScene);
			pInspectorUI->SetTargetScene(pScene);
			pInspectorUI->SetTargetLayer(nullptr);
			pInspectorUI->SetTargetObject(nullptr);

			m_pSelectedScene      = pScene;
			m_pSelectedLayer      = nullptr;
			m_pSelectedGameObject = nullptr;
		}
		break;
	case NODE_TYPE::ENGINE_LAYER:
		{
			auto pLayer = (CLayer*)pData;
			assert(pLayer);
			pInspectorUI->SetTargetLayer(pLayer);
			pInspectorUI->SetTargetObject(nullptr);

			m_pSelectedLayer      = pLayer;
			m_pSelectedGameObject = nullptr;
		}
		break;
	case NODE_TYPE::ENGINE_GAMEOBJECT:
		{
			auto pObj = (CGameObject*)pData;
			assert(pObj);
			pInspectorUI->SetTargetObject(pObj);

			m_pSelectedGameObject = pObj;
		}
		break;
	}
}

void SceneOutliner::RenderAddObject()
{
	// [ ADD GAMEOBJECT BUTTON ]
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(153.f / 255.f, 153.f / 255.f, 153.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(102.f / 255.f, 102.f / 255.f, 102.f / 255.f, 1.f));
	string ButtonName = ICON_FA_PLUS;
	ButtonName += "Add GameObject";
	if (ImGui::Button(ButtonName.c_str(), ImVec2(150.f, 25.f)))
	{
		ImGui::OpenPopup("Create New Obj");
	}
	ImGui::PopStyleColor(2);

	ImGui::SameLine();
	ImGui::Checkbox("Enable Picking", &m_bEnablePicking);

	if (m_bEnablePicking
	    && KEY_TAP(KEY::LBTN)
	    && CSceneMgr::GetInst()->GetCurScene()->GetSceneState() != SCENE_STATE::PLAY)
	{
		ObjectPicking();
	}


	bool unused_open = true;
	if (ImGui::BeginPopupModal("Create New Obj", &unused_open))
	{
		if (nullptr != m_pSelectedScene)
		{
			ImGui::Text("select Scene - ");
			ImGui::SameLine();
			string sName = ToString(m_pSelectedScene->GetName());
			if (sName == "")
				sName = "NO NAME";
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.5f, 1.f), sName.c_str());
		}
		else
			ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT SCENE!");

		if (nullptr != m_pSelectedLayer)
		{
			ImGui::Text("select Layer - ");
			ImGui::SameLine();
			string sName = ToString(m_pSelectedLayer->GetName());
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.8f, 1.f), sName.c_str());
		}
		else
			ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT LAYER!");


		ImGui::Text("Show Prefab List");
		ImGui::SameLine();
		if (ImGui::Button("##PrefabListbtn", Vec2(15, 15)))
		{
			// ListUI 활성화한다.
			const map<wstring, CRes*>& mapRes  = CResMgr::GetInst()->GetResList(RES_TYPE::PREFAB);
			auto                       pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
			pListUI->Clear();
			pListUI->SetTitle("Prefab List");

			for (const auto& pair : mapRes)
			{
				pListUI->AddList(ToString(pair.first));
			}

			pListUI->Activate();
			pListUI->SetDBCEvent(this, static_cast<DBCLKED>(&SceneOutliner::SelectPrefab));
		}


		static char buf[512];
		ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

		if (ImGui::Button("Complete"))
		{
			string name    = buf;
			auto   newName = wstring(name.begin(), name.end());

			auto NewObj = new CGameObject;
			NewObj->SetName(newName);

			// Create GameObject
			if (nullptr != m_pSelectedScene && nullptr != m_pSelectedLayer)
			{
				// 기본적으로 CTransform 을 갖는다. 
				NewObj->AddComponent(new CTransform);

				NewObj->Transform()->SetRelativePos(0.f, 0.f, 0.f);
				NewObj->Transform()->SetRelativeScale(200.f, 200.f, 1.f);


				m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());

				// TReeUI 에 추가하기 위해서 Reset() 
				ResetTreeUI();
				char empty[512] = {NULL,};
				strcpy_s(buf, empty);
				ImGui::CloseCurrentPopup();
			}

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void SceneOutliner::RenderAddLayer() {}

void SceneOutliner::RenderGuizmo()
{
	if (nullptr == m_pSelectedGameObject || false == TransformUI::IsGuizmoEnabled())
	{
		return;
	}

	const CCamera* pCamera = CRenderMgr::GetInst()->GetMainCam();
	ImGuizmo::SetOrthographic(pCamera->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC);

	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
	ImGuizmo::SetRect(ImGui::GetWindowPos().x,
	                  ImGui::GetWindowPos().y,
	                  ImGui::GetWindowViewport()->Size.x,
	                  ImGui::GetWindowViewport()->Size.y);

	Matrix modifiedMatrix = m_pSelectedGameObject->Transform()->GetWorldMat();
	Manipulate((float*)pCamera->GetViewMat().m,
	           (float*)pCamera->GetProjMat().m,
	           static_cast<ImGuizmo::OPERATION>(TransformUI::GetGuizmoType()),
	           ImGuizmo::LOCAL,
	           (float*)modifiedMatrix.m);

	if (ImGuizmo::IsUsing())
	{
		Vec3       modifiedTranslation{};
		Quaternion modifiedRotation{};
		Vec3       modifiedScale{};
		modifiedMatrix.Decompose(modifiedScale, modifiedRotation, modifiedTranslation);

		Vec3       originTranslation{};
		Quaternion originRotation{};
		Vec3       originScale{};
		Matrix     origin = m_pSelectedGameObject->Transform()->GetWorldMat();
		origin.Decompose(originScale, originRotation, originTranslation);


		switch (TransformUI::GetGuizmoType())
		{
		case GUIZMO_TYPE::TRANSLATE:
			{
				m_pSelectedGameObject->Transform()->SetRelativePos(modifiedTranslation);
				break;
			}
		case GUIZMO_TYPE::ROTATE:
			{
				const Vec3 originEuler   = DecomposeRotMat(Matrix::CreateFromQuaternion(originRotation));
				Vec3       modifiedEuler = DecomposeRotMat(Matrix::CreateFromQuaternion(modifiedRotation));
				modifiedEuler -= originEuler;

				const Vec3 relativeRotation = m_pSelectedGameObject->Transform()->GetRelativeRotation();

				m_pSelectedGameObject->Transform()->SetRelativeRotation(relativeRotation + modifiedEuler);
				break;
			}
		case GUIZMO_TYPE::SCALE:
			{
				m_pSelectedGameObject->Transform()->SetRelativeScale(modifiedScale);
				break;
			}
		default: assert(nullptr && "INVALID GUIZMO TYPE");
		}
	}
}

bool SceneOutliner::MakePrefab()
{
	if (nullptr == m_pSelectedGameObject)
	{
		return false;
	}
	const std::wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	const std::wstring prefabPath  = L"prefab\\" + m_pSelectedGameObject->GetName() + L".pref";

	if (std::filesystem::exists(contentPath + prefabPath) && false == m_bOverwritePrefab)
	{
		return false;
	}

	if (nullptr != CResMgr::GetInst()->FindRes<CPrefab>(prefabPath))
	{
		CResMgr::GetInst()->ForceDeleteRes<CPrefab>(prefabPath);
		if (std::filesystem::exists(contentPath + prefabPath))
		{
			std::filesystem::remove(contentPath + prefabPath);
		}
	}
	m_bOverwritePrefab = false;

	Ptr<CPrefab> pPrefab = new CPrefab{};
	pPrefab->SetProto(m_pSelectedGameObject->Clone());
	CResMgr::GetInst()->AddRes<CPrefab>(prefabPath, pPrefab.Get(), false);

	return true;
}

void SceneOutliner::SelectPrefab(DWORD_PTR _param)
{
	string  strSelectedName = (char*)_param;
	auto    strPrefabKey    = wstring(strSelectedName.begin(), strSelectedName.end());
	wstring strContent      = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath        = strContent + strPrefabKey;

	auto pPrefab = new CPrefab;
	pPrefab->Load(FullPath);

	assert(pPrefab);

	if (m_pSelectedScene && m_pSelectedLayer)
	{
		// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
		CGameObject* NewObj = pPrefab->Instantiate();
		m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());
		SAFE_DELETE(pPrefab);
		// TReeUI 에 추가하기 위해서 Reset() 
		ResetTreeUI();
	}
}

void SceneOutliner::CreateGameObject()
{
	if (m_pSelectedLayer)
	{
		CScene* pCurScene   = CSceneMgr::GetInst()->GetCurScene();
		auto    pDefaultObj = new CGameObject;

		pDefaultObj->SetName(L"New_GameObject");
		pDefaultObj->AddComponent(new CTransform);
		pCurScene->AddObject(pDefaultObj, m_pSelectedLayer->GetLayerIdx());

		Reset();
	}
	else {}
}

void SceneOutliner::ObjectPicking()
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		return;
	}
	static std::vector<CGameObject*> vecPickObjects{};
	vecPickObjects.clear();

	static std::vector<std::pair<float, CGameObject*>> vecIntersect{};
	vecIntersect.clear();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer*     pLayer     = CSceneMgr::GetInst()->GetCurScene()->GetLayer(i);
		const auto& vecObjects = pLayer->GetObjects();
		for (auto& pGameObject : vecObjects)
		{
			CRenderComponent* pRenderComponent = pGameObject->GetRenderComponent();
			if (nullptr == pRenderComponent
			    || nullptr == pRenderComponent->GetMesh()
			    || nullptr == pRenderComponent->GetMaterial(0)
			    || nullptr == pRenderComponent->GetMaterial(0)->GetShader())
			{
				continue;
			}
			vecPickObjects.push_back(pGameObject);
		}
	}
	const tRay& cameraRay = CRenderMgr::GetInst()->GetMainCam()->GetRay();

	for (const auto& pGameObject : vecPickObjects)
	{
		CRenderComponent* pRenderComponent = pGameObject->GetRenderComponent();

		const Matrix& matInverseWorld = pGameObject->Transform()->GetWorldInvMat();

		Vec3 rayLocalPosForMesh = XMVector3TransformCoord(cameraRay.vStart, matInverseWorld);
		Vec3 rayLocalDirForMesh = XMVector3TransformNormal(cameraRay.vDir, matInverseWorld);
		rayLocalDirForMesh.Normalize();

		const Vtx* pVertexData      = pRenderComponent->GetMesh()->GetVtxSysMem();
		const int  indexBufferCount = pRenderComponent->GetMesh()->GetSubsetCount();

		std::vector<float> vecDistance{};
		for (int i = 0; i < indexBufferCount; ++i)
		{
			Ptr<CMesh>  pMesh         = pRenderComponent->GetMesh();
			const UINT* pIndexData    = static_cast<UINT*>(pMesh->GetIndexInfo(i).pIdxSysMem);
			const UINT  indexCount    = pMesh->GetIndexInfo(i).iIdxCount;
			const UINT  triangleCount = indexCount / 3;

			for (UINT j = 0; j < triangleCount; ++j)
			{
				// Indices for this triangle.
				const UINT index_0 = pIndexData[j * 3 + 0];
				const UINT index_1 = pIndexData[j * 3 + 1];
				const UINT index_2 = pIndexData[j * 3 + 2];

				// Vertices for this triangle.
				Vec3 vertex_0 = pVertexData[index_0].vPos;
				Vec3 vertex_1 = pVertexData[index_1].vPos;
				Vec3 vertex_2 = pVertexData[index_2].vPos;

				// We have to iterate over all the triangles in order to find the nearest intersection.

				float distance = 0.0f;
				if (TriangleTests::Intersects(rayLocalPosForMesh,
				                              rayLocalDirForMesh,
				                              vertex_0,
				                              vertex_1,
				                              vertex_2,
				                              distance))
				{
					vecDistance.push_back(distance);
				}
			}
		}
		std::sort(vecDistance.begin(), vecDistance.end());
		if (vecDistance.size() > 0)
		{
			vecIntersect.push_back({vecDistance[0], pGameObject});
		}
	}
	if (vecIntersect.size() > 0)
	{
		std::sort(vecIntersect.begin(),
		          vecIntersect.end(),
		          [](const auto& left, const auto& right)
		          {
			          return left.first < right.first;
		          });

		m_pSelectedGameObject   = vecIntersect[0].second;
		TreeNode* pSelectedNode = m_TreeUI->FindNode(ToString(m_pSelectedGameObject->GetName()));

		TreeNode* pParentNode = pSelectedNode->GetParent();
		while (pParentNode)
		{
			pParentNode->SetCheckOn();
			pParentNode = pParentNode->GetParent();
		}
		m_TreeUI->SetSelectedNode(pSelectedNode);
	}
}

TreeNode* SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode, ToString(_pObject->GetName()), (DWORD_PTR)_pObject);
	pNode->SetNodeType(NODE_TYPE::ENGINE_GAMEOBJECT);


	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(vecChild[i], pNode);
	}

	return pNode;
}

TreeNode* SceneOutliner::AddSceneToTree(CScene* _pScene, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode, "Scene " + ToString(_pScene->GetName()), (DWORD_PTR)_pScene);
	pNode->SetNodeType(NODE_TYPE::ENGINE_SCENE);

	return pNode;
}

TreeNode* SceneOutliner::AddLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	int       layerIdx = _pLayer->GetLayerIdx();
	TreeNode* pNode    = m_TreeUI->AddTreeNode(_pDestNode, "Layer " + ToString(_pLayer->GetName()), (DWORD_PTR)_pLayer);

	pNode->SetNodeType(NODE_TYPE::ENGINE_LAYER);
	return pNode;
}

void SceneOutliner::PressDelete(DWORD_PTR _dw)
{
	auto pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	auto pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI 를 찾아서 Object 를 nullptr 로 세팅한다.

	auto pInspectorUI = static_cast<InspectorUI*>(CImGuiMgr::GetInst()->FindUI("Inspector"));
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	if (_dwDrag == (DWORD_PTR)nullptr || _dwDrop == (DWORD_PTR)nullptr)
		return;


	NODE_TYPE eDragNode_Type = m_TreeUI->GetDragNode()->GetNodeType();
	NODE_TYPE eDropNode_Type = m_TreeUI->GetDropNode()->GetNodeType();

	// GAME_OBJECT -> GAME_OBJECT
	if (eDragNode_Type == NODE_TYPE::ENGINE_GAMEOBJECT &&
	    eDropNode_Type == NODE_TYPE::ENGINE_GAMEOBJECT)
	{
		auto pDrag = (CGameObject*)_dwDrag; // Child 
		auto pDrop = (CGameObject*)_dwDrop; // Parent

		if (pDrop != nullptr)
		{
			if (pDrag == pDrop || pDrop->IsAncestor(pDrag))
			{
				return;
			}

			CSceneMgr::GetInst()->AddChild(pDrop, pDrag);
		}
		// 자식 오브젝트의 목적지가 nullptr 인 경우
		else
		{
			// 이미 최상위 부모 오브젝트는 이벤트 처리가 필요없다.
			if (nullptr == pDrag->GetParent())
			{
				return;
			}

			CSceneMgr::GetInst()->DisconnectParent(pDrag);
		}
	}
	// GAME_OBJECT -> LAYER 
	else if (eDragNode_Type == NODE_TYPE::ENGINE_GAMEOBJECT &&
	         eDropNode_Type == NODE_TYPE::ENGINE_LAYER)
	{
		auto pDrag = (CGameObject*)_dwDrag; // Object 
		auto pDrop = (CLayer*)_dwDrop;		// Layer

		CSceneMgr::GetInst()->ChangeObjectLayerIndex(pDrag, pDrop->GetLayerIdx());
	}
	// LAYER -> LAYER 
	else if (eDragNode_Type == NODE_TYPE::ENGINE_LAYER &&
	         eDropNode_Type == NODE_TYPE::ENGINE_LAYER)
	{
		auto pDrag = (CLayer*)_dwDrag;		// Layer 
		auto pDrop = (CLayer*)_dwDrop;		// Layer

		CSceneMgr::GetInst()->SwapLayer(pDrag->GetLayerIdx(), pDrop->GetLayerIdx());
	}
}

void SceneOutliner::ResDrop(DWORD_PTR _resPtr)
{
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));
		}

		ImGui::EndDragDropTarget();
	}
}
