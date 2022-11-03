#include "pch.h"
#include "CWalk_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CWalk_deuxiemie::CWalk_deuxiemie()
	: CState{L"WALK"}
{
}

CWalk_deuxiemie::~CWalk_deuxiemie()
{
}

void CWalk_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"WALK", true);
}

void CWalk_deuxiemie::Exit()
{
}
