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

void JugHand_Gen::Hand01Gen()
{
}

void JugHand_Gen::Hand02Gen()
{
}

void JugHand_Gen::Hand03Gen()
{
}

void JugHand_Gen::Enter()
{
	CState::ResetTimer();

	CScript* pMgrScript = GetOwner()->GetScript<HandStateMgrScript>();

	m_fLerfTime = 1.f;

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	int iIndex = ((BossJugHandScript*)pScript)->GetHandIndexNumber();
	wstring sAnimName = L"";

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (1 == iIndex)
		{
			sAnimName = L"Hand01_Idle";
		}
		else if (2 == iIndex)
		{
			sAnimName = L"Hand02_Idle";
		}
		else
		{
			sAnimName = L"Hand03_Idle";
		}

		if (mapAnim.find(sAnimName) == mapAnim.end())
			assert(nullptr && L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Hand State Script error");

		m_pAnimation = mapAnim.find(sAnimName)->second;
	}
	((BossJugHandScript*)pScript)->SetRunningTime(m_fLerfTime);
	GetOwner()->Animator3D()->Play(sAnimName, false);
}

void JugHand_Gen::Update()
{
	CState::Update();

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();

	if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
	{
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
		return;
	}
}

void JugHand_Gen::Exit()
{
	GetOwner()->GetScript<HandStateMgrScript>()->SetGenDone(true);
}

