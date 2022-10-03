#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>
#include "IconsFontAwesome5.h"
#include "CImGuiMgr.h"


ComponentUI::ComponentUI(const string& _strName, COMPONENT_TYPE _eComType)
	: UI(_strName)
	, m_eComType(_eComType)
	, m_bActive(false)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::update()
{
	if (nullptr == m_pTargetObject)
		return;

	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);
	m_bActive = pComponent->IsActive();
}

void ComponentUI::render_update()
{	
	// 담당 Component 이름
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(ToString(m_eComType).c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Component 활성화 체크
	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);	

	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)
	{
		bool IsActive = pComponent->IsActive();
		ImGui::SameLine();
		//ImGui::Checkbox("##ComponentActive", &IsActive);
		if(IsActive)
			ImGui::Checkbox(ICON_FA_EYE, &IsActive);
		else 
			ImGui::Checkbox(ICON_FA_EYE_SLASH, &IsActive);

		if (pComponent->IsActive() != IsActive)
		{
			if (IsActive)
				pComponent->Activate();			
			else
				pComponent->Deactivate();			
		}
	}

	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)
	{
		ImGui::SameLine();
		// Component Delete Buttton
		if (ImGui::Button(ICON_FA_TRASH))
		{
			m_bDelete = true;
		}
		if (m_bDelete)
		{
			ImGui::OpenPopup("ReallyDelete?");
			bool unused_open = true;
			if (ImGui::BeginPopupModal("ReallyDelete?", &unused_open))
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f)
					, "%s WARNING \n\nAre You sure you want to delete this Component? \n\n", ICON_FA_EXCLAMATION);
				if (ImGui::Button(ICON_FA_CIRCLE))
				{
					m_bDelete = false;
					/*
					* // CImGuiMgr 에 Delegate 등록
					tUIDelegate tDeleteCom;
					tDeleteCom.dwParam = (DWORD_PTR)m_eComType;
					tDeleteCom.pFunc = (PARAM_1)&InspectorUI::DeleteComponent;
					tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("Inspector");

					CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
					*/
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_XMARK))
				{
					m_bDelete = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
	


}


