#include "pch.h"
#include "CUIPanel.h"

#include "CTransform.h"

CUIPanel::CUIPanel()
	: CUIBase{COMPONENT_TYPE::UIPANEL}
	, m_isSorted{false} {}

CUIPanel::CUIPanel(const CUIPanel& _origin)
	: CUIBase{_origin}
	, m_isSorted{false} {}

CUIPanel::~CUIPanel() = default;

void CUIPanel::finalupdate()
{
	if (m_vecUIChild.size() != GetOwner()->GetChild().size())
	{
		m_isSorted = false;
	}
	//Panel�� ����� �ٸ� UI���� ��ȣ�� ������ ��쵵 �����Ƿ� �и��ؼ� ó��
	if (false == m_isSorted)
	{
		SortGameObject();
	}

	CUIBase::finalupdate();
}

void CUIPanel::SortGameObject()
{
	auto func = [](CGameObject* pLeft, CGameObject* pRight)
	{
		const CUIBase* pLeftUI = pLeft->GetUIBaseComponenent();
		assert(pLeftUI);
		const CUIBase* pRightUI = pRight->GetUIBaseComponenent();
		assert(pRightUI);

		return pLeftUI->GetOrderZ() > pRightUI->GetOrderZ();
	};
	GetOwner()->SortChild(func);

	m_vecUIChild.clear();
	for (auto& pUIObject : GetOwner()->GetChild())
	{
		assert(pUIObject->GetUIBaseComponenent());
		m_vecUIChild.push_back(pUIObject->GetUIBaseComponenent());
	}
	std::reverse(m_vecUIChild.begin(), m_vecUIChild.end());
	m_isSorted = true;
}

bool CUIPanel::CollisionMouse()
{
	for (const auto& pUIWidget : m_vecUIChild)
	{
		if (pUIWidget->CollisionMouse() && pUIWidget->m_hoverCallback)
		{
			pUIWidget->m_isMouseHovered = true;
			pUIWidget->m_hoverCallback();
			return true;
		}
		pUIWidget->m_isMouseHovered = false;
		return false;
	}
	return false;
}
