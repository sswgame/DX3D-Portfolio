#pragma once
#include <Engine\CState.h>

class CAnimation3D;

class JugHand_Gen :
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
	CLONE(JugHand_Gen);
	JugHand_Gen();
	virtual ~JugHand_Gen();
};

