#include "pch.h"
#include "Homon_Walk.h"

#include "FieldMonsteScript.h"

// engine
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Homon_Walk::Homon_Walk()
	: CState{ L"WALK" }
	, m_pAnimation(nullptr)
	, m_pOwnerMGR(nullptr)
{
}

Homon_Walk::~Homon_Walk()
{
}

void Homon_Walk::Enter()
{
	CState::ResetTimer();

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
		if (mapAnim.find(L"Homon_Walk") == mapAnim.end())
			assert(nullptr && L"Error! Homonculus Anim data is not loaded.");
		m_pAnimation = mapAnim.find(L"Homon_Walk")->second;

		m_pOwnerMGR = (GetOwner()->GetScript<FieldMonsteScript>()->GetMonsterMGR());
	}
	m_pAnimation->SetLerpTime(0.1f);
	GetOwner()->Animator3D()->Play(m_pAnimation->GetName(), true);
}

void Homon_Walk::Update()
{
	CState::Update();

	if (nullptr == m_pAnimation)
		return;

	if (m_pOwnerMGR->IsRunTimeEXIST())
	{
		if (m_pOwnerMGR->GetRunTime() <= CState::GetTimer())
		{
			GetOwner()->GetScript<FieldMonsteScript>()->SetCurAnimationDone();
		}
	}
	else
	{
		float fAnimationLength = m_pAnimation->GetEndTime() - m_pAnimation->GetStartTime();
		if (fAnimationLength <= CState::GetTimer())
		{
			GetOwner()->GetScript<FieldMonsteScript>()->SetCurAnimationDone();
		}
	}
}

void Homon_Walk::Exit()
{
	m_pAnimation->Reset();

}
