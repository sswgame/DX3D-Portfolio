#include "pch.h"
#include "CStun_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CStun_deuxiemie::CStun_deuxiemie()
	: CState{L"STUN"}
{
}

CStun_deuxiemie::~CStun_deuxiemie()
{
}


void CStun_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"STUN", true);
}

void CStun_deuxiemie::Exit()
{
}
