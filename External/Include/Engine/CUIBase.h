#pragma once
#include "CComponent.h"

class CUIPanel;
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

struct tImageInfo
{
	Vec2 imagePos;
	Vec2 imageSize;
};


class CUIBase
	: public CComponent
{
	friend class CUIPanel;
private:
	int                   m_orderZ;
	float                 m_opacity;
	bool                  m_isMouseHovered;
	bool                  m_mouseCollisionEnable;
	std::function<void()> m_hoverCallback;

	ANCHOR_VERTICAL   m_anchorV;
	ANCHOR_HORIZONTAL m_anchorH;

	Vec2 m_offsetPos;
	int  m_showDebugRect;

private:
	float AdjustWithCamera(ANCHOR_HORIZONTAL type) const;
	float AdjustWithCamera(ANCHOR_VERTICAL type) const;

	float AdjustWithPanel(ANCHOR_HORIZONTAL type) const;
	float AdjustWithPanel(ANCHOR_VERTICAL type) const;

	float AdjustPositionWithAnchor(ANCHOR_HORIZONTAL type);
	float AdjustPositionWithAnchor(ANCHOR_VERTICAL type);

public:
	virtual bool CollisionMouse();

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

	void SetShowDebugRect(bool enable);
	bool IsShowDebugRect() const { return m_showDebugRect; }

	template <typename T>
	void SetMouseHoverCallback(T* pInstance, void (T::*func)())
	{
		m_hoverCallback = std::bind(func, pInstance);
	}

	void FireCallback() const;

public:
	void finalupdate() override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CUIBase(COMPONENT_TYPE type);
	virtual ~CUIBase();
};
