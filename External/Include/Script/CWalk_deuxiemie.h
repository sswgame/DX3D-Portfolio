#pragma once
#include <Engine/CState.h>

class CWalk_deuxiemie : public CState
{
public:
	void Enter() override;
	void Exit() override;

public:
	CLONE(CWalk_deuxiemie);
	CWalk_deuxiemie();
	virtual ~CWalk_deuxiemie();
};
