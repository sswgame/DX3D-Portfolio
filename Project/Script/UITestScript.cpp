#include "pch.h"
#include "UITestScript.h"

#include "ButtonScript.h"
#include "CObjectManager.h"
#include "UIImageScript.h"
#include "UIPanelScript.h"
#include "UITextScript.h"


UITestScript::UITestScript()
	: CScript{(int)SCRIPT_TYPE::UITESTSCRIPT}
	, m_bLoaded{false} {}

UITestScript::~UITestScript() = default;

void UITestScript::start()
{
	CreateUI();
	CObjectManager::GetInst()->AddScriptEvent(this, &UITestScript::BindFunc);
}


void UITestScript::CreateUI()
{
	CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\UI_NameBox_Tex.png", L"texture\\UI\\UI_NameBox_Tex.png");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\StageBGUI_Tex.png", L"texture\\UI\\StageBGUI_Tex.png");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\Cursor.png", L"texture\\UI\\Cursor.png");
	CResMgr::GetInst()->Load<CMaterial>(L"material\\UIMtrl.mtrl", L"material\\UIMtrl.mtrl");

	if (m_bLoaded)
	{
		return;
	}
	//PANEL UI
	GetOwner()->SetName(L"UI_PANEL");
	GetOwner()->Transform()->SetRelativeScale(500.f, 500.f, 1.f);
	GetOwner()->AddComponent(new UIPanelScript{});

	//IMAGE UI_1
	CGameObject* pImageUI_1 = new CGameObject{};
	pImageUI_1->SetName(L"UI_1");
	pImageUI_1->AddComponent(new CTransform{});
	pImageUI_1->AddComponent(new CMeshRender{});
	pImageUI_1->AddComponent(new UIImageScript{});
	CSceneMgr::GetInst()->SpawnObject(pImageUI_1, GetOwner()->GetLayerIndex());
	CSceneMgr::GetInst()->AddChild(GetOwner(), pImageUI_1);

	//IMAMGE_UI_2
	CGameObject* pImageUI_2 = new CGameObject{};
	pImageUI_2->SetName(L"UI_2");
	pImageUI_2->AddComponent(new CTransform{});
	pImageUI_2->AddComponent(new CMeshRender{});
	pImageUI_2->AddComponent(new UIImageScript{});
	pImageUI_2->AddComponent(new ButtonScript{});
	CSceneMgr::GetInst()->SpawnObject(pImageUI_2, GetOwner()->GetLayerIndex());
	CSceneMgr::GetInst()->AddChild(GetOwner(), pImageUI_2);

	//TextUI
	CGameObject* pTextUI = new CGameObject{};
	pTextUI->SetName(L"TEXT_UI");
	pTextUI->AddComponent(new CTransform{});
	pTextUI->AddComponent(new UITextScript{});
	CSceneMgr::GetInst()->SpawnObject(pTextUI, GetOwner()->GetLayerIndex());
	CSceneMgr::GetInst()->AddChild(GetOwner(), pTextUI);

	m_bLoaded = true;
}

void UITestScript::BindFunc()
{
	if (GetOwner()->GetScript<UIPanelScript>())
	{
		GetOwner()->GetScript<UIPanelScript>()->SetSorted(false);
	}
	CGameObject* pObject = GetOwner()->GetChild(L"UI_2");
	assert(pObject);
	pObject->GetScript<UIImageScript>()->SetMouseHoverCallback(this, &UITestScript::MouseHover);
	pObject->GetScript<ButtonScript>()->SetCallback(this,
	                                                &UITestScript::Drag,
	                                                MOUSE_TYPE::LEFT,
	                                                CLICK_TYPE::DRAG);
}

void UITestScript::OneClick()
{
	MessageBox(nullptr, L"CL", L"C", 0);
}

void UITestScript::DoubleClick()
{
	OutputDebugString(L"DOUBLE\n");
}

void UITestScript::MouseHover()
{
	OutputDebugString(L"HOVER\n");
}

void UITestScript::Drag()
{
	Vec2 deltaPos = CKeyMgr::GetInst()->GetMouseDelta();

	CUIBase* pScript = GetOwner()->GetScript<CUIBase>();
	assert(pScript);
	Vec2 currentOffset = pScript->GetOffsetPos();
	pScript->SetOffsetPos(currentOffset + Vec2{deltaPos.x, -deltaPos.y});
}

void UITestScript::Serialize(YAML::Emitter& emitter)
{
	CScript::Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_bLoaded) << m_bLoaded;
}

void UITestScript::Deserialize(const YAML::Node& node)
{
	CScript::Deserialize(node);
	m_bLoaded = node[NAME_OF(m_bLoaded)].as<bool>();
}
