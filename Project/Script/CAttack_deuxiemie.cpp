#include "pch.h"
#include "CAttack_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CAttack_deuxiemie::CAttack_deuxiemie()
	: CState{L"ATTACK"}
{
}

CAttack_deuxiemie::~CAttack_deuxiemie()
{
}

void CAttack_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"ATTACK", true);
}

void CAttack_deuxiemie::Exit()
{
}
