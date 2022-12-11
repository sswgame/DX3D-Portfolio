#include "pch.h"
#include "NaviMapUI.h"

#include "CImGuiMgr.h"
#include "ListUI.h"

#include <Engine/CNaviMap.h>
#include <Engine/CNaviMapData.h>


NaviMapUI::NaviMapUI()
	: ComponentUI{"NaviMap", COMPONENT_TYPE::NAVIMAP}
{
}

NaviMapUI::~NaviMapUI()
{
}


void NaviMapUI::render_update()
{
	ComponentUI::render_update();
	if (ComponentUI::IsFold())
	{
		SetSize(Vec2(0.f, 25.f));
		return;
	}
	else
	{
		if (GetTargetObject()->GetParent() == nullptr)
			SetSize(Vec2(0.f, 105.f));
		else
			SetSize(Vec2(0.f, 130.f));
	}


	CGameObject*  pTargetObject = GetTargetObject();
	CNaviMap*     pNaviMap      = pTargetObject->NaviMap();
	CNaviMapData* pData         = pNaviMap->GetNaviMapData();

	// 현재 내비맵
	string strNaviMapDataName = "Select NaviMap Data";
	if (nullptr != pData)
		strNaviMapDataName = ToString(pData->GetKey());

	ImGui::Text("NaviMapData");
	ImGui::SameLine(86.f);
	ImGui::InputText("##NaviMapDataName",
	                 (char*)strNaviMapDataName.c_str(),
	                 strNaviMapDataName.capacity(),
	                 ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##NaviMapDataBtn", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const map<wstring, CRes*>& mapRes  = CResMgr::GetInst()->GetResList(RES_TYPE::NAVIMAPDATA);
		ListUI*                    pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("NaviMapData List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(ToString(pair.first));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&NaviMapUI::NaviMapDataSelect);
	}


	// 현재 메쉬
	string strMeshName = "No Mesh";
	if (pData->GetNaviMesh().Get() != nullptr)
		strMeshName = ToString(pData->GetNaviMesh()->GetKey());

	ImGui::Text("Mesh");
	ImGui::SameLine(86.f);
	ImGui::InputText("##MeshName",
	                 (char*)strMeshName.c_str(),
	                 strMeshName.capacity(),
	                 ImGuiInputTextFlags_ReadOnly);
	// Cell 개수
	int iCellCount = pData->GetCellCount();
	ImGui::Text("Cell Count");
	ImGui::SameLine(86.f);
	ImGui::InputInt("##CellCount", &iCellCount, 1, 100, ImGuiInputTextFlags_ReadOnly);

	// Debug Line Color
	//static ImVec4 vLineColor = pNaviMap->GetDebugLineColor();
	//ImGui::Text("Debug Line Color");
	//ImGui::SameLine(86.f);
	//ImGui::ColorEdit4("Debug Line Color##editor",
	//                  (float*)&vLineColor,
	//                  ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview);

	//// Debug Mesh Color
	//static ImVec4 vMesholor = pNaviMap->GetDebugMeshColor();
	//ImGui::Text("Debug Mesh Color");
	//ImGui::SameLine(86.f);
	//ImGui::ColorEdit4("Debug Mesh Color##editor",
	//                  (float*)&vMesholor,
	//                  ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview);

}

void NaviMapUI::NaviMapDataSelect(DWORD_PTR _param)
{
	string  strSelectedName = (char*)_param;
	wstring strKey          = ToWString(strSelectedName);

	CGameObject*      pTargetObject = GetTargetObject();
	CNaviMap*         pNaviMap      = pTargetObject->NaviMap();
	Ptr<CNaviMapData> pData         = CResMgr::GetInst()->FindRes<CNaviMapData>(strKey);
	assert(pData.Get());

	pNaviMap->SetNaviMapData(pData.Get());
}
