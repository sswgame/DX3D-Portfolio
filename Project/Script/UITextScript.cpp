#include "pch.h"
#include "UITextScript.h"

#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

#include "CObjectManager.h"
#include "CUIBase.h"


UITextScript::UITextScript()
	: CUIBase{SCRIPT_TYPE::UITEXTSCRIPT}
	, m_pUIScript{nullptr}
	, m_text{"EMPTY"}
	, m_fontSize{16.f}
	, m_alphaEnable{false}
	, m_color{1.f, 1.f, 1.f, 1.f}
	, m_alignTextH{TEXT_ALIGN_HORIZONTAL::MIDDLE}
	, m_alignTextV{TEXT_ALIGN_VERTICAL::MIDDLE}
{
	AddScriptParamAsCheckBox("TEXT ALPHA ENABLE", (int*)&m_alphaEnable);
	AddScriptParamAsColorPicker("COLOR", &m_color, [this]() { SetColor(m_color); });
	AddScriptParamAsDropDown("TEXT ALIGN(HORIZONTAL)",
	                         SCRIPTPARAM_TYPE::INT,
	                         &m_alignTextH,
	                         {"LEFT", "MIDDLE", "RIGHT"});
	AddScriptParamAsDropDown("TEXT ALIGN(VERTICAL)", SCRIPTPARAM_TYPE::INT, &m_alignTextV, {"TOP", "MIDDLE", "BOTTOM"});
	AddScriptParam("TEXT", SCRIPTPARAM_TYPE::TEXT, &m_text, [this]() { SetText(ToWString(m_text)); });
}

UITextScript::UITextScript(const UITextScript& _origin)
	: CUIBase{_origin}
	, m_pUIScript{nullptr}
	, m_text{_origin.m_text}
	, m_fontSize{_origin.m_fontSize}
	, m_alphaEnable{_origin.m_alphaEnable}
	, m_color{_origin.m_color}
	, m_alignTextH{_origin.m_alignTextH}
	, m_alignTextV{_origin.m_alignTextV}
{
	RenewScalarParam("TEXT ALPHA ENABLE", &m_alphaEnable);
	RenewScalarParam("COLOR", &m_color, [this]() { SetColor(m_color); });
	RenewScalarParam("TEXT ALIGN(HORIZONTAL)", &m_alignTextH);
	RenewScalarParam("TEXT ALIGN(VERTICAL)", &m_alignTextV);
	RenewScalarParam("TEXT", &m_text, [this]() { SetText(ToWString(m_text)); });
}

UITextScript::~UITextScript() = default;

void UITextScript::start()
{
	CUIBase::start();
	m_pRTV2D      = CDevice::GetInst()->GetRtv2D();
	m_pFont       = CFontMgr::GetInst()->LoadFontFromFile(L"font\\fa-solid-900.ttf");
	m_pColorBrush = CFontMgr::GetInst()->GetBrush(m_color);

	CreateTextLayout();
}

void UITextScript::lateupdate()
{
	if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial(0))
	{
		return;
	}
	CUIBase::lateupdate();
	CObjectManager::GetInst()->AddScriptEvent(this, &UITextScript::RenderText);
}

void UITextScript::CreateTextLayout()
{
	if (nullptr == m_pFont)
	{
		return;
	}
	Vec3 size{};
	size.x = m_text.size() * m_fontSize;
	size.y = m_fontSize;
	Transform()->SetRelativeScale(size);
	m_pLayout = CFontMgr::GetInst()->CreateTextLayout(ToWString(m_text), m_pFont, size.x, size.y);

	DWRITE_TEXT_RANGE range{};
	range.startPosition = 0;
	range.length        = (UINT)m_text.size();

	m_pLayout->SetFontSize(m_fontSize, range);

	switch (m_alignTextH)
	{
	case TEXT_ALIGN_HORIZONTAL::LEFT:
		m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TEXT_ALIGN_HORIZONTAL::MIDDLE:
		m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_HORIZONTAL::RIGHT:
		m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch (m_alignTextV)
	{
	case TEXT_ALIGN_VERTICAL::TOP:
		m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case TEXT_ALIGN_VERTICAL::MIDDLE:
		m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_VERTICAL::BOTTOM:
		m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	}
}

void UITextScript::RenderText()
{
	m_pRTV2D->BeginDraw();
	const Vec3 worldPos       = Transform()->GetWorldPos();
	const Vec3 halfWorldScale = Transform()->GetWorldScale() * 0.5f;
	const Vec2 halfResolution = CDevice::GetInst()->GetRenderResolution() * 0.5f;

	if (m_alphaEnable)
	{
		m_pColorBrush->SetOpacity(GetOpacity());
	}
	else
	{
		m_pColorBrush->SetOpacity(1.f);
	}

	//화면의 좌표계에 그려야 한다.
	D2D1_POINT_2F ptSize{};
	const float   yOffset = worldPos.y > 0 ? -fabs(worldPos.y) : fabs(worldPos.y);
	ptSize.x              = worldPos.x + halfResolution.x - halfWorldScale.x;
	ptSize.y              = yOffset + halfResolution.y - halfWorldScale.y;

	m_pRTV2D->DrawTextLayout(ptSize, m_pLayout.Get(), m_pColorBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

	m_pRTV2D->EndDraw();
}

void UITextScript::AddText(const std::wstring& text)
{
	m_text += ToString(text);
	CreateTextLayout();
}

const std::string& UITextScript::GetText() const
{
	return m_text;
}

void UITextScript::SetText(const std::wstring& text)
{
	m_text = ToString(text);
	CreateTextLayout();
}

int UITextScript::GetTextCount() const
{
	return static_cast<int>(m_text.size());
}

void UITextScript::PopBack()
{
	if (false == m_text.empty())
	{
		m_text.pop_back();
		CreateTextLayout();
	}
}

void UITextScript::ClearText()
{
	m_text.clear();
	CreateTextLayout();
}

void UITextScript::SetFontSize(float size)
{
	m_fontSize = size;
	CreateTextLayout();
}

void UITextScript::SetAlphaEnable(bool enable)
{
	m_alphaEnable = enable;
}

void UITextScript::SetFont(const std::wstring& fontKey)
{
	m_pFont = CFontMgr::GetInst()->LoadFontFromFile(fontKey);
	CreateTextLayout();
}

void UITextScript::SetAlignH(TEXT_ALIGN_HORIZONTAL alignment)
{
	m_alignTextH = alignment;
}

void UITextScript::SetAlignV(TEXT_ALIGN_VERTICAL alignment)
{
	m_alignTextV = alignment;
}

void UITextScript::SetColor(UINT r, UINT g, UINT b)
{
	m_color.x = r / 255.f;
	m_color.y = g / 255.f;
	m_color.z = b / 255.f;

	m_pColorBrush = CFontMgr::GetInst()->GetBrush(m_color);
}

void UITextScript::SetColor(const Vec4& color)
{
	m_color       = color;
	m_pColorBrush = CFontMgr::GetInst()->GetBrush(m_color);
}

void UITextScript::Serialize(YAML::Emitter& emitter)
{
	CUIBase::Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_text) << YAML::Value << m_text;
	emitter << YAML::Key << NAME_OF(m_fontSize) << YAML::Value << m_fontSize;
	emitter << YAML::Key << NAME_OF(m_fontName) << YAML::Value << m_fontName;
	emitter << YAML::Key << NAME_OF(m_alphaEnable) << YAML::Value << m_alphaEnable;
	emitter << YAML::Key << NAME_OF(m_color) << YAML::Value << m_color;
	emitter << YAML::Key << NAME_OF(m_alignTextH) << YAML::Value << static_cast<int>(m_alignTextH);
	emitter << YAML::Key << NAME_OF(m_alignTextV) << YAML::Value << static_cast<int>(m_alignTextV);
}

void UITextScript::Deserialize(const YAML::Node& node)
{
	CUIBase::Deserialize(node);
	m_text        = node[NAME_OF(m_text)].as<std::string>();
	m_fontSize    = node[NAME_OF(m_fontSize)].as<float>();
	m_fontName    = node[NAME_OF(m_fontName)].as<std::string>();
	m_alphaEnable = node[NAME_OF(m_alphaEnable)].as<bool>();
	m_color       = node[NAME_OF(m_color)].as<Vec4>();
	m_alignTextH  = static_cast<TEXT_ALIGN_HORIZONTAL>(node[NAME_OF(m_alignTextH)].as<int>());
	m_alignTextV  = static_cast<TEXT_ALIGN_VERTICAL>(node[NAME_OF(m_alignTextV)].as<int>());

	RenewScalarParam("TEXT", &m_text, [this]() { SetText(ToWString(m_text)); });
}
