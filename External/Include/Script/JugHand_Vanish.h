#pragma once
#include <Engine\CState.h>

class CAnimation3D;

class JugHand_Vanish :
	public CState
{
	CAnimation3D* m_pAnimation;
	float			m_fLerfTime;

public:
	void Hand01Gen();
	void Hand02Gen();
	void Hand03Gen();

public:
	void Enter() override;
	void Exit() override;

public:
	void Update() override;


public:
	CLONE(JugHand_Vanish);
	JugHand_Vanish();
	virtual ~JugHand_Vanish();
};

