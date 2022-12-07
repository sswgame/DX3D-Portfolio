#include "pch.h"
#include "Deux_Hit.h"

#include "FieldMonsteScript.h"

// engine
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Deux_Hit::Deux_Hit()
	: CState{ L"HIT" }
	, m_pAnimation(nullptr)
	, m_pOwnerMGR(nullptr)
{
}

Deux_Hit::~Deux_Hit()
{
}

void Deux_Hit::Enter()
{
	CState::ResetTimer();

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
		if (mapAnim.find(L"Deux_Hit") == mapAnim.end())
			assert(nullptr && L"Error! Deuxiemie Anim data is not loaded.");
		m_pAnimation = mapAnim.find(L"Deux_Hit")->second;

		m_pOwnerMGR = (GetOwner()->GetScript<FieldMonsteScript>()->GetMonsterMGR());
	}
	GetOwner()->Animator3D()->Play(m_pAnimation->GetName(), false);
}

void Deux_Hit::Update()
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
			GetOwner()->Animator3D()->Play(L"Deux_Idle", true);
		}
	}
}

void Deux_Hit::Exit()
{
	m_pAnimation->Reset();
}
