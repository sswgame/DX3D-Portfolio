#include "pch.h"
#include "CUIBase.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CCamera.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTransform.h>

#include "UIPanelScript.h"

CUIBase::CUIBase(SCRIPT_TYPE type, UI_TYPE eUIType)
	: CScript{static_cast<int>(type)}
	, m_pPanel{nullptr}
	, m_eUIType{eUIType}
	, m_orderZ{0}
	, m_opacity{0.f}
	, m_isMouseHovered{false}
	, m_mouseCollisionEnable{true}
	, m_anchorV{ANCHOR_VERTICAL::MIDDLE}
	, m_anchorH{ANCHOR_HORIZONTAL::MIDDLE}
	, m_showDebugRect{true}
{
}

CUIBase::~CUIBase() = default;


bool CUIBase::CollisionMouse()
{
	if (false == m_mouseCollisionEnable)
	{
		return false;
	}

	Vec2       mousePos   = CKeyMgr::GetInst()->GetMousePos();
	const Vec2 resolution = CDevice::GetInst()->GetRenderResolution();
	//마우스 자체가 화면 공간 밖에 있어도 안된다.
	const bool isMouseOutWindow = (0 <= mousePos.x && mousePos.x <= resolution.x
	                               && 0 <= mousePos.y && mousePos.y <= resolution.y);
	if (false == isMouseOutWindow)
	{
		return false;
	}
	//좌상단이 0,0이므로 DirectX와 좌표계를 맞추기 위한 보정.
	const Vec2 half = resolution * 0.5f;
	mousePos.x -= half.x;
	mousePos.y = (mousePos.y >= half.y) ? -mousePos.y + half.y : half.y - mousePos.y;

	const Vec3 uiPos      = Transform()->GetRelativePos();
	const Vec3 halfUISize = Transform()->GetRelativeScale() * 0.5f;

	const Vec2 leftTop     = {uiPos.x - halfUISize.x, uiPos.y + halfUISize.y};
	const Vec2 rightBottom = {uiPos.x + halfUISize.x, uiPos.y - halfUISize.y};

	const bool isInside = (leftTop.x < mousePos.x && mousePos.x < rightBottom.x
	                       && rightBottom.y < mousePos.y && mousePos.y < leftTop.y);

	return isInside;
}

void CUIBase::SetOrderZ(int iOrderZ)
{
	m_orderZ = iOrderZ;
	//Vec3 pos = Transform()->GetRelativePos();
	//Transform()->SetRelativePos(pos.x, pos.y, static_cast<float>(m_orderZ));
	if (m_pPanel)
	{
		m_pPanel->SetSorted(false);
	}
}

void CUIBase::ShowDebugRect(bool enable)
{
	m_showDebugRect = enable;
	if (m_showDebugRect)
	{
		MeshRender()->Activate();
	}
	else
	{
		MeshRender()->Deactivate();
	}
}

void CUIBase::FireCallback() const
{
	if (m_hoverCallback)
	{
		m_hoverCallback();
	}
}

void CUIBase::update()
{
	ShowDebugRect(m_showDebugRect);

	Vec3 adjustPos = Transform()->GetRelativePos();
	adjustPos.x    = AdjustPositionWithAnchor(m_anchorH);
	adjustPos.y    = AdjustPositionWithAnchor(m_anchorV);
	Transform()->SetRelativePos(adjustPos);

	if (nullptr == m_pPanel)
	{
		m_isMouseHovered = CollisionMouse();
		if (m_isMouseHovered && m_hoverCallback)
		{
			m_hoverCallback();
		}
	}
}

void CUIBase::lateupdate()
{
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, (int*)&m_eUIType);
	Ptr<CTexture> pTexture = MeshRender()->GetDynamicMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
	if (nullptr != pTexture)
	{
		Vec2 textureSize = {pTexture->Width(), pTexture->Height()};
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &textureSize);
	}
}

float CUIBase::AdjustPositionWithAnchor(ANCHOR_HORIZONTAL type)
{
	if (nullptr == GetOwner()->GetParent())
	{
		m_pPanel = nullptr;
	}
	else
	{
		Transform()->SetIgnoreParentScale(true);
	}
	if (nullptr == m_pPanel)
	{
		return AdjustWithCamera(type);
	}
	return AdjustWithPanel(type);
}

float CUIBase::AdjustPositionWithAnchor(ANCHOR_VERTICAL type)
{
	if (nullptr == GetOwner()->GetParent())
	{
		m_pPanel = nullptr;
	}
	else
	{
		Transform()->SetIgnoreParentScale(true);
	}
	if (nullptr == m_pPanel)
	{
		return AdjustWithCamera(type);
	}
	return AdjustWithPanel(type);
}

float CUIBase::AdjustWithCamera(ANCHOR_HORIZONTAL type) const
{
	CCamera* pCamera = CRenderMgr::GetInst()->GetUICamera();
	if (nullptr == pCamera)
	{
		return m_offsetPos.x;
	}
	const float halfWidth = pCamera->GetWidth() * 0.5f;
	const Vec3  cameraPos = pCamera->Transform()->GetWorldPos();


	switch (type)
	{
	case ANCHOR_HORIZONTAL::LEFT:
		return cameraPos.x - halfWidth + m_offsetPos.x;
	case ANCHOR_HORIZONTAL::MIDDLE:
		return cameraPos.x + m_offsetPos.x;
	case ANCHOR_HORIZONTAL::RIGHT:
		return cameraPos.x + halfWidth + m_offsetPos.x;
	}
	assert(nullptr && "INVALID POS");
	return m_offsetPos.x;
}

float CUIBase::AdjustWithCamera(ANCHOR_VERTICAL type) const
{
	CCamera* pCamera = CRenderMgr::GetInst()->GetUICamera();
	if (nullptr == pCamera)
	{
		return m_offsetPos.y;
	}
	const float halfHeight = (pCamera->GetWidth() / pCamera->GetAspectRatio()) * 0.5f;
	const Vec3  cameraPos  = pCamera->Transform()->GetWorldPos();
	switch (type)
	{
	case ANCHOR_VERTICAL::TOP:
		return cameraPos.y + halfHeight + m_offsetPos.y;
	case ANCHOR_VERTICAL::MIDDLE:
		return cameraPos.y + m_offsetPos.y;
	case ANCHOR_VERTICAL::BOTTOM:
		return cameraPos.y - halfHeight + m_offsetPos.y;
	}

	assert(nullptr && "INVALID POS");

	return m_offsetPos.y;
}

float CUIBase::AdjustWithPanel(ANCHOR_HORIZONTAL type) const
{
	//어차피 부모 윈도우 기준에서 자식 윈도우의 위치가 정해지므로(Transform에 의하여)
	//거기서 얼마나 떨어져야 하는지를 지정해주기만 하면 된다.
	const Vec3  panelScale = m_pPanel->Transform()->GetRelativeScale();
	const float halfWidth  = panelScale.x * 0.5f;

	switch (type)
	{
	case ANCHOR_HORIZONTAL::LEFT:
		return - halfWidth + m_offsetPos.x;
	case ANCHOR_HORIZONTAL::MIDDLE:
		return m_offsetPos.x;
	case ANCHOR_HORIZONTAL::RIGHT:
		return halfWidth + m_offsetPos.x;
	}
	assert(nullptr && "INVALID POS");
	return m_offsetPos.x;
}

float CUIBase::AdjustWithPanel(ANCHOR_VERTICAL type) const
{
	const Vec3  panelScale = m_pPanel->Transform()->GetRelativeScale();
	const float halfHeight = panelScale.y * 0.5f;

	switch (type)
	{
	case ANCHOR_VERTICAL::TOP:
		return halfHeight + m_offsetPos.y;
	case ANCHOR_VERTICAL::MIDDLE:
		return m_offsetPos.y;
	case ANCHOR_VERTICAL::BOTTOM:
		return -halfHeight + m_offsetPos.y;
	}
	assert(nullptr && "INVALID POS");
	return m_offsetPos.y;
}
