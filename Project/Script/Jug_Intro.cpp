#include "pch.h"
#include "Jug_Intro.h"

// [ ENGINE PART ]
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

Jug_Intro::Jug_Intro()
	: CState(L"JUG_INTRO")
	, m_pAnim(nullptr)
	, m_fPaperBurnStrength(1.f)
{
}

Jug_Intro::Jug_Intro(const Jug_Intro& _origin)
	: CState(_origin.m_sStateType)
{
}

Jug_Intro::~Jug_Intro()
{
}

void Jug_Intro::Init()
{
	if (nullptr == m_pAnim)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (mapAnim.find(L"JUG_NORM_IDLE") == mapAnim.end())
			assert(nullptr && L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Monster State Script error");

		m_pAnim = mapAnim.find(L"JUG_NORM_IDLE")->second;
	}
}

void Jug_Intro::Enter()
{
	// IDLE �ִϸ��̼� �÷���
	GetOwner()->Animator3D()->Play(m_pAnim->GetName(), true);
}

void Jug_Intro::Exit()
{
}

void Jug_Intro::Update()
{
	// paper burn ����
}

void Jug_Intro::LateUpdate()
{
	CState::LateUpdate();
}
