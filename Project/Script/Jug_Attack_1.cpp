#include "pch.h"
#include "Jug_Attack_1.h"

// [ ENGINE PART ]
#include <Engine/CAnimator3D.h>

Jug_Attack_1::Jug_Attack_1()
	:CState(L"JUG_ATTACK_1")
	, m_pAnim(nullptr)
{
}

Jug_Attack_1::Jug_Attack_1(const Jug_Attack_1& _origin)
	: CState(_origin.m_sStateType)
	, m_pAnim(_origin.m_pAnim)
{
}

Jug_Attack_1::~Jug_Attack_1()
{
}

void Jug_Attack_1::Init()
{
	if (nullptr == m_pAnim)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (mapAnim.find(L"JUG_ATTACK_1") == mapAnim.end())
			assert(nullptr && L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Boss State Script error");

		m_pAnim = mapAnim.find(L"JUG_ATTACK_1")->second;
	}
}

void Jug_Attack_1::Enter()
{
	// IDLE �ִϸ��̼� �÷���
	GetOwner()->Animator3D()->Play(L"JUG_ATTACK_1", false);
}

void Jug_Attack_1::Exit()
{
}

void Jug_Attack_1::Update()
{
	CState::Update();
}

void Jug_Attack_1::LateUpdate()
{
	CState::LateUpdate();
}

