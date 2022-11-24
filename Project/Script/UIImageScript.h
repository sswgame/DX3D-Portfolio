#pragma once
#include "CUIBase.h"

struct tImageInfo
{
	Vec2 imagePos;
	Vec2 imageSize;
};

class UIImageScript final
	: public CUIBase
{
private:
	std::map<std::wstring, tImageInfo> m_mapInfo;
	tImageInfo                         m_currentInfo;
	int                                m_useInfo;
	bool                               m_preserveRatio;
	float                              m_ratio;

	std::wstring m_textureName;
private:
	auto FindImageInfo(const std::wstring& frameName) { return m_mapInfo.find(frameName); };

public:
	void update() override;
	void lateupdate() override;
public:
	void Clear();

	void SetTexture(const std::wstring& texturePath) { m_textureName = texturePath; }

	void       AddImageInfo(const std::wstring& frameName, const Vec2& pos, const Vec2& size);
	tImageInfo GetImageInfo(const std::wstring& frameName);

	void SetImageInfo(const std::wstring& frameName);
	void SetPreserveRatio(bool enable);
public:
	CLONE(UIImageScript);
	UIImageScript();
	virtual ~UIImageScript();
};
