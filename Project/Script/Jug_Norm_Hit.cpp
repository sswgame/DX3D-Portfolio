#include "pch.h"
#include "Jug_Norm_Hit.h"

// [ ENGINE PART ]
#include <Engine/CAnimator3D.h>

Jug_Norm_Hit::Jug_Norm_Hit()
	: CState(L"JUG_NORM_HIT")
	, m_pAnim(nullptr)
{

}

Jug_Norm_Hit::Jug_Norm_Hit(const Jug_Norm_Hit& _origin)
	: CState(_origin.m_sStateType)
	, m_pAnim(_origin.m_pAnim)
{
}

Jug_Norm_Hit::~Jug_Norm_Hit()
{
}

void Jug_Norm_Hit::Init()
{
	// �ִϸ��̼� ����
	if (nullptr == m_pAnim)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (mapAnim.find(L"JUG_NORM_HIT") == mapAnim.end())
			assert(nullptr && L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Boss State Script error");

		m_pAnim = mapAnim.find(L"JUG_NORM_HIT")->second;
	}
}

void Jug_Norm_Hit::Enter()
{
	// IDLE �ִϸ��̼� �÷���
	GetOwner()->Animator3D()->Play(L"JUG_NORM_HIT", true);
}

void Jug_Norm_Hit::Exit()
{
}

void Jug_Norm_Hit::Update()
{
	CState::Update();
}

void Jug_Norm_Hit::LateUpdate()
{
	CState::LateUpdate();
}

