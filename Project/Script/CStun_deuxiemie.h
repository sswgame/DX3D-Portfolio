#pragma once
#include <Engine/CState.h>

class CStun_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CStun_deuxiemie);
	CStun_deuxiemie();
	virtual ~CStun_deuxiemie();
};
