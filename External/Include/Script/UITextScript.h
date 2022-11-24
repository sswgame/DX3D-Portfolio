#pragma once
#include "CUIBase.h"

enum class TEXT_ALIGN_HORIZONTAL
{
	LEFT,
	MIDDLE,
	RIGHT
};

enum class TEXT_ALIGN_VERTICAL
{
	TOP,
	MIDDLE,
	BOTTOM
};

class UITextScript final
	: public CUIBase
{
private:
	CUIBase* m_pUIScript;

	std::string m_text;
	float       m_fontSize;
	int         m_textCapacity;

	ComPtr<IDWriteTextFormat>    m_pFont;
	ComPtr<IDWriteTextLayout>    m_pLayout;
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;
	ComPtr<ID2D1RenderTarget>    m_pRTV2D;
	std::string                  m_fontName;
	bool                         m_alphaEnable;
	Vec4                         m_color;

	TEXT_ALIGN_HORIZONTAL m_alignH;
	TEXT_ALIGN_VERTICAL   m_alignV;

private:
	void CreateTextLayout();
	void RenderText();
public:
	void start() override;
	void lateupdate() override;
public:
	void               AddText(const std::wstring& text);
	const std::string& GetText() const;
	void               SetText(const std::wstring& text);

	int  GetTextCount() const;
	void PopBack();
	void ClearText();

public:
	void  SetFontSize(float size);
	float GetFontSize() const { return m_fontSize; }

	void SetAlphaEnable(bool enable);
	void SetFont(const std::wstring& fontKey);

	void SetAlignH(TEXT_ALIGN_HORIZONTAL alignment);
	void SetAlignV(TEXT_ALIGN_VERTICAL alignment);

	void SetColor(UINT r, UINT g, UINT b);
	void SetColor(const Vec4& color);

public:
	CLONE(UITextScript);
	UITextScript();
	virtual ~UITextScript();
};
