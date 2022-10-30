#pragma once
#include <Engine/CState.h>

class CRun_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CRun_deuxiemie);
	CRun_deuxiemie();
	virtual ~CRun_deuxiemie();
};
