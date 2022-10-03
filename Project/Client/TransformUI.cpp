#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	:
	ComponentUI("Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(Vec2(0.f, 100.f));
}

TransformUI::~TransformUI() {}

void TransformUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;

	CTransform* pTransform = pTargetObj->Transform();

	m_vRelativePos   = pTransform->GetRelativePos();
	m_vRelativeScale = pTransform->GetRelativeScale();
	m_vRelativeRot   = pTransform->GetRelativeRotation();
}

void TransformUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CTransform*  pTrans        = pTargetObject->Transform();
	Vec3         vPos          = pTrans->GetRelativePos();
	Vec3         vScale        = pTrans->GetRelativeScale();
	Vec3         vRot          = pTrans->GetRelativeRotation();
	vRot.ToDegree();


	ImGui::PushItemWidth(200); // Float3 ���� ���� ����

	/*ImGui::Text("Relative Position", 100.f);
	ImGui::SameLine();
	ImGui::SliderFloat3("##Position", vPos, 0.5f);
	pTrans->SetRelativePos(vPos);

	ImGui::Text("Relative Scale     ", 100.f);
	ImGui::SameLine();
	ImGui::InputFloat3("##Scale", vScale);
	pTrans->SetRelativeScale(vScale);

	ImGui::Text("Relative Rotation", 100.f);
	ImGui::SameLine();
	ImGui::InputFloat3("##Rotation", vRot);
	vRot.ToRadian();
	pTrans->SetRelativeRotation(vRot);*/


	
	ImGui::Text("Relative Position", 100.f);
	ImGui::SameLine();
	//ImGui::InputFloat3("##Position", vPos);

	ImGui::TextColored(ImVec4(1.f, 0.f ,0.f, 1.f),"X"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##LeftTop X_drag", &vPos.x);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##LeftTop Y_drag", &vPos.y);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##LeftTop Z_drag", &vPos.z);
	ImGui::PopItemWidth();
	pTrans->SetRelativePos(vPos);


	ImGui::Text("Relative Scale     ", 100.f);
	ImGui::SameLine();
	//ImGui::InputFloat3("##Scale", vScale);

	ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "X"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Scale X_drag", &vScale.x);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Scale Y_drag", &vScale.y);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Scale Z_drag", &vScale.z);
	ImGui::PopItemWidth();
	pTrans->SetRelativeScale(vScale);


	ImGui::Text("Relative Rotation", 100.f);
	ImGui::SameLine();
	//ImGui::InputFloat3("##Rotation", vRot);

	ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "X"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Rotation X_drag", &vRot.x);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Rotation Y_drag", &vRot.y);
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(0.f);
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Rotation Z_drag", &vRot.z);
	ImGui::PopItemWidth();
	vRot.ToRadian();
	pTrans->SetRelativeRotation(vRot);

	//if (nullptr != pTargetObject->GetParent())
	//{
	//	m_bIgnorantParentScale = pTrans->GetIgnoreParentScale();
	//	ImGui::Checkbox("IgnoreParentScale", &m_bIgnorantParentScale);
	//	{
	//		pTrans->SetIgnoreParentScale(m_bIgnorantParentScale);
	//	}
	//}
}
