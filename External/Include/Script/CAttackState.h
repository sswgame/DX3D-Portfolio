#pragma once
#include <Engine/CState.h>

class CAttackState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CAttackState)
public:
    CAttackState();
    CAttackState(const CAttackState& _origin);
    virtual ~CAttackState();
};

