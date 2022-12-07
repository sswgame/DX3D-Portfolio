#pragma once
#include "CUIBase.h"

class CUIPanel final
	: public CUIBase
{
private:
	std::vector<CUIBase*> m_vecUIChild;
	bool                  m_isSorted;
private:
	void SortGameObject();
public:
	bool CollisionMouse() override;
	void SetSorted(bool enable) { m_isSorted = enable; }
public:
	void finalupdate() override;

public:
	CLONE(CUIPanel);
	CUIPanel();
	CUIPanel(const CUIPanel& _origin);
	virtual ~CUIPanel();
};
