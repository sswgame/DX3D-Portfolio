#include "pch.h"
#include "Jug_Dead.h"

// [ ENGINE PART ]
#include <Engine/CAnimator3D.h>

Jug_Dead::Jug_Dead()
	: CState(L"JUG_DEAD")
	, m_pAnim(nullptr)
{
}

Jug_Dead::Jug_Dead(const Jug_Dead& _origin)
	: CState(_origin.m_sStateType)
{
}

Jug_Dead::~Jug_Dead()
{
}

void Jug_Dead::Init()
{
	if (nullptr == m_pAnim)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (mapAnim.find(L"JUG_DEAD") == mapAnim.end())
			assert(nullptr, L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Boss State Script error");

		m_pAnim = mapAnim.find(L"JUG_DEAD")->second;
	}
}

void Jug_Dead::Enter()
{
	// IDLE �ִϸ��̼� �÷���
	GetOwner()->Animator3D()->Play(L"JUG_DEAD", true);
}

void Jug_Dead::Exit()
{
}

void Jug_Dead::Update()
{
	CState::Update();
}

void Jug_Dead::LateUpdate()
{
	CState::LateUpdate();
}

