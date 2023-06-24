#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CGameObject.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "IconsFontAwesome5.h"
#include "ImGuizmo/ImGuizmo.h"

#include "UI.h"
#include "SceneOutliner.h"
#include "MenuUI.h"
#include "InspectorUI.h"
#include "ResourceUI.h"
#include "ListUI.h"
#include "ParamUI.h"
#include "ParticleTool.h"
#include "FogTool.h"
#include "UITool.h"
#include "../Engine/Engine/CEventMgr.h"

CImGuiMgr::CImGuiMgr()
	: m_hNotify{nullptr} {}

CImGuiMgr::~CImGuiMgr()
{
	Safe_Del_Map(m_mapUI);
}

void CImGuiMgr::init(HWND _hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows

	// �ѱ� ��Ʈ �߰�
	const string strFontPath = ToString(CPathMgr::GetInst()->GetContentPath() + L"font\\Pretendard-Medium.ttf");
	io.Fonts->AddFontFromFileTTF(strFontPath.c_str(), 15.f, nullptr, io.Fonts->GetGlyphRangesKorean());

	// Font Awesome Icon �߰�
	const string strFontAwesomeIcon = ToString(CPathMgr::GetInst()->GetContentPath() + L"font/fa-solid-900.ttf");
	ImFontConfig config{};
	config.MergeMode                       = true;
	config.GlyphMinAdvanceX                = 13.0f; // Use if you want to make the icon monospaced
	static constexpr ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	io.Fonts->AddFontFromFileTTF(strFontAwesomeIcon.c_str(),
	                             13.0f,
	                             &config,
	                             icon_ranges);    // outputs a paint brush icon and 'Paint' as a string.
	io.Fonts->Build();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding            = 5.f;
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);
		style.FramePadding              = ImVec2(1.5f, 1.5f);
		style.FrameRounding             = 1.f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(DEVICE, CONTEXT);

	// �⺻ UI �� ����
	CreateUI();

	// �˸�����  
	const wstring strPath = CPathMgr::GetInst()->GetContentPath();
	m_hNotify             = FindFirstChangeNotification(strPath.c_str(),
	                                                    TRUE,
	                                                    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
	                                                    FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CImGuiMgr::progress()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// Param Init
	ParamUI::KeyCount = 0;

	// UI Update
	for (const auto& [UIName, pUI] : m_mapUI)
	{
		pUI->update();
	}
	// UI Render
	for (const auto& [UIName, pUI] : m_mapUI)
	{
		pUI->render();
	}

	// Delegate ȣ��
	for (auto& UIDelegate : m_vecDelegate)
	{
		(UIDelegate.pInst->*UIDelegate.pFunc)(UIDelegate.dwParam);
	}
	m_vecDelegate.clear();

	// Content ���� ����
	ObserveContent();
}


void CImGuiMgr::render()
{
	SceneOutliner* pTest = (SceneOutliner*)FindUI("SceneOutliner");
	pTest->RenderGuizmo();
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}


void CImGuiMgr::clear()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void CImGuiMgr::CreateUI()
{
	// MenuBar ����
	MenuUI* pMenuUI = new MenuUI;
	pMenuUI->Activate();
	m_mapUI.insert(make_pair(pMenuUI->GetName(), pMenuUI));

	// InspectorUI ����
	InspectorUI* pUI = new InspectorUI;
	pUI->SetTargetObject(nullptr);
	m_mapUI.insert(make_pair(pUI->GetName(), pUI));

	// ResourceUI ����
	ResourceUI* pResUI = new ResourceUI;
	m_mapUI.insert(make_pair(pResUI->GetName(), pResUI));

	// SceneOutLiner
	SceneOutliner* pSceneOutliner = new SceneOutliner;
	m_mapUI.insert(make_pair(pSceneOutliner->GetName(), pSceneOutliner));

	// ListUI ����
	ListUI* pListUI = new ListUI;
	pListUI->Deactivate();
	pListUI->SetModalUI(true);
	m_mapUI.insert(make_pair(pListUI->GetName(), pListUI));

	// Particle Tool UI ����
	ParticleTool* pParticleTool = new ParticleTool;
	pParticleTool->Deactivate();
	m_mapUI.insert(make_pair(pParticleTool->GetName(), pParticleTool));

	// Fog Tool UI ����
	FogTool* pFogTool = new FogTool;
	pFogTool->Deactivate();
	m_mapUI.insert(make_pair(pFogTool->GetName(), pFogTool));

	UITool* pUITool = new UITool{};
	pUITool->Deactivate();
	m_mapUI.insert({pUITool->GetName(), pUITool});
}

UI* CImGuiMgr::FindUI(const string& _strKey)
{
	const auto iter = m_mapUI.find(_strKey);
	if (m_mapUI.end() == iter)
	{
		return nullptr;
	}
	return iter->second;
}

void CImGuiMgr::ObserveContent()
{
	const DWORD dwWaitStatus = WaitForSingleObject(m_hNotify, 0);
	if (dwWaitStatus == WAIT_OBJECT_0)
	{
		LOG_INFO("CONTENTS HAS CHANGED");
		ResourceUI* pResUI = static_cast<ResourceUI*>(FindUI("Resource"));
		pResUI->Reset();

		FindNextChangeNotification(m_hNotify);
	}
}
