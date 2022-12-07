#include "pch.h"
#include "UITool.h"

#include <Engine/CCore.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CFontMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CUIImage.h>
#include <Engine/CUIText.h>
#include <Engine/CUIButton.h>
#include <Engine/CUIProgressBar.h>
#include <Engine/CUIPanel.h>

#include "CImGuiMgr.h"
#include "TreeUI.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ListUI.h"
#include "ImGui/imgui_internal.h"

UITool::UITool()
	: UI{"##UITool"}
{
	m_pToolSceneTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"ToolRenderTargetTex");

	m_pTree = new TreeUI{true};
	m_pTree->SetTitle(u8"UI 계층도");
	m_pTree->UseFrame(false);
	m_pTree->ShowDummyRoot(false);
	m_pTree->UseDragDropSelf(true);
	m_pTree->SetClickedDelegate(this, static_cast<CLICKED>(&UITool::ItemClicked));
}

UITool::~UITool()
{
	SAFE_DELETE(m_pTree);
}

void UITool::render_update()
{
	DrawImage();
	ImGui::SameLine();
	if (ImGui::BeginChild("UI_INFO_SECTION"))
	{
		DrawHierarchy();
		ImGui::SameLine();
		DrawInfo();
		ImGui::EndChild();
	}

	if (m_bOpenPopUp)
	{
		ImGui::OpenPopup("UI_WARNING");
		m_bOpenPopUp = false;
	}
	DrawWarning();

	if (m_bOpenFrameTexture)
	{
		ImGui::OpenPopup("UI_FRAME_TEXTURE");
		m_bOpenFrameTexture = false;
	}
	DrawOpenFrameTexture();
}

void UITool::DrawImage()
{
	const ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::BeginChild(u8"UI_TOOL_SCENE",
	                  {windowSize.x * 0.5f, windowSize.y},
	                  true,
	                  ImGuiWindowFlags_HorizontalScrollbar);
	assert(nullptr != m_pToolSceneTargetTex);
	ImGui::Image(m_pToolSceneTargetTex->GetSRV().Get(), windowSize * ImVec2{0.5f, 1.f});
	ImGui::EndChild();
}

void UITool::CropImageByDrag()
{
	const ImVec2      windowPos  = ImGui::GetWindowPos();
	const ImVec2      currentPos = ImGui::GetMousePos() - windowPos;
	ImDrawList* const pDrawList  = ImGui::GetWindowDrawList();

	const bool isHovered = ImGui::IsItemHovered();
	//clicked first
	if (true == isHovered && false == m_isDrawing && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		m_isDrawing    = true;
		m_bHasDragArea = false;
		m_startPos     = ImGui::GetMousePos() - windowPos;
	}

	//still drawing
	if (m_isDrawing)
	{
		//mouse up
		if (false == ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			m_isDrawing = false;
			m_lastPos   = currentPos;

			m_bHasDragArea = true;
			CalculatePosAndSize();
		}
		//draw rect
		pDrawList->AddRect(m_startPos + windowPos, currentPos + windowPos, IM_COL32(0, 255, 0, 255));
	}

	if (m_bHasDragArea)
	{
		pDrawList->AddRect(m_startPos + windowPos, m_lastPos + windowPos, IM_COL32(0, 255, 0, 255));
	}
}

void UITool::DrawHierarchy()
{
	const ImVec2 windowSize = ImGui::GetWindowSize();
	const float  width      = windowSize.x * 0.5f;
	const float  height     = windowSize.y * 0.25f;
	ImGui::BeginChild(u8"UI_HIERARCHY", ImVec2{width, height}, true);

	m_pTree->render_update();

	ImGui::EndChild();
}

void UITool::DrawInfo()
{
	ImGui::BeginGroup();
	if (ImGui::Button(u8"패널 생성"))
	{
		AddUIObject(GAME_UI_TYPE::PANEL);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"이미지 생성"))
	{
		AddUIObject(GAME_UI_TYPE::IMAGE);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"텍스트 생성"))
	{
		AddUIObject(GAME_UI_TYPE::TEXT);
	}

	if (ImGui::Button(u8"버튼 기능 추가"))
	{
		AddExtraFeature(UI_EXTRA_TYPE::BUTTON);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"프로그레스 바 기능 추가"))
	{
		AddExtraFeature(UI_EXTRA_TYPE::PROGRESS_BAR);
	}

	if (ImGui::Button(u8"불러오기"))
	{
		wchar_t szName[256] = {};

		OPENFILENAME ofn = {};

		ofn.lStructSize    = sizeof(OPENFILENAME);
		ofn.hwndOwner      = CCore::GetInst()->GetMainHwnd();
		ofn.lpstrFile      = szName;
		ofn.nMaxFile       = sizeof(szName);
		ofn.lpstrFilter    = L"Prefab\0*.pref\0";
		ofn.nFilterIndex   = 0;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle  = 0;

		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder += L"prefab";

		ofn.lpstrInitialDir = strTileFolder.c_str();
		ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Modal
		if (GetOpenFileName(&ofn))
		{
			std::wstring relativePath = CPathMgr::GetInst()->GetRelativePath(szName);
			Ptr<CPrefab> pPrefab      = CResMgr::GetInst()->FindRes<CPrefab>(relativePath);
			assert(nullptr != pPrefab);
			CUIBase* pAnyUIComponent = pPrefab->GetProto()->GetUIBaseComponenent();
			if (nullptr == pAnyUIComponent)
			{
				m_bShowWarning = true;
				m_bWrongPrefab = true;
				m_bOpenPopUp   = true;
			}
			else
			{
				CGameObject*  pGameObject = pPrefab->Instantiate();
				const CScene* pToolScene  = CSceneMgr::GetInst()->GetToolScene();
				pToolScene->AddObject(pGameObject, L"UI_INTERACTIVE");
				TreeRenew();

				m_pSelectedNode = m_pTree->FindNode(ToString(pGameObject->GetName()));
				m_pTree->SetSelectedNode(m_pSelectedNode);
			}
		}
	}

	if (m_pSelectedNode)
	{
		auto pGameObject = reinterpret_cast<CGameObject*>(m_pSelectedNode->GetData());
		if (pGameObject->UIImage())
		{
			ImGui::Text(u8"드래그 정보");
			ImGui::Text("LEFT TOP");
			ImGui::InputFloat2("##DRAG_LEFTOP", &m_leftTop.x);
			ImGui::Text("SIZE");
			ImGui::InputFloat2("##DRAG_SIZE", &m_size.x);

			Ptr<CTexture> pTexture = pGameObject->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
			if (nullptr != pTexture && ImGui::Button(u8"프레임 선택"))
			{
				m_pFrameTexture     = pTexture;
				m_bShowFrameTexture = true;
				m_bOpenFrameTexture = true;
			}
		}

		if (ImGui::Button(u8"프리팹으로 저장"))
		{
			wchar_t szName[256] = {};

			OPENFILENAME ofn = {};

			ofn.lStructSize    = sizeof(OPENFILENAME);
			ofn.hwndOwner      = CCore::GetInst()->GetMainHwnd();
			ofn.lpstrFile      = szName;
			ofn.nMaxFile       = sizeof(szName);
			ofn.lpstrFilter    = L"Prefab\0*.pref\0";
			ofn.nFilterIndex   = 0;
			ofn.lpstrFileTitle = nullptr;
			ofn.nMaxFileTitle  = 0;

			wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
			strTileFolder += L"prefab";

			ofn.lpstrInitialDir = strTileFolder.c_str();
			ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Modal
			if (GetOpenFileName(&ofn))
			{
				std::wstring relativePath = CPathMgr::GetInst()->GetRelativePath(szName);
				Ptr<CPrefab> pPrefab      = CResMgr::GetInst()->FindRes<CPrefab>(relativePath);

				//이미 있는 프리팹을 선택하였으므로, 해당 프리팹을 강제 교체.
				//단 UI Prefab이 아닌 경우 선택 실패
				if (nullptr != pPrefab)
				{
					CUIBase* pAnyUIComponent = pPrefab->GetProto()->GetUIBaseComponenent();
					if (nullptr == pAnyUIComponent)
					{
						m_bShowWarning = true;
						m_bWrongPrefab = true;
						m_bOpenPopUp   = true;
					}
					else
					{
						//메모리에서 삭제
						CResMgr::GetInst()->ForceDeleteRes<CPrefab>(pPrefab->GetKey());
						//파일 자체를 삭제
						std::filesystem::remove(szName);
					}
				}
				//ToolScene의 루트를 프리팹화
				const CScene* pToolScene = CSceneMgr::GetInst()->GetToolScene();
				CGameObject*  pRoot      = pToolScene->GetLayer(L"UI_INTERACTIVE")->GetRootObjects().at(0);
				pPrefab                  = new CPrefab{};
				pPrefab->SetProto(pRoot->Clone());
				pPrefab->Save(szName);
				CResMgr::GetInst()->AddRes<CPrefab>(relativePath, pPrefab.Get());
			}
		}
	}
	ImGui::EndGroup();

	if (m_pSelectedNode)
	{
		auto pGameObject = reinterpret_cast<CGameObject*>(m_pSelectedNode->GetData());

		CUIBase* pUIBaseComponent = pGameObject->GetUIBaseComponenent();
		if (pUIBaseComponent)
		{
			COMPONENT_TYPE type = pUIBaseComponent->GetType();
			switch (type)
			{
			case COMPONENT_TYPE::UITEXT:
				ShowTextInfo(static_cast<CUIText*>(pUIBaseComponent));
				break;
			case COMPONENT_TYPE::UIIMAGE:
				ShowImageInfo(static_cast<CUIImage*>(pUIBaseComponent));
				break;
			case COMPONENT_TYPE::UIPANEL:
				ShowPanelInfo(static_cast<CUIPanel*>(pUIBaseComponent));
				break;
			}
		}
		CComponent* pProgressBar = pGameObject->GetComponent(COMPONENT_TYPE::UIPROGRESSBAR);
		if (nullptr != pProgressBar)
		{
			ShowProgressBarInfo(static_cast<CUIProgressBar*>(pProgressBar));
		}
		CComponent* pButton = pGameObject->GetComponent(COMPONENT_TYPE::UIBUTTON);
		if (nullptr != pButton)
		{
			ShowButtonInfo(static_cast<CUIButton*>(pButton));
		}
	}
}

void UITool::DrawWarning()
{
	if (m_bShowWarning)
	{
		if (ImGui::BeginPopup("UI_WARNING"))
		{
			if (m_bWrongPrefab)
			{
				ImGui::Text(u8"선택한 프리팹은 UI 프리팹이 아닙니다");
				ImGui::Text(u8"다시 선택하세요");
			}
			else if (nullptr == m_pSelectedNode)
			{
				ImGui::Text(u8"선택된 UI 객체가 없습니다");
				ImGui::Text(u8"객체를 추가하고 Button/Progress bar를 축하세요");
			}
			else if (m_bAlreadyHas)
			{
				ImGui::Text(u8"이미 Button 또는 Progress bar가 존재합니다");
			}
			else if (m_bNoImageScript)
			{
				ImGui::Text(u8"Button/ProgressBar는 UIImage/UIPanel/UIText를 가진 객체에만 가능합니다");
			}
			else
			{
				ImGui::Text(u8"선택된 UI는 Panel Component가 존재하지 않습니다.");
				ImGui::Text(u8"Component 종류를 변경하세요");
			}
			if (ImGui::Button(u8"닫기"))
			{
				m_bShowWarning   = false;
				m_bWrongPrefab   = false;
				m_bAlreadyHas    = false;
				m_bNoImageScript = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void UITool::DrawOpenFrameTexture()
{
	if (ImGui::BeginPopupModal("UI_FRAME_TEXTURE",
	                           &m_bShowFrameTexture,
	                           ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
		const ImVec2 windowSize = ImGui::GetWindowSize();
		if (ImGui::BeginChild(u8"##UI_FRAME_TEXTURE",
		                      windowSize,
		                      true,
		                      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Image(m_pFrameTexture->GetSRV().Get(), windowSize);
			CropImageByDrag();

			ImGui::EndChild();
		}
		ImGui::PopStyleVar(3);
		ImGui::EndPopup();
	}
}

void UITool::TreeRenew()
{
	m_pTree->Clear();

	CGameObject* pUIRoot = CSceneMgr::GetInst()->GetToolScene()->GetLayer(L"UI_INTERACTIVE")->GetRootObjects().at(0);

	m_pTree->AddTreeNode(nullptr, ToString(pUIRoot->GetName()), (DWORD_PTR)pUIRoot);

	for (auto& pChild : pUIRoot->GetChild())
	{
		const std::string parentUIName = (pChild->GetParent()) ? ToString(pChild->GetParent()->GetName()) : "";
		TreeNode*         pParentNode  = parentUIName.empty() ? nullptr : m_pTree->FindNode(parentUIName);
		m_pTree->AddTreeNode(pParentNode, ToString(pChild->GetName()), (DWORD_PTR)pChild);
	}
}

void UITool::SelectTexture(DWORD_PTR _pTextureName)
{
	const auto          pTextureName = (const char*)_pTextureName;
	const Ptr<CTexture> pTexture     = CResMgr::GetInst()->FindRes<CTexture>(ToWString(pTextureName));
	const auto          pGameObject  = (CGameObject*)m_pSelectedNode->GetData();
	pGameObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
}

void UITool::SelectFontName(DWORD_PTR _pFontName)
{
	const auto pFontName   = (const char*)_pFontName;
	const auto pGameObject = (CGameObject*)m_pSelectedNode->GetData();
	pGameObject->UIText()->SetFont(ToWString(pFontName));
}

void UITool::AddObjectDelegate(DWORD_PTR _pGameObject)
{
	auto pGameObject = reinterpret_cast<CGameObject*>(_pGameObject);

	if (nullptr == m_pSelectedNode)
	{
		const CScene* pToolScene = CSceneMgr::GetInst()->GetToolScene();
		pGameObject->Transform()->SetRelativePos({0.f, 0.f, 500.f});
		pToolScene->AddObject(pGameObject, L"UI_INTERACTIVE");
	}
	else
	{
		auto            pParentUIObject = reinterpret_cast<CGameObject*>(m_pSelectedNode->GetData());
		const CUIPanel* pPanelScript    = pParentUIObject->GetScript<CUIPanel>();
		if (nullptr == pPanelScript)
		{
			m_bShowWarning = true;
			m_bOpenPopUp   = true;
			pGameObject->Destroy();

			return;
		}

		pParentUIObject->AddChild(pGameObject);
	}
	TreeRenew();

	m_pSelectedNode       = m_pTree->FindNode(ToString(pGameObject->GetName()));
	TreeNode* pParentNode = m_pSelectedNode->GetParent();
	while (pParentNode)
	{
		pParentNode->SetCheckOn();
		pParentNode = pParentNode->GetParent();
	}
	m_pTree->SetSelectedNode(m_pSelectedNode);
}

void UITool::AddUIObject(GAME_UI_TYPE type)
{
	const std::wstring generatedName = L"UI_" + std::to_wstring(s_geneatedID++);

	auto pNewGameObject = new CGameObject{};
	pNewGameObject->SetName(generatedName);
	pNewGameObject->AddComponent(new CTransform{});
	pNewGameObject->Transform()->SetIgnoreParentScale(true);
	pNewGameObject->AddComponent(new CMeshRender{});
	pNewGameObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pNewGameObject->MeshRender()->
	                SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\UIMtrl.mtrl"), 0);

	switch (type)
	{
	case GAME_UI_TYPE::PANEL:
		pNewGameObject->AddComponent(new CUIPanel{});
		break;
	case GAME_UI_TYPE::IMAGE:
		pNewGameObject->AddComponent(new CUIImage{});
		break;
	case GAME_UI_TYPE::TEXT:
		pNewGameObject->AddComponent(new CUIText{});
		break;
	default: assert(nullptr && "INVALID UI SCRIPT TYPE");
	}

	tUIDelegate uiDelegate{};
	uiDelegate.pInst   = this;
	uiDelegate.pFunc   = static_cast<PARAM_1>(&UITool::AddObjectDelegate);
	uiDelegate.dwParam = (DWORD_PTR)pNewGameObject;
	CImGuiMgr::GetInst()->AddDelegate(uiDelegate);
}

void UITool::AddExtraFeature(UI_EXTRA_TYPE type)
{
	if (nullptr == m_pSelectedNode)
	{
		m_bShowWarning = true;
		m_bOpenPopUp   = true;
		return;
	}

	auto pGameObject = (CGameObject*)m_pSelectedNode->GetData();
	if (nullptr == pGameObject->GetUIBaseComponenent())
	{
		m_bShowWarning   = true;
		m_bOpenPopUp     = true;
		m_bNoImageScript = true;

		return;
	}
	switch (type)
	{
	case UI_EXTRA_TYPE::BUTTON:
		{
			if (pGameObject->UIText())
			{
				m_bShowWarning = true;
				m_bAlreadyHas  = true;
				m_bOpenPopUp   = true;
			}
			else
			{
				pGameObject->AddComponent(new CUIButton{});
			}
			break;
		}
	case UI_EXTRA_TYPE::PROGRESS_BAR:
		{
			if (pGameObject->UIProgressBar())
			{
				m_bShowWarning = true;
				m_bAlreadyHas  = true;
				m_bOpenPopUp   = true;
			}
			else
			{
				pGameObject->AddComponent(new CUIProgressBar{});
			}
			break;
		}
	}
}

void UITool::ItemClicked(DWORD_PTR _dwUIGameObject)
{
	m_pSelectedNode = reinterpret_cast<TreeNode*>(_dwUIGameObject);
}

void UITool::CalculatePosAndSize()
{
	//좌상단->우하단
	if (m_lastPos.x - m_startPos.x > 0 && m_lastPos.y - m_startPos.y > 0)
	{
		m_leftTop = m_startPos;
		m_size    = m_lastPos - m_startPos;
	}
	//우하단->좌상단
	else if (m_lastPos.x - m_startPos.x < 0 && m_lastPos.y - m_startPos.y < 0)
	{
		m_leftTop = m_lastPos;
		m_size    = m_startPos - m_lastPos;
	}
	//좌하단->우상단
	else if (m_lastPos.x - m_startPos.x > 0 && m_lastPos.y - m_startPos.y < 0)
	{
		m_leftTop = ImVec2{m_startPos.x, m_lastPos.y};
		m_size    = ImVec2{m_lastPos.x - m_startPos.x, m_startPos.y - m_lastPos.y};
	}
	//우상단->좌하단
	else if (m_lastPos.x - m_startPos.x < 0 && m_lastPos.y - m_startPos.y > 0)
	{
		m_leftTop = ImVec2{m_lastPos.x, m_startPos.y};
		m_size    = ImVec2{m_startPos.x - m_lastPos.x, m_lastPos.y - m_startPos.y};
	}
	//둘 중 한 쪽 축으로 이동한 것이므로 사이즈가 없다.
	else
	{
		m_bHasDragArea = false;
		m_leftTop      = ImVec2{0, 0};
		m_size         = ImVec2{0, 0};
	}
}

void UITool::ShowUIDefaultInfo(CUIBase* pScript)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[DEFAULT]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("SHOW FULL RANGE");
	ImGui::SameLine(200);
	static bool showFullRange = false;
	ImGui::Checkbox("##UI_SHOW_RANGE", &showFullRange);
	pScript->ShowDebugRect(showFullRange);

	bool enableCollision = pScript->GetMouseCollision();
	ImGui::Text("MOUSE COLLISION");
	ImGui::SameLine(200);
	ImGui::Checkbox("##UI_COLLISION_EANBLE", &enableCollision);
	pScript->SetMouseCollision(enableCollision);

	ImGui::Text("OFFSET POS");
	ImGui::SameLine(200);
	Vec2 offsetPos = pScript->GetOffsetPos();
	ImGui::InputFloat2("##UI_OFFSET", &offsetPos.x);
	pScript->SetOffsetPos(offsetPos);

	Vec3 scale = pScript->GetOwner()->Transform()->GetRelativeScale();
	ImGui::Text("UI SIZE");
	ImGui::InputFloat2("##UI_SIZE", &scale.x);
	pScript->GetOwner()->Transform()->SetRelativeScale(scale);

	ImGui::Text("Z ORDER");
	ImGui::SameLine(200);
	int zOrder = pScript->GetOrderZ();
	ImGui::InputInt("##UI_Z_ORDER", &zOrder);
	pScript->SetOrderZ(zOrder);

	ImGui::Text("OPACITY");
	ImGui::SameLine(200);
	float opacity = pScript->GetOpacity();
	ImGui::InputFloat("##UI_OPACITY", &opacity);
	opacity = ClampData(opacity, 0.f, 1.f);
	pScript->SetOpacity(opacity);

	static const char* arrAlignH[] = {"LEFT", "MIDDLE", "RIGHT"};
	ImGui::Text("ALIGNMENT(HORIZONTAL)");
	ImGui::SameLine(200);
	if (ImGui::BeginCombo("##UI_HORIZONTAL_COMBO", arrAlignH[static_cast<UINT>(pScript->GetAnchorH())]))
	{
		for (size_t i = 0; i < std::size(arrAlignH); ++i)
		{
			if (ImGui::Selectable(arrAlignH[i]))
			{
				pScript->SetAnchorH(static_cast<ANCHOR_HORIZONTAL>(i));
			}
		}
		ImGui::EndCombo();
	}

	static const char* arrAlignV[] = {"TOP", "MIDDLE", "BOTTOM"};
	ImGui::Text("ALIGNMENT(VERTICAL)");
	ImGui::SameLine(200);
	if (ImGui::BeginCombo("##UI_VERTICAL_COMBO", arrAlignV[static_cast<UINT>(pScript->GetAnchorH())]))
	{
		for (size_t i = 0; i < std::size(arrAlignV); ++i)
		{
			if (ImGui::Selectable(arrAlignV[i]))
			{
				pScript->SetAnchorH(static_cast<ANCHOR_HORIZONTAL>(i));
			}
		}
		ImGui::EndCombo();
	}
}

void UITool::ShowPanelInfo(CUIPanel* pScript)
{
	ShowUIDefaultInfo(pScript);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[PANEL INFO]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("TEXTURE");
	ImGui::SameLine(200);

	CMeshRender*  pMeshRender = pScript->GetOwner()->MeshRender();
	Ptr<CTexture> pTexture    = pMeshRender->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);

	ImGui::Text((nullptr != pTexture) ? ToString(pTexture->GetKey()).c_str() : "NO TEXTURE");
	ImGui::SameLine(400);
	if (ImGui::Button("##TEXTURE_SELECT", ImVec2{15, 15}))
	{
		// ListUI 활성화한다.
		const std::map<wstring, CRes*>& mapRes  = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		auto                            pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
		pListUI->Clear();
		pListUI->SetTitle("Texture List");
		for (const auto& pair : mapRes)
		{
			pListUI->AddList(ToString(pair.first));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, static_cast<DBCLKED>(&UITool::SelectTexture));
	}
}

void UITool::ShowImageInfo(CUIImage* pScript)
{
	ShowUIDefaultInfo(pScript);

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[IMAGE INFO]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("TEXTURE");
	ImGui::SameLine(200);

	CMeshRender*  pMeshRender = pScript->GetOwner()->MeshRender();
	Ptr<CTexture> pTexture    = pMeshRender->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);

	ImGui::Text((nullptr != pTexture) ? ToString(pTexture->GetKey()).c_str() : "NO TEXTURE");
	ImGui::SameLine(400);
	if (ImGui::Button("##TEXTURE_SELECT", ImVec2{15, 15}))
	{
		// ListUI 활성화한다.
		const std::map<wstring, CRes*>& mapRes  = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		auto                            pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
		pListUI->Clear();
		pListUI->SetTitle("Texture List");
		for (const auto& pair : mapRes)
		{
			pListUI->AddList(ToString(pair.first));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, static_cast<DBCLKED>(&UITool::SelectTexture));
	}

	if (nullptr == pTexture)
	{
		return;
	}

	ImGui::Text("USE WHOLE TEXTURE");
	ImGui::SameLine(200);
	static bool useWholeTexture = true;
	ImGui::Checkbox("##UI_USE_WHOLE", &useWholeTexture);

	if (false == useWholeTexture)
	{
		bool preserveRatio = pScript->IsPreserveRatio();
		ImGui::Text("PRESERVE RATIO", &preserveRatio);
		pScript->SetPreserveRatio(preserveRatio);

		if (pScript->HasCurrentInfo())
		{
			ImGui::Text("CURRENT INFO");
			ImGui::SameLine(200);

			ImGui::Text("CURRENT INFO NAME");
			ImGui::SameLine(200);

			static char szName[256]{};
			_snprintf_s(szName, std::size(szName), "%s", pScript->CurrentInfoName().c_str());
			if (ImGui::InputText("##UI_CURRENT_NAME", szName, std::size(szName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				const tImageInfo info = pScript->GetImageInfo(ToWString(pScript->CurrentInfoName()));
				pScript->RemoveInfo(ToWString(pScript->CurrentInfoName()));
				pScript->AddImageInfo(ToWString(szName), info.imagePos, info.imageSize);
				pScript->SetImageInfo(ToWString(szName));
			}

			ImGui::Text("CURRENT INFO");
			tImageInfo info = pScript->GetImageInfo(ToWString(pScript->CurrentInfoName()));

			ImGui::Text("POS");
			ImGui::SameLine(200);
			ImGui::InputFloat2("##UI_INFO_POS", &info.imagePos.x);
			ImGui::SameLine(200);
			ImGui::Text("SIZE");
			ImGui::SameLine(200);
			ImGui::InputFloat2("##UI_INFO_SIZE", &info.imageSize.x);

			pScript->AddImageInfo(ToWString(pScript->CurrentInfoName()), info.imagePos, info.imageSize);

			if (m_bHasDragArea && ImGui::Button("APPLY POS/SIZE"))
			{
				pScript->AddImageInfo(ToWString(pScript->CurrentInfoName()),
				                      Vec2{m_leftTop.x, m_leftTop.y},
				                      Vec2{m_size.x, m_size.y});
			}
		}

		ImGui::Text("INFO LIST");
		static char szInfo[256]{};
		static int  infoCount = 0;
		for (const auto& [frameName, frameInfo] : pScript->GetInfoList())
		{
			ImGui::Text(ToString(frameName).c_str());
			if (frameName == ToWString(pScript->CurrentInfoName()))
			{
				ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(),
				                                    ImGui::GetItemRectMax(),
				                                    IM_COL32(255, 0, 0, 255));
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				pScript->SetImageInfo(frameName);
			}
			ImGui::SameLine(200);

			tImageInfo info = frameInfo;
			_snprintf_s(szInfo, std::size(szInfo), "##INFO_POS_%d", infoCount);
			ImGui::InputFloat2(szInfo, &info.imagePos.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			_snprintf_s(szInfo, std::size(szInfo), "##INFO_SIZE_%d", infoCount);
			ImGui::InputFloat2(szInfo, &info.imageSize.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			++infoCount;
		}
		infoCount = 0;

		if (ImGui::Button("Add Frame"))
		{
			pScript->AddImageInfo(L"FRAME_" + std::to_wstring(pScript->GetInfoList().size()), Vec2{}, Vec2{});
		}

		ImGui::SameLine(200);
		if (ImGui::Button("Remove Selected Frame"))
		{
			pScript->RemoveInfo(ToWString(pScript->CurrentInfoName()));
		}
	}
}

void UITool::ShowTextInfo(CUIText* pScript)
{
	ShowUIDefaultInfo(pScript);

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[TEXT INFO]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("FONT NAME");
	ImGui::SameLine(200);
	const std::string fontName = pScript->GetFont().empty() ? "NO FONT" : pScript->GetFont();
	ImGui::Text(fontName.c_str());
	ImGui::SameLine(400);
	if (ImGui::Button("##FONT_SELECT", ImVec2{15, 15}))
	{
		// ListUI 활성화한다.
		std::vector<std::wstring> vecFont = CFontMgr::GetInst()->GetFonts();
		auto                      pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
		pListUI->Clear();
		pListUI->SetTitle("Font List");
		for (const auto& font : vecFont)
		{
			pListUI->AddList(ToString(font));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, static_cast<DBCLKED>(&UITool::SelectFontName));
	}

	ImGui::Text("FONT SIZE");
	ImGui::SameLine(200);
	float fontSize = pScript->GetFontSize();
	ImGui::InputFloat("##UI_FONT_SIZE", &fontSize);
	pScript->SetFontSize(fontSize);

	ImGui::Text("FONT COLOR");
	ImGui::SameLine(200);
	Vec4 fontColor = pScript->GetColor();
	ImGui::ColorPicker3("##UI_FONT_COLOR", &fontColor.x);
	pScript->SetColor(fontColor);

	ImGui::Text("ENABLE ALPHA");
	ImGui::SameLine(200);
	bool alphaEanble = pScript->IsAlphaEnable();
	ImGui::Checkbox("##UI_TEXT_ALPHA", &alphaEanble);
	pScript->SetAlphaEnable(alphaEanble);

	static const char* arrAlignH[] = {"LEFT", "MIDDLE", "RIGHT"};
	ImGui::Text("TEXT ALIGNMENT(HORIZONTAL)");
	ImGui::SameLine(200);
	if (ImGui::BeginCombo("##UI_TEXT_ALIGN_HORIZONTAL", arrAlignH[static_cast<UINT>(pScript->GetAlignH())]))
	{
		for (size_t i = 0; i < std::size(arrAlignH); ++i)
		{
			if (ImGui::Selectable(arrAlignH[i]))
			{
				pScript->SetAlignH(static_cast<TEXT_ALIGN_HORIZONTAL>(i));
			}
		}
		ImGui::EndCombo();
	}

	static const char* arrAlignV[] = {"TOP", "MIDDLE", "BOTTOM"};
	ImGui::Text("TEXT ALIGNMENT(VERTICAL)");
	ImGui::SameLine(200);
	if (ImGui::BeginCombo("##UI_TEXT_ALIGN_VERTICAL", arrAlignV[static_cast<UINT>(pScript->GetAlignV())]))
	{
		for (size_t i = 0; i < std::size(arrAlignV); ++i)
		{
			if (ImGui::Selectable(arrAlignV[i]))
			{
				pScript->SetAlignV(static_cast<TEXT_ALIGN_VERTICAL>(i));
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(u8"텍스트");
	ImGui::SameLine(100);

	static char szText[512]{};
	if (ImGui::InputTextMultiline("##INPUT_TEXT",
	                              szText,
	                              std::size(szText),
	                              {0, 0},
	                              ImGuiInputTextFlags_EnterReturnsTrue))
	{
		pScript->SetText(ToWString(szText));
		std::memset(szText, 0, std::size(szText));
	}
}

void UITool::ShowButtonInfo(CUIButton* pScript)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[BUTTON SCRIPT INFO]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void UITool::ShowProgressBarInfo(CUIProgressBar* pScript)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(220.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(150.f / 255.f, 10.f / 255.f, 10.f / 255.f, 1.f));
	ImGui::Button("[PROGRESS BAR SCRIPT INFO]");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("TEXTURE");
	ImGui::SameLine(200);

	CMeshRender*  pMeshRender = pScript->GetOwner()->MeshRender();
	Ptr<CTexture> pTexture    = pMeshRender->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);

	ImGui::Text((nullptr != pTexture) ? ToString(pTexture->GetKey()).c_str() : "NO TEXTURE");
	ImGui::SameLine(400);
	if (ImGui::Button("##TEXTURE_SELECT", ImVec2{15, 15}))
	{
		// ListUI 활성화한다.
		const std::map<wstring, CRes*>& mapRes  = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		auto                            pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
		pListUI->Clear();
		pListUI->SetTitle("Texture List");
		for (const auto& pair : mapRes)
		{
			pListUI->AddList(ToString(pair.first));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, static_cast<DBCLKED>(&UITool::SelectTexture));
	}

	ImGui::Text("DIRECTION");
	ImGui::SameLine(200);
	static const char* arrProgressDir[] = {"RIGHT_TO_LEFT", "LEFT_TO_RIGHT", "TOP_TO_BOTTOM", "BOTTOM_TO_TOP"};
	if (ImGui::BeginCombo("##UI_PROGRESS_BAR_COMBO", arrProgressDir[static_cast<UINT>(pScript->GetDir())]))
	{
		for (size_t i = 0; i < std::size(arrProgressDir); ++i)
		{
			if (ImGui::Selectable(arrProgressDir[i]))
			{
				pScript->SetDir(static_cast<PROGRESS_DIR>(i));
			}
		}
		ImGui::EndCombo();
	}
}
