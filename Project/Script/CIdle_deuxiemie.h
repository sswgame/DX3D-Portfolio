#pragma once
#include <Engine/CState.h>

class CIdle_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CIdle_deuxiemie);
	CIdle_deuxiemie();
	virtual ~CIdle_deuxiemie();
};
