#pragma once
#include <Engine/CState.h>

class CPlayerHitState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerHitState)
public:
    CPlayerHitState();
    CPlayerHitState(const CPlayerHitState& _origin);
    virtual ~CPlayerHitState();
};

