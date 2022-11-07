#pragma once
#include <Engine\CState.h>

class CAnimation3D;

class JugHand_Attack :
    public CState
{
private:
    CAnimation3D* m_pAnim;

public:
    virtual void Init();
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();

    CLONE(JugHand_Attack)
public:
    JugHand_Attack();
    JugHand_Attack(const JugHand_Attack& _origin);
    virtual ~JugHand_Attack();
};

