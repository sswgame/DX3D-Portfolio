#include "pch.h"
#include "ScriptUI.h"

#include <Engine/CScript.h>
#include <Script/CScriptMgr.h>

#include "ParamUI.h"
#include "IconsFontAwesome5.h"
#include "InspectorUI.h"
#include "CImGuiMgr.h"

ScriptUI::ScriptUI()
	:
	UI("Script")
  , m_pTargetObject(nullptr)
  , m_pTargetScript(nullptr)
  , m_eComType(COMPONENT_TYPE::SCRIPT)
  , m_bActive(false) { }

ScriptUI::~ScriptUI() {}

void ScriptUI::update() {}

void ScriptUI::render_update()
{
	// 담당 Script 이름
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(50.f / 255.f, 170.f / 255.f, 50.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(50.f / 255.f, 170.f / 255.f, 50.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(50.f / 255.f, 120.f / 255.f, 50.f / 255.f, 1.f));

	wstring wstrScriptName = CScriptMgr::GetScriptName(m_pTargetScript);
	string strScriptName(wstrScriptName.begin(), wstrScriptName.end());

	string ButtonName;;
	if (m_bFold == false) ButtonName = ICON_FA_CARET_DOWN;
	else ButtonName = ICON_FA_CARET_RIGHT;
	ButtonName += strScriptName;
	if (ImGui::Button(ButtonName.c_str())) { m_bFold = !m_bFold; }

	ImGui::PopStyleColor(3);
	ImGui::PopID();


	// SCRIPT 활성화 체크
	if (m_pTargetScript != nullptr)
	{
		bool IsActive = m_pTargetScript->IsActive();
		ImGui::SameLine();

		if (IsActive)
			ImGui::Checkbox(ICON_FA_EYE, &IsActive);
		else
			ImGui::Checkbox(ICON_FA_EYE_SLASH, &IsActive);

		if (m_pTargetScript->IsActive() != IsActive)
		{
			if (IsActive)
				m_pTargetScript->Activate();
			else
				m_pTargetScript->Deactivate();
		}

	}
	


	// [ SCRIPT DELETE BUTTON ]
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_TRASH))
			ImGui::OpenPopup(u8"SCRIPT 삭제 경고창");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal(u8"SCRIPT 삭제 경고창", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			string TargetObjName = ToString(m_pTargetObject->GetName());
			string text = TargetObjName + " / " + strScriptName;
			ImGui::Text(text.c_str());
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f)
				, u8"해당 SCRIPT를 정말로 삭제하시겠습니까?\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				tUIDelegate tDeleteCom;
				tDeleteCom.dwParam = (DWORD_PTR)m_pTargetScript;
				tDeleteCom.pFunc = (PARAM_1)&InspectorUI::DeleteScript;
				tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("Inspector");

				CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}


	const vector<tScriptParamInfo>& vecParam = m_pTargetScript->GetScriptParam();
	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].eType)
		{
		case SCRIPTPARAM_TYPE::INT:
			{
				int data = ParamUI::Param_Int(vecParam[i].strParamName, (const int*)vecParam[i].pParam);
				*(int*)vecParam[i].pParam = data;
			}

			break;
		case SCRIPTPARAM_TYPE::FLOAT:
			{
				float data = ParamUI::Param_Float(vecParam[i].strParamName, (const float*)vecParam[i].pParam);
				*(float*)vecParam[i].pParam = data;
			}

			break;
		case SCRIPTPARAM_TYPE::VEC2:
			{
				Vec2 data = ParamUI::Param_Vec2(vecParam[i].strParamName, (const Vec2*)vecParam[i].pParam);
				*(Vec2*)vecParam[i].pParam = data;
			}
			break;
		case SCRIPTPARAM_TYPE::VEC4:
			{
				Vec4 data = ParamUI::Param_Vec4(vecParam[i].strParamName, (const Vec4*)vecParam[i].pParam);
				*(Vec4*)vecParam[i].pParam = data;
			}
			break;

		case SCRIPTPARAM_TYPE::TEX:
			break;
		case SCRIPTPARAM_TYPE::PREFAB:
			break;
		}
	}
}
