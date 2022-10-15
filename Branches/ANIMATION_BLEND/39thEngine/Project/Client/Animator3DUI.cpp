#include "pch.h"
#include "Animator3DUI.h"

#include "CImGuiMgr.h"
#include "Animator3D_Tool.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>

#define TO_STRING(wstr) std::string{wstr.begin(),wstr.end()}
#define TO_WSTRING(str) std::wstring{str.begin(),str.end()}

Animator3DUI::Animator3DUI()
	:
	ComponentUI{"Animator3D", COMPONENT_TYPE::ANIMATOR3D} {}

Animator3DUI::~Animator3DUI() {}


void Animator3DUI::render_update()
{
	ComponentUI::render_update();

	RenderCurrentAnimInfo();
	RenderAnimList();
	RenderOpenTool();
}

void Animator3DUI::RenderCurrentAnimInfo()
{
	//1. 현재 가지고 있는 총 클립 표시
	const auto& mapAnim = GetTargetObject()->Animator3D()->GetAnimList();
	ImGui::Text("TOTAL ANIM CLIP COUNT : %d", mapAnim.size());
	//2. 현재 재생 중인 애니메이션 정보 표시
	const auto& pCurrentAnim = GetTargetObject()->Animator3D()->GetCurrentAnim();
	ImGui::Text("CLIP NAME : %s", (nullptr != pCurrentAnim) ? TO_STRING(pCurrentAnim->GetName()).c_str() : "NONE");
	ImGui::Text("TOTAL FRAME COUNT : %d", (nullptr != pCurrentAnim) ? pCurrentAnim->GetAnimInfo().iFrameLength : 0);
	ImGui::Text("DURATION : %.2f", (nullptr != pCurrentAnim) ? pCurrentAnim->GetAnimInfo().dTimeLength : 0.f);
	ImGui::Text("FPS : %d", GetTargetObject()->Animator3D()->GetLeastFrameCount());
}

void Animator3DUI::RenderAnimList()
{
	const auto& mapAnim      = GetTargetObject()->Animator3D()->GetAnimList();
	const auto& pCurrentAnim = GetTargetObject()->Animator3D()->GetCurrentAnim();

	//3. 현재 가진 모든 애니메이션 콤보 박스 표시
	ImGui::Text("ANIM LIST");
	ImGui::SameLine();
	const std::string currentSelectedAnim = (nullptr != pCurrentAnim) ? TO_STRING(pCurrentAnim->GetName()) : "NONE";
	if (ImGui::BeginCombo("##ANIM_LIST", currentSelectedAnim.c_str()))
	{
		auto iter = mapAnim.begin();
		while (iter != mapAnim.end())
		{
			static int selectedIndex{};
			const bool isSelected = (selectedIndex == std::distance(mapAnim.begin(), iter));

			if (ImGui::Selectable(TO_STRING(iter->first).c_str(), isSelected))
			{
				selectedIndex = std::distance(mapAnim.begin(), iter);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
			++iter;
		}
		ImGui::EndCombo();
	}
}

void Animator3DUI::RenderOpenTool()
{
	//애니메이션 툴 오픈
	if (ImGui::Button("OPEN ANIM3D TOOL"))
	{
		const auto pToolUI = static_cast<Animator3D_Tool*>(CImGuiMgr::GetInst()->FindUI("Animator3D_Tool"));
		assert(pToolUI);
		if (false == pToolUI->IsActive())
		{
			pToolUI->SetAnimator3D(GetTargetObject()->Animator3D());
			pToolUI->Activate();
			GetTargetObject()->Animator3D()->GetCurrentAnim()->SetPause(true);
		}
	}
}
