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
	std::string                        m_currentFrameName;
	int                                m_useInfo;
	bool                               m_preserveRatio;
	float                              m_ratio;

	std::string m_textureName;
private:
	auto FindImageInfo(const std::wstring& frameName) { return m_mapInfo.find(frameName); };
	void SetUseInfo(bool enable);
public:
	void update() override;
	void lateupdate() override;
public:
	void Clear();

	void SetTexture(const std::string& texturePath) { m_textureName = texturePath; }

	void       AddImageInfo(const std::wstring& frameName, const Vec2& pos, const Vec2& size);
	tImageInfo GetImageInfo(const std::wstring& frameName);

	void SetImageInfo(const std::wstring& frameName);
	void SetPreserveRatio(bool enable);

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CLONE(UIImageScript);
	UIImageScript();
	UIImageScript(const UIImageScript& _origin);
	virtual ~UIImageScript();
};
