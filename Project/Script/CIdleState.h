#pragma once
#include <Engine/CState.h>

class  CIdleState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CIdleState)
public:
    CIdleState();
    CIdleState(const CIdleState& _origin);
    virtual ~CIdleState();

};

