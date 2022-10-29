#pragma once
#include <Engine/CState.h>

class CHitState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CHitState)
public:
    CHitState();
    CHitState(const CHitState& _origin);
    virtual ~CHitState();
};

