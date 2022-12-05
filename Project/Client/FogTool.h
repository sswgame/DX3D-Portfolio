#pragma once
#include "UI.h"

class CGameObject;

class FogTool :
    public UI
{
private:
    CGameObject* m_pFog;
    bool m_bFogOn;

public:
    void SetData();

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    FogTool();
    ~FogTool();
};

