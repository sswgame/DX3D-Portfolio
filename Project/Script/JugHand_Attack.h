#pragma once
#include <Engine\CState.h>

class CAnimation3D;

class JugHand_Attack :
	public CState
{
public:
	CAnimation3D* m_pAnimation;
	float			m_fLerfTime;

	bool			m_bFirstAttackDone;
	bool			m_bSecondAttackDone;
	bool			m_bThirdAttackDone;

	bool		m_bLandingEffectOn;


public:
	void Hand01Attack();
	void Hand02Attack();
	void Hand03Attack();


public:
	void Enter() override;
	void Exit() override;

public:
	void Update() override;


public:
	CLONE(JugHand_Attack);
	JugHand_Attack();
	virtual ~JugHand_Attack();

};

