#pragma once
#include <Engine/CState.h>

class CShield_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CShield_deuxiemie);
	CShield_deuxiemie();
	virtual ~CShield_deuxiemie();
};
