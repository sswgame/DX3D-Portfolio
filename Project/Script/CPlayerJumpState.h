#pragma once
#include <Engine/CState.h>
class CPlayerJumpState :
    public CState
{

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerJumpState)
public:
    CPlayerJumpState();
    CPlayerJumpState(const CPlayerJumpState& _origin);
    virtual ~CPlayerJumpState();

};

