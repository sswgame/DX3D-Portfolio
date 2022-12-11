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

class CUIText final
	: public CUIBase
{
private:
	std::string m_text;
	float       m_fontSize;

	ComPtr<IDWriteTextFormat>    m_pFont;
	ComPtr<IDWriteTextLayout>    m_pLayout;
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;

	std::string m_fontName;
	bool        m_alphaEnable;
	Vec4        m_color;

	TEXT_ALIGN_HORIZONTAL m_alignTextH;
	TEXT_ALIGN_VERTICAL   m_alignTextV;

private:
	void CreateTextLayout();
	void RenderText();
public:
	void start() override;
	void finalupdate() override;
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
	bool IsAlphaEnable() const { return m_alphaEnable; }

	void               SetFont(const std::wstring& fontKey);
	const std::string& GetFont() const { return m_fontName; }

	void                  SetAlignH(TEXT_ALIGN_HORIZONTAL alignment);
	TEXT_ALIGN_HORIZONTAL GetAlignH() const { return m_alignTextH; }

	void                SetAlignV(TEXT_ALIGN_VERTICAL alignment);
	TEXT_ALIGN_VERTICAL GetAlignV() const { return m_alignTextV; }

	void SetColor(UINT r, UINT g, UINT b);
	void SetColor(const Vec4& color);
	Vec4 GetColor() const { return m_color; }

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CLONE(CUIText);
	CUIText();
	CUIText(const CUIText& _origin);
	virtual ~CUIText();

	friend class CEventMgr;
};
