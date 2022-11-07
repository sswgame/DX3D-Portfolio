#pragma once
#include <Engine\CState.h>
class JugPhase_Dead :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();

    CLONE(JugPhase_Dead)

public:
    JugPhase_Dead();
    JugPhase_Dead(const JugPhase_Dead& _origin);
    virtual ~JugPhase_Dead();
};

