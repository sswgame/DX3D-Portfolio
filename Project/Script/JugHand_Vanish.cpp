#include "pch.h"
#include "JugHand_Vanish.h"

#include "BossJugHandScript.h"

// Engine
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>

JugHand_Vanish::JugHand_Vanish()
	: CState{ L"VANISH" }
{
}

JugHand_Vanish::~JugHand_Vanish()
{
}

void JugHand_Vanish::Hand01Gen()
{
}

void JugHand_Vanish::Hand02Gen()
{
}

void JugHand_Vanish::Hand03Gen()
{
}

void JugHand_Vanish::Enter()
{
	CState::ResetTimer();

	m_fLerfTime = 2.f;

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
			sAnimName = L"Hand02_Pattern2";
		}
		else
		{
			sAnimName = L"Hand03_Pattern3";
		}

		if (mapAnim.find(sAnimName) == mapAnim.end())
			assert(nullptr, L"애니메이션 클립을 찾을 수 없습니다. \n Hand State Script error");

		m_pAnimation = mapAnim.find(sAnimName)->second;
	}

	((BossJugHandScript*)pScript)->SetRunningTime(2.f);

	// Vanish는 공격 모션 마지막 Frame 을 계속 유지한다.
	GetOwner()->Animator3D()->Play(sAnimName, false);
	m_pAnimation->SetCurFrameIdx(m_pAnimation->GetEndFrameIdx());
	//m_pAnimation->SetCurFrameIdx(m_pAnimation->GetSavedFrameIdx());

}

void JugHand_Vanish::Update()
{
	CState::Update();

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	int iIndex = ((BossJugHandScript*)pScript)->GetHandIndexNumber();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
	{
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);

		if (1 == iIndex)
		{
			GetOwner()->Destroy();
		}
		else if (2 == iIndex)
		{
			GetOwner()->Destroy();
		}
		else
		{
			// 보스 머리 위로 이동.
			// vPos.x = 위치값;
			// vPos.z = 위치값;
		}

		return;
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
}

void JugHand_Vanish::Exit()
{
	// vanish 는 attack 이 완전히 끝나야 true 가 된다.
	if (true == GetOwner()->GetScript<Hand_StateMgr>()->GetAllAttackDone())
	{
		GetOwner()->GetScript<Hand_StateMgr>()->SetCurAttackNumber(-1);
		GetOwner()->GetScript<Hand_StateMgr>()->SetVanishDone(true);
	}

}
