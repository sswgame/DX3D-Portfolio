#pragma once
#include <Engine/CState.h>

class  CPlayerIdleState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerIdleState)
public:
    CPlayerIdleState();
    CPlayerIdleState(const CPlayerIdleState& _origin);
    virtual ~CPlayerIdleState();

};

