#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr() {}

CFontMgr::~CFontMgr() {}

void CFontMgr::Init()
{
	const HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
	                                       __uuidof(m_pFactory),
	                                       (IUnknown**)m_pFactory.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"dwrite 초기화 실패", L"Error", MB_OK);
		assert(nullptr);
	}
}

ComPtr<IDWriteTextFormat> CFontMgr::LoadFontFromFile(const std::wstring& _relativePath)
{
	CreateFontCollection(_relativePath, _relativePath);
	const std::wstring fontFace = GetFontFaceName(_relativePath);
	assert(fontFace.empty() == false);
	return CreateFontFromFile(_relativePath, fontFace);
}

std::vector<std::wstring> CFontMgr::GetFonts() const
{
	std::vector<std::wstring> vecFont{};
	vecFont.reserve(m_mapFont.size());

	for (auto& [fontName,pFont] : m_mapFont)
	{
		vecFont.push_back(fontName);
	}

	return vecFont;
}

void CFontMgr::CreateFontCollection(const std::wstring& _key, const std::wstring& _relativePath)
{
	auto pFontCollection = FindFontCollection(_key);
	if (nullptr != pFontCollection)
	{
		return;
	}

	const std::wstring      fullPath = CPathMgr::GetInst()->GetContentPath() + _relativePath;
	ComPtr<IDWriteFontFile> pFontFile{};

	if (FAILED(m_pFactory->CreateFontFileReference(fullPath.c_str(),nullptr,pFontFile.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 파일 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}

	ComPtr<IDWriteFontSetBuilder1> pBuilder{};
	if (FAILED(m_pFactory->CreateFontSetBuilder(pBuilder.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 파일 셋 빌더 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	pBuilder->AddFontFile(pFontFile.Get());

	ComPtr<IDWriteFontSet> pFontSet{};
	if (FAILED(pBuilder->CreateFontSet(pFontSet.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 파일 셋 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}

	if (FAILED(m_pFactory->CreateFontCollectionFromFontSet(pFontSet.Get(),pFontCollection.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 콜렉션 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	m_mapCollection.insert({_key, pFontCollection});
}

ComPtr<IDWriteFontCollection1> CFontMgr::FindFontCollection(const std::wstring& _key)
{
	const auto iter = m_mapCollection.find(_key);
	if (iter != m_mapCollection.end())
	{
		return iter->second;
	}

	return nullptr;
}

ComPtr<IDWriteTextFormat> CFontMgr::FindFont(const std::wstring& _key)
{
	const auto iter = m_mapFont.find(_key);
	if (iter != m_mapFont.end())
	{
		return iter->second;
	}
	return nullptr;
}

std::wstring CFontMgr::GetFontFaceName(const std::wstring& _key)
{
	auto pFontCollection = FindFontCollection(_key);
	if (nullptr == pFontCollection)
	{
		return L"";
	}

	ComPtr<IDWriteFontFamily> pFamily{};
	if (FAILED(pFontCollection->GetFontFamily(0,pFamily.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 패밀리 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	ComPtr<IDWriteLocalizedStrings> pLocalizedName{};
	if (FAILED(pFamily->GetFamilyNames(pLocalizedName.GetAddressOf())))
	{
		MessageBox(nullptr, L"언어에 맞는 폰트 패밀리 추출 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	WCHAR szName[256]{};
	if (FAILED(pLocalizedName->GetString(0,szName,ARRAYSIZE(szName))))
	{
		MessageBox(nullptr, L"언어에 맞는 이름 추출 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	return szName;
}

ComPtr<IDWriteTextFormat> CFontMgr::CreateFontFromFile(const std::wstring& _key,
                                                       const std::wstring& _fontFace,
                                                       DWRITE_FONT_WEIGHT  _fontWeight,
                                                       DWRITE_FONT_STYLE   _fontStyle,
                                                       DWRITE_FONT_STRETCH _fontStretch,
                                                       float               _fontSize,
                                                       const std::wstring& _locale)
{
	ComPtr<IDWriteTextFormat> pFont = FindFont(_key);

	if (pFont)
	{
		return pFont;
	}

	if (FAILED(m_pFactory->CreateTextFormat(_fontFace.c_str(), nullptr, _fontWeight,_fontStyle, _fontStretch, _fontSize,
		           _locale.c_str(), pFont.GetAddressOf())))
	{
		MessageBox(nullptr, L"폰트 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}

	m_mapFont.insert({_key, pFont});

	return pFont;
}


ComPtr<ID2D1SolidColorBrush> CFontMgr::GetBrush(const Vec4& _color)
{
	ComPtr<ID2D1SolidColorBrush> pBrush = FindColor(_color);
	if (nullptr != pBrush)
	{
		return pBrush;
	}

	if (FAILED(CDevice::GetInst()->GetRtv2D()->CreateSolidColorBrush(D2D1::ColorF{_color.x,_color.y,_color.z,_color.w},
		           pBrush.GetAddressOf())))
	{
		MessageBox(nullptr, L"브러시 생성 실패", L"Error",MB_OK);
		assert(nullptr);
	}
	m_mapBrush.insert({GetColorID(_color), pBrush});

	return pBrush;
}

ComPtr<ID2D1SolidColorBrush> CFontMgr::GetBrush(const D2D1::ColorF& _color)
{
	return GetBrush(Vec4{_color.r, _color.g, _color.b, _color.a});
}

ComPtr<IDWriteTextLayout> CFontMgr::CreateTextLayout(const std::wstring&       text,
                                                     ComPtr<IDWriteTextFormat> pFont,
                                                     float                     width,
                                                     float                     height)
{
	ComPtr<IDWriteTextLayout> pLayout{};
	const HRESULT             hr = m_pFactory->CreateTextLayout(text.c_str(),
	                                                            (uint32_t)text.size(),
	                                                            pFont.Get(),
	                                                            width,
	                                                            height,
	                                                            pLayout.GetAddressOf());
	if (FAILED(hr))
	{
		return nullptr;
	}

	return pLayout;
}


ComPtr<ID2D1SolidColorBrush> CFontMgr::FindColor(const Vec4& _color)
{
	const auto iter = m_mapBrush.find(GetColorID(_color));
	if (iter != m_mapBrush.end())
	{
		return iter->second;
	}
	return nullptr;
}

UINT CFontMgr::GetColorID(const Vec4& _color)
{
	UINT  colorID{};
	UCHAR R{}, G{}, B{}, A{};

	R = static_cast<UCHAR>(_color.x * 255);
	G = static_cast<UCHAR>(_color.y * 255);
	B = static_cast<UCHAR>(_color.z * 255);
	A = static_cast<UCHAR>(_color.w * 255);

	colorID = (R << 24) | (G << 16) | (B << 8) | A;

	return colorID;
}
