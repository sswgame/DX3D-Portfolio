#pragma once
#include "CRenderComponent.h"

enum class UI_TYPE
{
	WIDGET
	, BILBOARD
	, WINDOW
};

enum class ANCHOR_HORIZONTAL
{
	LEFT
	, MIDDLE
	, RIGHT
	, END
};

enum class ANCHOR_VERTICAL
{
	TOP
	, MIDDLE
	, BOTTOM
	, END
};


class UIScript final
	: public CRenderComponent
{
private:
	int     m_orderZ;
	float   m_opacity;
	bool    m_isMouseHovered;
	bool    m_mouseCollisionEnable;
	UI_TYPE m_eUIType;

	ANCHOR_VERTICAL   m_anchorV;
	ANCHOR_HORIZONTAL m_anchorH;

	std::vector<CGameObject*> m_vecUIChild;
	bool                      m_isSorted;

	Vec2 m_offsetPos;

private:
	bool  CollisionMouse();
	float AdjustPositionWithAnchor(ANCHOR_HORIZONTAL type, CCamera* pCamera);
	float AdjustPositionWithAnchor(ANCHOR_VERTICAL type, CCamera* pCamera);

public:
	void SetOffsetPos(const Vec2 offsetPos) { m_offsetPos = offsetPos; }
	Vec2 GetOffsetPos() const { return m_offsetPos; }

	void SetOrderZ(int iOrderZ) { m_orderZ = iOrderZ; }
	int  GetOrderZ() const { return m_orderZ; }

	void  SetOpacity(float fOpacity) { m_opacity = fOpacity; }
	float GetOpacity() const { return m_opacity; }

	void SetMouseCollision(bool enable) { m_mouseCollisionEnable = enable; }
	bool GetMouseCollision() const { return m_mouseCollisionEnable; }

	void              SetAnchorH(ANCHOR_HORIZONTAL type) { m_anchorH = type; }
	ANCHOR_HORIZONTAL GetAnchorH() const { return m_anchorH; }

	void            SetAnchorV(ANCHOR_VERTICAL type) { m_anchorV = type; }
	ANCHOR_VERTICAL GetAnchorV() const { return m_anchorV; }


	void    SetUIType(UI_TYPE type);
	UI_TYPE GetUIType() const { return m_eUIType; }

	bool MouseHovered() const { return m_isMouseHovered; }
public:
	void update() override;
	void finalupdate() override;
	void render() override;
	CLONE(UIScript);
public:
	UIScript();
	virtual ~UIScript();
};
