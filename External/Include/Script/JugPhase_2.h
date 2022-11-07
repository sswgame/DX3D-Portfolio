#pragma once
#include <Engine\CState.h>
class JugPhase_2 :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();
    
    CLONE(JugPhase_2)

public:
    JugPhase_2();
    JugPhase_2(const JugPhase_2& _origin);
    virtual ~JugPhase_2();
};

