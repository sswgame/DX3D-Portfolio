#pragma once
#include <Engine/CState.h>

class CAttack_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CAttack_deuxiemie);
	CAttack_deuxiemie();
	virtual ~CAttack_deuxiemie();
};
