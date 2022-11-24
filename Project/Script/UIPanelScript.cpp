#include "pch.h"
#include "UIPanelScript.h"

#include "CObjectManager.h"

UIPanelScript::UIPanelScript()
	: CUIBase{SCRIPT_TYPE::UIPANELSCRIPT, UI_TYPE::PANEL}
	, m_isSorted{false}
{
}

UIPanelScript::~UIPanelScript() = default;

void UIPanelScript::start()
{
	Transform()->SetRelativePos(0.f, 0.f, 500.f);
	//CObjectManager::GetInst()->AddScriptEvent(this, &UIPanelScript::SortGameObject);
}

void UIPanelScript::update()
{
	if (m_vecUIChild.size() != GetOwner()->GetChild().size())
	{
		m_isSorted = false;
	}
	if (false == m_isSorted)
	{
		CObjectManager::GetInst()->AddScriptEvent(this, &UIPanelScript::SortGameObject);
	}

	CUIBase::update();
}

void UIPanelScript::lateupdate()
{
	CUIBase::lateupdate();
	const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(m_textureName);
	assert(nullptr != pTexture);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
}

void UIPanelScript::SortGameObject()
{
	auto func = [](CGameObject* pLeft, CGameObject* pRight)
	{
		CUIBase* pLeftUI = pLeft->GetScript<CUIBase>();
		assert(pLeftUI);
		CUIBase* pRightUI = pRight->GetScript<CUIBase>();
		assert(pRightUI);

		return pLeftUI->GetOrderZ() > pRightUI->GetOrderZ();
	};
	GetOwner()->SortChild(func);

	m_vecUIChild.clear();
	for (auto& pUIObject : GetOwner()->GetChild())
	{
		CUIBase* pScript = pUIObject->GetScript<CUIBase>();
		assert(pScript);
		pScript->SetPanel(this);
		m_vecUIChild.push_back(pScript);
	}
	std::reverse(m_vecUIChild.begin(), m_vecUIChild.end());
	m_isSorted = true;
}

bool UIPanelScript::CollisionMouse()
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
