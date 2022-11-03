#pragma once
#include <Engine/CState.h>

class CDeath_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CDeath_deuxiemie);
	CDeath_deuxiemie();
	virtual ~CDeath_deuxiemie();
};
