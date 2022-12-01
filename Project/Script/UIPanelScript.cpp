#include "pch.h"
#include "UIPanelScript.h"

#include "CObjectManager.h"

UIPanelScript::UIPanelScript()
	: CUIBase{SCRIPT_TYPE::UIPANELSCRIPT}
	, m_isSorted{false}
{
	AddScriptParam("TEXTURE NAME", SCRIPTPARAM_TYPE::TEXTURE, &m_textureName);
}

UIPanelScript::UIPanelScript(const UIPanelScript& _origin)
	: CUIBase{_origin}
	, m_isSorted{false}
	, m_textureName{_origin.m_textureName}
{
	RenewScalarParam("TEXTURE NAME", &m_textureName);
}

UIPanelScript::~UIPanelScript() = default;

void UIPanelScript::start()
{
	CUIBase::start();
	Transform()->SetRelativePos(0.f, 0.f, 500.f);
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
	if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial(0) || nullptr == MeshRender()->GetMesh())
	{
		return;
	}
	CUIBase::lateupdate();
	const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(ToWString(m_textureName));
	if (nullptr != pTexture)
	{
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	}
}

void UIPanelScript::SortGameObject()
{
	auto func = [](CGameObject* pLeft, CGameObject* pRight)
	{
		const CUIBase* pLeftUI = pLeft->GetScript<CUIBase>();
		assert(pLeftUI);
		const CUIBase* pRightUI = pRight->GetScript<CUIBase>();
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

void UIPanelScript::Serialize(YAML::Emitter& emitter)
{
	CUIBase::Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_textureName) << m_textureName;
}

void UIPanelScript::Deserialize(const YAML::Node& node)
{
	CUIBase::Deserialize(node);
	m_textureName = node[NAME_OF(m_textureName)].as<std::string>();
	RenewScalarParam("TEXTURE NAME", &m_textureName);
}
