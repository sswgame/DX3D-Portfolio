#include "pch.h"
#include "Homon_Die.h"

#include "FieldMonsteScript.h"
#include "PaperBurnScript.h"

// engine
#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Homon_Die::Homon_Die()
	: CState{L"DIE"}
	, m_pAnimation(nullptr)
	, m_pOwnerMGR(nullptr) {}

Homon_Die::~Homon_Die() {}

void Homon_Die::Enter()
{
	CState::ResetTimer();

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
		if (mapAnim.find(L"Homon_Death01") == mapAnim.end())
			assert(nullptr && L"Error! Homonculus Anim data is not loaded.");
		m_pAnimation = mapAnim.find(L"Homon_Death01")->second;

		m_pOwnerMGR = (GetOwner()->GetScript<FieldMonsteScript>()->GetMonsterMGR());
	}
	GetOwner()->Animator3D()->Play(m_pAnimation->GetName(), false);

	CScript* pPaperburnScript = GetOwner()->GetScript<PaperBurnScript>();

	((PaperBurnScript*)pPaperburnScript)->On();
	((PaperBurnScript*)pPaperburnScript)->SetDir(1.f);
	((PaperBurnScript*)pPaperburnScript)->SetStrength(0.f);
	((PaperBurnScript*)pPaperburnScript)->SetFinishTime(5.f);
	((PaperBurnScript*)pPaperburnScript)->SetColor(Vec4(0.f, 0.5f, 1.f, 1.f));
	GetOwner()->MeshRender()->SetDynamicShadow(false);
}

void Homon_Die::Update()
{
	CState::Update();

	if (nullptr == m_pAnimation)
		return;

	if (GetOwner()->IsDead() == true)
		return;

	if (m_pOwnerMGR->IsRunTimeEXIST())
	{
		if (m_pOwnerMGR->GetRunTime() <= CState::GetTimer())
		{
			GetOwner()->GetScript<FieldMonsteScript>()->SetCurAnimationDone();
			GetOwner()->Destroy();
		}
	}
	else
	{
		float fAnimationLength = (float)(m_pAnimation->GetEndTime() - m_pAnimation->GetStartTime());
		if (fAnimationLength <= CState::GetTimer())
		{
			GetOwner()->GetScript<FieldMonsteScript>()->SetCurAnimationDone();
			GetOwner()->Destroy();
		}
	}
}

void Homon_Die::Exit()
{
	m_pAnimation->Reset();
}
