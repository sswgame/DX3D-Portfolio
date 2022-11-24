#pragma once
#include <Engine/CScript.h>
class UIPanelScript;
enum class SCRIPT_TYPE;

enum class ANCHOR_HORIZONTAL
{
	LEFT,
	MIDDLE,
	RIGHT,
	END
};

enum class ANCHOR_VERTICAL
{
	TOP,
	MIDDLE,
	BOTTOM,
	END
};

enum class UI_TYPE
{
	BUTTON,
	PROGRESS_BAR,
	PANEL,
	TEXT,
	IMAGE,

	END
};

class CUIBase
	: public CScript
{
	friend class UIPanelScript;
private:
	UIPanelScript* m_pPanel;

	const UI_TYPE         m_eUIType;
	int                   m_orderZ;
	float                 m_opacity;
	bool                  m_isMouseHovered;
	bool                  m_mouseCollisionEnable;
	std::function<void()> m_hoverCallback;

	ANCHOR_VERTICAL   m_anchorV;
	ANCHOR_HORIZONTAL m_anchorH;

	Vec2 m_offsetPos;
	bool m_showDebugRect;
private:
	float AdjustWithCamera(ANCHOR_HORIZONTAL type) const;
	float AdjustWithCamera(ANCHOR_VERTICAL type) const;

	float AdjustWithPanel(ANCHOR_HORIZONTAL type) const;
	float AdjustWithPanel(ANCHOR_VERTICAL type) const;

	float AdjustPositionWithAnchor(ANCHOR_HORIZONTAL type);
	float AdjustPositionWithAnchor(ANCHOR_VERTICAL type);

public:
	virtual bool CollisionMouse();
public:
	void           SetPanel(UIPanelScript* pPanel) { m_pPanel = pPanel; }
	UIPanelScript* GetPanel() const { return m_pPanel; }

	void SetOffsetPos(const Vec2 offsetPos) { m_offsetPos = offsetPos; }
	Vec2 GetOffsetPos() const { return m_offsetPos; }

	void SetOrderZ(int iOrderZ);
	int  GetOrderZ() const { return m_orderZ; }

	void  SetOpacity(float fOpacity) { m_opacity = fOpacity; }
	float GetOpacity() const { return m_opacity; }

	void SetMouseCollision(bool enable) { m_mouseCollisionEnable = enable; }
	bool GetMouseCollision() const { return m_mouseCollisionEnable; }

	void              SetAnchorH(ANCHOR_HORIZONTAL type) { m_anchorH = type; }
	ANCHOR_HORIZONTAL GetAnchorH() const { return m_anchorH; }

	void            SetAnchorV(ANCHOR_VERTICAL type) { m_anchorV = type; }
	ANCHOR_VERTICAL GetAnchorV() const { return m_anchorV; }

	bool IsMouseHovered() const { return m_isMouseHovered; }

	void ShowDebugRect(bool enable);

	template <typename T>
	void SetMouseHoverCallback(T* pInstance, void (T::*func)())
	{
		m_hoverCallback = std::bind(func, pInstance);
	}

	void FireCallback() const;

public:
	void update() override;
	void lateupdate() override;
public:
	CUIBase(SCRIPT_TYPE type, UI_TYPE eUIType);
	virtual ~CUIBase();
};
