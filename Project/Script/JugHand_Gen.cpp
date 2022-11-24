#include "pch.h"
#include "JugHand_Gen.h"

#include "BossJugHandScript.h"

	// Engine
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>

JugHand_Gen::JugHand_Gen()
	: CState{ L"GEN" }
{
}

JugHand_Gen::~JugHand_Gen()
{
}

void JugHand_Gen::Enter()
{
	CState::ResetTimer();

	GetOwner()->Activate();

	CGameObject* pMgrObject = GetOwner()->GetParent();

	CScript* pMgrScript = pMgrObject->GetScript<HandStateMgrScript>();

	if (nullptr == pMgrScript)
		int i = 0;


	m_fLerfTime = 5.f;

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	int iIndex = ((BossJugHandScript*)pScript)->GetHandIndexNumber();
	wstring sAnimName = L"";

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (1 == iIndex)
		{
			sAnimName = L"Hand01_Smashing";
		}
		else if (2 == iIndex)
		{
			sAnimName = L"Hand02_Idle";
		}
		else
		{
			sAnimName = L"Hand03_Idle";
		}

		if (L"" == sAnimName)
			assert(L"Animation Name is not Setting");

		if (mapAnim.find(sAnimName) == mapAnim.end())
			assert(nullptr && L"Hand State Script error");

		m_pAnimation = mapAnim.find(sAnimName)->second;
	}

	sAnimName = m_pAnimation->GetName();

	if (1 == iIndex)
	{
		((BossJugHandScript*)pScript)->SetRunningTime(0.5f);
		GetOwner()->Animator3D()->Play(sAnimName, false);
		m_pAnimation->SetAnimState(ANIMATION_STATE::STOP);
	}
	else
	{
		((BossJugHandScript*)pScript)->SetRunningTime(5.f);
		GetOwner()->Animator3D()->Play(sAnimName, true);
	}
}

void JugHand_Gen::Update()
{
	CState::Update();

	CScript* pScript = GetOwner()->GetScript<BossJugHandScript>();

	if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
	{
		((BossJugHandScript*)pScript)->SetGenStateDone(true);
		return;
	}
}

void JugHand_Gen::Exit()
{
}

