#pragma once
#include "singleton.h"

#include <dwrite_3.h>
#pragma comment(lib,"dwrite")

//enum class TEXT_ALIGN_HORIZONTAL
//{
//	LEFT,
//	CENTER,
//	RIGHT
//};
//
//enum class TEXT_ALIGN_VERTICAL
//{
//	TOP,
//	MIDDLE,
//	BOTTOM
//};

class CFontMgr : public CSingleton<CFontMgr>
{
	SINGLE(CFontMgr);
private:
	ComPtr<IDWriteFactory5>                                m_pFactory;
	std::map<std::wstring, ComPtr<IDWriteFontCollection1>> m_mapCollection;
	std::map<std::wstring, ComPtr<IDWriteTextFormat>>      m_mapFont;
	std::map<UINT, ComPtr<ID2D1SolidColorBrush>>           m_mapBrush;

private:
	void                           CreateFontCollection(const std::wstring& _key, const std::wstring& _relativePath);
	ComPtr<IDWriteFontCollection1> FindFontCollection(const std::wstring& _key);
	ComPtr<IDWriteTextFormat>      FindFont(const std::wstring& _key);
	std::wstring                   GetFontFaceName(const std::wstring& _key);

	ComPtr<IDWriteTextFormat> CreateFontFromFile(const std::wstring& _key,
	                                             const std::wstring& _fontFace,
	                                             DWRITE_FONT_WEIGHT  _fontWeight  = DWRITE_FONT_WEIGHT_NORMAL,
	                                             DWRITE_FONT_STYLE   _fontStyle   = DWRITE_FONT_STYLE_NORMAL,
	                                             DWRITE_FONT_STRETCH _fontStretch = DWRITE_FONT_STRETCH_NORMAL,
	                                             float               _fontSize    = 16.f,
	                                             const std::wstring& _locale      = L"en_us");

	ComPtr<ID2D1SolidColorBrush> FindColor(const Vec4& color);
	UINT                         GetColorID(const Vec4& color);
public:
	void                         Init();
	ComPtr<IDWriteTextFormat>    LoadFontFromFile(const std::wstring& _relativePath);
	ComPtr<ID2D1SolidColorBrush> GetBrush(const Vec4& color);
	ComPtr<ID2D1SolidColorBrush> GetBrush(const D2D1::ColorF& color);
};
