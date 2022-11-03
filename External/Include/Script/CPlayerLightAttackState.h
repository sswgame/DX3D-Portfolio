#pragma once
#include <Engine/CState.h>
class CPlayerLightAttackState :
    public CState
{

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerLightAttackState)

public:
    CPlayerLightAttackState();
    CPlayerLightAttackState(const CPlayerLightAttackState& _origin);
    virtual ~CPlayerLightAttackState();

};

