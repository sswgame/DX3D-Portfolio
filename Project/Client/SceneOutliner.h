#pragma once
#include "UI.h"

#include "TreeUI.h"

class CGameObject;
class CScene;
class CLayer;

class SceneOutliner : public UI
{
private:
	TreeUI*		m_TreeUI;
	NODE_TYPE	m_eClickedNodeType;

	CScene*      m_pSelectedScene;
	CLayer*      m_pSelectedLayer;
	CGameObject* m_pSelectedGameObject;

private:
	void RenderAddObject();
	void RenderAddLayer();

	void MakePrefab();
	void SelectPrefab(DWORD_PTR _param);
	void CreateGameObject();

public:
	TreeNode* AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode);
	TreeNode* AddSceneToTree(CScene* _pScene, TreeNode* _pDestNode);
	TreeNode* AddLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode);


public:
	virtual void update() override;
	virtual void render_update() override;

public:
	void Reset();
	void ResetTreeUI();
	void ObjectClicked(DWORD_PTR _dw);


	// Delegeate Funcs 
	void PressDelete(DWORD_PTR _dw);
	void DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop);
	void ResDrop(DWORD_PTR _resPtr);

public:
	SceneOutliner();
	~SceneOutliner();
};
