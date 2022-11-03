#pragma once
#include <Engine/CState.h>

class CPlayerDieState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerDieState)
public:
    CPlayerDieState();
    CPlayerDieState(const CPlayerDieState& _origin);
    virtual ~CPlayerDieState();
};

