#pragma once
#include <Engine/CState.h>
class CPlayerSprintState :
    public CState
{

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerSprintState)

public:
    CPlayerSprintState();
    CPlayerSprintState(const CPlayerSprintState& _origin);
    virtual ~CPlayerSprintState();

};
