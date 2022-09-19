#pragma once
#include "CRenderComponent.h"
class CLandScape :
    public CRenderComponent
{
private:
    UINT    m_iXFaceCount;
    UINT    m_iZFaceCount;

public:
    virtual void finalupdate() override;
    virtual void render() override;
    

public:
    void SetFaceCount(UINT _iX, UINT _iZ) 
    {
        m_iXFaceCount = _iX;
        m_iZFaceCount = _iZ;
    }

    void Create();

public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

