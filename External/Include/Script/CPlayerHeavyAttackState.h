#pragma once
#include <Engine/CState.h>
class CPlayerHeavyAttackState :
    public CState
{

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerHeavyAttackState)
public:
    CPlayerHeavyAttackState();
    CPlayerHeavyAttackState(const CPlayerHeavyAttackState& _origin);
    virtual ~CPlayerHeavyAttackState();

};

