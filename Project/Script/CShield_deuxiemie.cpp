#include "pch.h"
#include "CShield_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CShield_deuxiemie::CShield_deuxiemie()
	: CState{L"SHIELD"}
{
}

CShield_deuxiemie::~CShield_deuxiemie()
{
}


void CShield_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"SHIELD", true);
}

void CShield_deuxiemie::Exit()
{
}
