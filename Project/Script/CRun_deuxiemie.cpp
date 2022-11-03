#include "pch.h"
#include "CRun_deuxiemie.h"
#include <Engine/CGameObject.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

CRun_deuxiemie::CRun_deuxiemie()
	: CState{L"RUN"}
{
}

CRun_deuxiemie::~CRun_deuxiemie()
{
}

void CRun_deuxiemie::Enter()
{
	GetOwner()->Animator3D()->Play(L"RUN", true);
}

void CRun_deuxiemie::Exit()
{
}
