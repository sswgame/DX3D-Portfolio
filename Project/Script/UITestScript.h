#pragma once
#include <Engine/CScript.h>

class UITestScript
	: public CScript
{
private:
	bool m_bLoaded;

private:
	void CreateUI();
	void BindFunc();

	void OneClick();
	void DoubleClick();
	void MouseHover();
	void Drag();

public:
	void start() override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CLONE(UITestScript);
	UITestScript();
	virtual ~UITestScript();
};
