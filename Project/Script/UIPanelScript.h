#pragma once
#include "CUIBase.h"

class UIPanelScript final
	: public CUIBase
{
private:
	std::vector<CUIBase*> m_vecUIChild;
	bool                  m_isSorted;
	std::string           m_textureName;
private:
	void SortGameObject();
public:
	bool CollisionMouse() override;
	void SetTexture(const std::string& texturePath) { m_textureName = texturePath; }
	void SetSorted(bool enable) { m_isSorted = enable; }
public:
	void start() override;
	void update() override;
	void lateupdate() override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CLONE(UIPanelScript);
	UIPanelScript();
	UIPanelScript(const UIPanelScript& _origin);
	virtual ~UIPanelScript();
};
