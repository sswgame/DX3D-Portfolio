#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>
#include <Engine/CTransform.h>

#include <Engine/CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "IconsFontAwesome5.h"
#include "CImGuiMgr.h"
#include "ListUI.h"


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
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutliner::ObjectClicked);

	// Drag and Drop Delegate 등록
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutliner::DragAndDropDelegate);

	// Key Delegate 등록
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutliner::PressDelete);


	Reset();
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
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
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
	TreeNode* pNode = (TreeNode*)_dw;

	string       strKey       = pNode->GetName();
	NODE_TYPE    NodeType     = pNode->GetNodeType();
	DWORD_PTR    pData        = pNode->GetData();
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	switch (NodeType)
	{
	case NODE_TYPE::ENGINE_SCENE:
		{
			CScene* pScene = (CScene*)pData;
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
			CLayer* pLayer = (CLayer*)pData;
			assert(pLayer);
			pInspectorUI->SetTargetLayer(pLayer);
			pInspectorUI->SetTargetObject(nullptr);

			m_pSelectedLayer      = pLayer;
			m_pSelectedGameObject = nullptr;
		}
		break;
	case NODE_TYPE::ENGINE_GAMEOBJECT:
		{
			CGameObject* pObj = (CGameObject*)pData;
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
			ListUI*                    pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Prefab List");

			for (const auto& pair : mapRes)
			{
				pListUI->AddList(ToString(pair.first));
			}

			pListUI->Activate();
			pListUI->SetDBCEvent(this, (DBCLKED)&::SceneOutliner::SelectPrefab);
		}


		static char buf[512];
		ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

		if (ImGui::Button("Complete"))
		{
			string  name    = buf;
			wstring newName = wstring(name.begin(), name.end());

			CGameObject* NewObj = new CGameObject;
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
	wstring strPrefabKey    = wstring(strSelectedName.begin(), strSelectedName.end());
	wstring strContent      = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath        = strContent + strPrefabKey;

	CPrefab* pPrefab = new CPrefab;
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
		CScene*      pCurScene   = CSceneMgr::GetInst()->GetCurScene();
		CGameObject* pDefaultObj = new CGameObject;

		pDefaultObj->SetName(L"New_GameObject");
		pDefaultObj->AddComponent(new CTransform);
		pCurScene->AddObject(pDefaultObj, m_pSelectedLayer->GetLayerIdx());

		Reset();
	}
	else { }
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
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI 를 찾아서 Object 를 nullptr 로 세팅한다.

	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
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
		CGameObject* pDrag = (CGameObject*)_dwDrag; // Child 
		CGameObject* pDrop = (CGameObject*)_dwDrop; // Parent

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
		CGameObject* pDrag = (CGameObject*)_dwDrag; // Object 
		CLayer*      pDrop = (CLayer*)_dwDrop;		// Layer

		CSceneMgr::GetInst()->ChangeObjectLayerIndex(pDrag, pDrop->GetLayerIdx());
	}
	// LAYER -> LAYER 
	else if (eDragNode_Type == NODE_TYPE::ENGINE_LAYER &&
	         eDropNode_Type == NODE_TYPE::ENGINE_LAYER)
	{
		CLayer* pDrag = (CLayer*)_dwDrag;		// Layer 
		CLayer* pDrop = (CLayer*)_dwDrop;		// Layer

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
