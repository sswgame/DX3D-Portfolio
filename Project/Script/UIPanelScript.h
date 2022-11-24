#pragma once
#include "CUIBase.h"

class UIPanelScript final
	: public CUIBase
{
private:
	std::vector<CUIBase*> m_vecUIChild;
	bool                  m_isSorted;
	std::wstring          m_textureName;
private:
	void SortGameObject();
public:
	bool CollisionMouse() override;
	void SetTexture(const std::wstring& texturePath) { m_textureName = texturePath; }
	void SetSorted(bool enable) { m_isSorted = enable; }
public:
	void start() override;
	void update() override;
	void lateupdate() override;
public:
	CLONE(UIPanelScript);
	UIPanelScript();
	virtual ~UIPanelScript();
};
