#include "pch.h"
#include "UITextScript.h"

#include <Engine/CFontMgr.h>

#include "CObjectManager.h"
#include "CUIBase.h"

UITextScript::UITextScript()
	: CUIBase{SCRIPT_TYPE::UITEXTSCRIPT, UI_TYPE::TEXT}
	, m_pUIScript{nullptr}
	, m_text{"EMPTY"}
	, m_fontSize{16.f}
	, m_textCapacity{0}
	, m_alphaEnable{false}
	, m_color{1.f, 1.f, 1.f, 1.f}
	, m_alignH{TEXT_ALIGN_HORIZONTAL::MIDDLE}
	, m_alignV{TEXT_ALIGN_VERTICAL::MIDDLE}
{
}

UITextScript::~UITextScript() = default;

void UITextScript::start()
{
	m_pRTV2D      = CDevice::GetInst()->GetRtv2D();
	m_pFont       = CFontMgr::GetInst()->LoadFontFromFile(L"font\\fa-solid-900.ttf");
	m_pColorBrush = CFontMgr::GetInst()->GetBrush(m_color);
	CreateTextLayout();
}

void UITextScript::lateupdate()
{
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

	switch (m_alignH)
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

	switch (m_alignV)
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
	const Vec3    worldPos       = Transform()->GetWorldPos();
	const Vec3    halfWorldScale = Transform()->GetWorldScale() * 0.5f;
	const Vec2    halfResolution = CDevice::GetInst()->GetRenderResolution() * 0.5f;
	D2D1_POINT_2F ptSize{};
	ptSize.x = worldPos.x + halfResolution.x - halfWorldScale.x;
	ptSize.y = worldPos.y + halfResolution.y - halfWorldScale.y;
	if (m_alphaEnable)
	{
		m_pColorBrush->SetOpacity(GetOpacity());
	}
	else
	{
		m_pColorBrush->SetOpacity(1.f);
	}

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
	m_alignH = alignment;
}

void UITextScript::SetAlignV(TEXT_ALIGN_VERTICAL alignment)
{
	m_alignV = alignment;
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
