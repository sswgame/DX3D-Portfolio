#include "pch.h"
#include "CDeath_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CDeath_deuxiemie::CDeath_deuxiemie()
	: CState{L"DEATH"}
{
}

CDeath_deuxiemie::~CDeath_deuxiemie()
{
}

void CDeath_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"DEATH", true);
}

void CDeath_deuxiemie::Exit()
{
}
