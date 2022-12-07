#pragma once
#include "UI.h"

class CUIBase;
class CUIText;
class CUIProgressBar;
class CUIButton;
class CUIImage;
class CUIPanel;
class TreeNode;
class TreeUI;

enum class GAME_UI_TYPE
{
	PANEL,
	IMAGE,
	TEXT,

	END
};

enum class UI_EXTRA_TYPE
{
	BUTTON,
	PROGRESS_BAR,

	END
};

class UITool final
	: public UI
{
private:
	inline static UINT s_geneatedID = 0;
private:
	Ptr<CTexture> m_pFrameTexture = nullptr;
	CGameObject*  m_pSelected     = nullptr;

	bool   m_isDrawing = false;
	ImVec2 m_startPos;
	ImVec2 m_lastPos;

	ImVec2 m_size;
	ImVec2 m_leftTop;

	bool m_bHasDragArea   = false;
	bool m_bShowWarning   = false;
	bool m_bWrongPrefab   = false;
	bool m_bAlreadyHas    = false;
	bool m_bNoImageScript = false;
	bool m_bOpenPopUp     = false;
private:
	void DrawImage();
	void CropImageByDrag();
	void DrawInfo();
	void DrawWarning();
private:
	void ShowUIDefaultInfo(CUIBase* pScript);
	void ShowPanelInfo(CUIPanel* pScript);
	void ShowImageInfo(CUIImage* pScript);
	void ShowTextInfo(CUIText* pScript);
	void ShowButtonInfo(CUIButton* pScript);
	void ShowProgressBarInfo(CUIProgressBar* pScript);
private:
	void AddObjectDelegate(DWORD_PTR _pGameObject);
	void AddUIObject(GAME_UI_TYPE type);
	void AddExtraFeature(UI_EXTRA_TYPE type);

	void SelectFontName(DWORD_PTR _pFontName);
	void SelectTexture(DWORD_PTR _pTextureName);

	void CalculatePosAndSize();
public:
	void render_update() override;
public:
	UITool();
	virtual ~UITool();
};
