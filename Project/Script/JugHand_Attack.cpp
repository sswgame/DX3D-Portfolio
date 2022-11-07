#include "pch.h"
#include "JugHand_Attack.h"

// [ ENGINE PART ]
#include <Engine/CAnimator3D.h>

JugHand_Attack::JugHand_Attack()
	: CState(L"JUG_HAMD_ATTACK")
	, m_pAnim(nullptr)
{
}

JugHand_Attack::JugHand_Attack(const JugHand_Attack& _origin)
	: CState(_origin.m_sStateType)
	, m_pAnim(_origin.m_pAnim)
{
}

JugHand_Attack::~JugHand_Attack()
{
}

void JugHand_Attack::Init()
{
	// �ִϸ��̼� ����
	if (nullptr == m_pAnim)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (mapAnim.find(L"JUG_HAMD_ATTACK") == mapAnim.end())
			assert(nullptr, L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Boss State Script error");

		m_pAnim = mapAnim.find(L"JUG_HAMD_ATTACK")->second;
	}
}

void JugHand_Attack::Enter()
{
	// IDLE �ִϸ��̼� �÷���
	GetOwner()->Animator3D()->Play(L"JUG_HAMD_ATTACK", true);
}

void JugHand_Attack::Exit()
{
}

void JugHand_Attack::Update()
{
	CState::Update();
}

void JugHand_Attack::LateUpdate()
{
	CState::LateUpdate();
}