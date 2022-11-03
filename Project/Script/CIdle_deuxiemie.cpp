#include "pch.h"
#include "CIdle_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>


CIdle_deuxiemie::CIdle_deuxiemie()
	: CState{L"IDLE"}
{
}

CIdle_deuxiemie::~CIdle_deuxiemie()
{
}

void CIdle_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"IDLE", true);
}

void CIdle_deuxiemie::Exit()
{
}
