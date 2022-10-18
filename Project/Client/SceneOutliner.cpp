#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"


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

SceneOutliner::~SceneOutliner()
{

}

void SceneOutliner::update()
{
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
	const ImVec2   windowSize = ImGui::GetContentRegionAvail();
	const ImGuiIO& io = ImGui::GetIO();
	const ImVec2   mousePos = io.MousePos - windowLeftTop;

	static bool bIsOpenPopUp = false;
	if (0 < mousePos.x && mousePos.x < windowSize.x
		&& 0 < mousePos.y && mousePos.y < windowSize.y)
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("PREFAB_POPUP");
			bIsOpenPopUp = true;
		}
	}

	if (bIsOpenPopUp)
	{
		static const std::string menus[] = {
			"Create"
		  , "Delete"
		  , "Add Component"
		  , "Delete Component"
		  , "Add Script"
		  , "Delete Script"
		  , "Make as Prefab"
		  , "Change Name"
		};
		if (ImGui::BeginPopup("PREFAB_POPUP"))
		{
			for (int i = 0; i < std::size(menus); ++i)
			{
				if (ImGui::Selectable(menus[i].c_str()))
				{

				}
			}
			ImGui::EndPopup();
		}
	}

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
	m_pSelectedScene = nullptr;
	m_pSelectedLayer = nullptr;
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
		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			// 3. GAMEOBJECT NODE
			AddGameObjectToTree(vecRoots[i], pLayerNode);
		}
	}
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string			strKey = pNode->GetName();
	NODE_TYPE		NodeType = pNode->GetNodeType();
	DWORD_PTR		pData = pNode->GetData();
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

		m_pSelectedScene = pScene;
		m_pSelectedLayer = nullptr;
		m_pSelectedGameObject = nullptr;

	}
	break;
	case NODE_TYPE::ENGINE_LAYER:
	{
		CLayer* pLayer = (CLayer*)pData;
		assert(pLayer);
		pInspectorUI->SetTargetLayer(pLayer);
		pInspectorUI->SetTargetObject(nullptr);

		m_pSelectedLayer = pLayer;
		m_pSelectedGameObject = nullptr;	}
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

TreeNode* SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);
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
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, "Scene " + string(_pScene->GetName().begin(), _pScene->GetName().end())
		, (DWORD_PTR)_pScene);
	pNode->SetNodeType(NODE_TYPE::ENGINE_SCENE);

	return pNode;

}

TreeNode* SceneOutliner::AddLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	int layerIdx = _pLayer->GetLayerIdx();
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, "Layer " + string(_pLayer->GetName().begin(), _pLayer->GetName().end())
		, (DWORD_PTR)_pLayer);

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
	CGameObject* pChildObject = (CGameObject*)_dwDrag;
	CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;


	// 드롭 목적지가 제대로 들어 온 경우
	if (nullptr != pDropTargetObject)
	{
		if (pChildObject == pDropTargetObject
			|| pDropTargetObject->IsAncestor(pChildObject))
		{
			return;
		}

		CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
	}

	// 자식 오브젝트의 목적지가 nullptr 인 경우
	else
	{
		// 이미 최상위 부모 오브젝트는 이벤트 처리가 필요없다.
		if (nullptr == pChildObject->GetParent())
		{
			return;
		}

		CSceneMgr::GetInst()->DisconnectParent(pChildObject);
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
