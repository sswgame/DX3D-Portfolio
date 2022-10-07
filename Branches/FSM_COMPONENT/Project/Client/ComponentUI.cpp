#include "pch.h"
#include "ComponentUI.h"

// ENGINE
#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>


// CLIENT
#include "IconsFontAwesome5.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"


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
	// ��� Component �̸�
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(ToString(m_eComType).c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Component Ȱ��ȭ üũ
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

	// [ COMPONENT DELETE BUTTON ]
	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)
	{
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_TRASH))
			ImGui::OpenPopup(u8"COMPONENT ���� ���â");
		
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal(u8"COMPONENT ���� ���â", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			string TargetObjName = ToString(m_pTargetObject->GetName());
			string ComponentName = ToString(m_eComType);
			string text = TargetObjName + " / " + ComponentName + " COMPONENT";
			ImGui::Text(text.c_str());
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f)
				, u8"�ش� COMPONENT�� ������ �����Ͻðڽ��ϱ�?\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) 
			{ 
				tUIDelegate tDeleteCom;
				tDeleteCom.dwParam = (DWORD_PTR)m_eComType;
				tDeleteCom.pFunc = (PARAM_1)&InspectorUI::DeleteComponent;
				tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("Inspector");

				CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}
	


}


