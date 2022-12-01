#include "pch.h"
#include "CUIBase.h"

#include <Engine/CDevice.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CCamera.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTransform.h>

#include "UIPanelScript.h"

CUIBase::CUIBase(SCRIPT_TYPE type)
	: CScript{static_cast<int>(type)}
	, m_pPanel{nullptr}
	, m_orderZ{0}
	, m_opacity{1.f}
	, m_isMouseHovered{false}
	, m_mouseCollisionEnable{true}
	, m_anchorV{ANCHOR_VERTICAL::MIDDLE}
	, m_anchorH{ANCHOR_HORIZONTAL::MIDDLE}
	, m_showDebugRect{true}
{
	AddScriptParamAsCheckBox("SHOW RANGE",
	                         &m_showDebugRect,
	                         [this]()
	                         {
		                         if (this->MeshRender())
			                         this->ShowDebugRect(m_showDebugRect);
	                         });
	AddScriptParamAsCheckBox("MOUSE COLLISION", (int*)&m_mouseCollisionEnable);
	AddScriptParam("OFFSET", SCRIPTPARAM_TYPE::VEC2, &m_offsetPos);
	AddScriptParam("Z ORDER", SCRIPTPARAM_TYPE::INT, &m_orderZ, [this]() { this->SetOrderZ(m_orderZ); });
	AddScriptParam("OPACITY", SCRIPTPARAM_TYPE::FLOAT, &m_opacity, [this]() { this->SetOpacity(m_opacity); });
	AddScriptParamAsDropDown("ALIGNMENT(HORIZONTAL)",
	                         SCRIPTPARAM_TYPE::INT,
	                         &m_anchorH,
	                         {"LEFT", "MIDDLE", "RIGHT"});
	AddScriptParamAsDropDown("ALIGNMENT(VERTICAL)",
	                         SCRIPTPARAM_TYPE::INT,
	                         &m_anchorV,
	                         {"TOP", "MIDDLE", "BOTTOM"});
}

CUIBase::CUIBase(const CUIBase& _origin)
	: CScript{_origin}
	, m_pPanel{nullptr}
	, m_orderZ{_origin.m_orderZ}
	, m_opacity{_origin.m_opacity}
	, m_isMouseHovered{false}
	, m_mouseCollisionEnable{_origin.m_mouseCollisionEnable}
	, m_anchorV{_origin.m_anchorV}
	, m_anchorH{_origin.m_anchorH}
	, m_showDebugRect{_origin.m_showDebugRect}
{
	RenewScalarParam("SHOW RANGE",
	                 &m_showDebugRect,
	                 [this]()
	                 {
		                 if (this->MeshRender())
			                 this->ShowDebugRect(m_showDebugRect);
	                 });
	RenewScalarParam("MOUSE COLLISION", &m_mouseCollisionEnable);
	RenewScalarParam("OFFSET", &m_offsetPos);
	RenewScalarParam("Z ORDER", &m_orderZ, [this]() { this->SetOrderZ(m_orderZ); });
	RenewScalarParam("OPACITY", &m_opacity, [this]() { this->SetOpacity(m_opacity); });
	RenewScalarParam("ALIGNMENT(HORIZONTAL)", &m_anchorH);
	RenewScalarParam("ALIGNMENT(VERTICAL)", &m_anchorV);
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

	const Vec3 uiPos      = Transform()->GetWorldPos();
	const Vec3 halfUISize = Transform()->GetWorldScale() * 0.5f;

	const Vec2 leftTop     = {uiPos.x - halfUISize.x, uiPos.y + halfUISize.y};
	const Vec2 rightBottom = {uiPos.x + halfUISize.x, uiPos.y - halfUISize.y};

	const bool isInside = (leftTop.x < mousePos.x && mousePos.x < rightBottom.x
	                       && rightBottom.y < mousePos.y && mousePos.y < leftTop.y);

	return isInside;
}

void CUIBase::SetOrderZ(int iOrderZ)
{
	m_orderZ = iOrderZ;

	if (m_pPanel)
	{
		m_pPanel->SetSorted(false);
	}
}

void CUIBase::ShowDebugRect(bool enable)
{
	m_showDebugRect = enable;
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
	m_opacity = ClampData(m_opacity, 0.f, 1.f);

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
	if (MeshRender() && false == MeshRender()->IsUsingDynamicMaterial(0))
	{
		MeshRender()->GetDynamicMaterial(0);
	}

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_showDebugRect);

	Ptr<CTexture> pTexture = MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
	if (nullptr != pTexture)
	{
		Vec2 textureSize = {pTexture->Width(), pTexture->Height()};
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &textureSize);
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
	const float halfWidth         = pCamera->GetWidth() * 0.5f;
	const Vec3  cameraPos         = pCamera->Transform()->GetWorldPos();
	const Vec3  widgetUIHalfScale = GetOwner()->Transform()->GetRelativeScale() * 0.5f;

	switch (type)
	{
	case ANCHOR_HORIZONTAL::LEFT:
		return cameraPos.x - halfWidth + m_offsetPos.x + widgetUIHalfScale.x;
	case ANCHOR_HORIZONTAL::MIDDLE:
		return cameraPos.x + m_offsetPos.x;
	case ANCHOR_HORIZONTAL::RIGHT:
		return cameraPos.x + halfWidth + m_offsetPos.x - widgetUIHalfScale.x;
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
	const float halfHeight        = (pCamera->GetWidth() / pCamera->GetAspectRatio()) * 0.5f;
	const Vec3  cameraPos         = pCamera->Transform()->GetWorldPos();
	const Vec3  widgetUIHalfScale = GetOwner()->Transform()->GetRelativeScale() * 0.5f;

	switch (type)
	{
	case ANCHOR_VERTICAL::TOP:
		return cameraPos.y + halfHeight + m_offsetPos.y - widgetUIHalfScale.y;
	case ANCHOR_VERTICAL::MIDDLE:
		return cameraPos.y + m_offsetPos.y;
	case ANCHOR_VERTICAL::BOTTOM:
		return cameraPos.y - halfHeight + m_offsetPos.y + widgetUIHalfScale.y;
	}

	assert(nullptr && "INVALID POS");

	return m_offsetPos.y;
}

float CUIBase::AdjustWithPanel(ANCHOR_HORIZONTAL type) const
{
	//어차피 부모 윈도우 기준에서 자식 윈도우의 위치가 정해지므로(Transform에 의하여)
	//거기서 얼마나 떨어져야 하는지를 지정해주기만 하면 된다.
	const Vec3  panelScale        = m_pPanel->Transform()->GetRelativeScale();
	const float panelHalfWidth    = panelScale.x * 0.5f;
	const Vec3  widgetUIHalfScale = GetOwner()->Transform()->GetRelativeScale() * 0.5f;
	switch (type)
	{
	case ANCHOR_HORIZONTAL::LEFT:
		return (-panelHalfWidth) + m_offsetPos.x + widgetUIHalfScale.x;
	case ANCHOR_HORIZONTAL::MIDDLE:
		return m_offsetPos.x;
	case ANCHOR_HORIZONTAL::RIGHT:
		return panelHalfWidth + m_offsetPos.x - widgetUIHalfScale.x;
	}
	assert(nullptr && "INVALID POS");
	return m_offsetPos.x;
}

float CUIBase::AdjustWithPanel(ANCHOR_VERTICAL type) const
{
	const Vec3  panelScale        = m_pPanel->Transform()->GetRelativeScale();
	const float halfHeight        = panelScale.y * 0.5f;
	const Vec3  widgetUIHalfScale = GetOwner()->Transform()->GetRelativeScale() * 0.5f;

	switch (type)
	{
	case ANCHOR_VERTICAL::TOP:
		return halfHeight + m_offsetPos.y - widgetUIHalfScale.y;
	case ANCHOR_VERTICAL::MIDDLE:
		return m_offsetPos.y;
	case ANCHOR_VERTICAL::BOTTOM:
		return (-halfHeight) + m_offsetPos.y + widgetUIHalfScale.y;
	}
	assert(nullptr && "INVALID POS");
	return m_offsetPos.y;
}

void CUIBase::Serialize(YAML::Emitter& emitter)
{
	CScript::Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_orderZ) << YAML::Value << m_orderZ;
	emitter << YAML::Key << NAME_OF(m_opacity) << YAML::Value << m_opacity;
	emitter << YAML::Key << NAME_OF(m_mouseCollisionEnable) << YAML::Value << m_mouseCollisionEnable;
	emitter << YAML::Key << NAME_OF(m_anchorH) << YAML::Value << static_cast<int>(m_anchorH);
	emitter << YAML::Key << NAME_OF(m_anchorV) << YAML::Value << static_cast<int>(m_anchorV);
	emitter << YAML::Key << NAME_OF(m_offsetPos) << YAML::Value << m_offsetPos;
}

void CUIBase::Deserialize(const YAML::Node& node)
{
	CScript::Deserialize(node);
	m_orderZ               = node[NAME_OF(m_orderZ)].as<int>();
	m_opacity              = node[NAME_OF(m_opacity)].as<float>();
	m_mouseCollisionEnable = node[NAME_OF(m_mouseCollisionEnable)].as<bool>();
	m_anchorH              = static_cast<ANCHOR_HORIZONTAL>(node[NAME_OF(m_anchorH)].as<int>());
	m_anchorV              = static_cast<ANCHOR_VERTICAL>(node[NAME_OF(m_anchorV)].as<int>());
	m_offsetPos            = node[NAME_OF(m_offsetPos)].as<Vec2>();
}
