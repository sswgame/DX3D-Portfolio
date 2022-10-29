#pragma once
#include <Engine/CState.h>

class CDieState :
    public CState
{
public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();


    CLONE(CDieState)
public:
    CDieState();
    CDieState(const CDieState& _origin);
    virtual ~CDieState();
};

