#pragma once
#include <Engine/CState.h>
class CPlayerParryState :
    public CState
{

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CPlayerParryState)
public:
    CPlayerParryState();
    CPlayerParryState(const CPlayerParryState& _origin);
    virtual ~CPlayerParryState();
};

