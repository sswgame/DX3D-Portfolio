#include "pch.h"
#include "Homon_Attack.h"

#include "FieldMonsteScript.h"

// engine
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Homon_Attack::Homon_Attack()
	: CState{ L"ATTACK" }
	, m_pAnimation(nullptr)
	, m_pOwnerMGR(nullptr)
{
}

Homon_Attack::~Homon_Attack()
{
}

void Homon_Attack::Enter()
{
	CState::ResetTimer();

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
		if (mapAnim.find(L"Homon_Attack2") == mapAnim.end())
			assert(nullptr && L"Error! Homonculus Anim data is not loaded.");
		m_pAnimation = mapAnim.find(L"Homon_Attack2")->second;

		m_pOwnerMGR = (GetOwner()->GetScript<FieldMonsteScript>()->GetMonsterMGR());
	}
	GetOwner()->Animator3D()->Play(m_pAnimation->GetName(), false);
}


void Homon_Attack::Update()
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
			GetOwner()->Animator3D()->Play(L"Homon_Idle", true);
			m_pOwnerMGR->SetRunTime(1.f);
		}
	}
}

void Homon_Attack::Exit()
{
	m_pAnimation->Reset();
}
