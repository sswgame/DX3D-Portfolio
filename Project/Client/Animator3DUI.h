#pragma once
#include "ComponentUI.h"
#include "Anim3DSequencer.h"

class CAnimator3D;
class CAnimation3D;

class Animator3DUI :
    public ComponentUI
{
private:
    CAnimator3D*        m_pAnimator3D;
    CAnimation3D*       m_pCurAnim3D;
    Anim3DSequencer     m_FrameTimeLine;


private:
    int                 m_iSelectedIdx;
    int                 m_iSelectedPrevIdx;
    int                 m_iCurFrameUI;
    float               m_fCommonLerpTime; // ��� �ִϸ��̼ǿ� ������ LerpTime
    bool                m_bPlay;

public:
    // delegate 
    void Reset(DWORD_PTR _ptr);

public:
    void Clear();
public:
    void RenderComponentWindow();
    void RenderAnim3DClipWindow();

public:
    CAnimator3D* GetCurAnimator3D() { return m_pAnimator3D; }
    CAnimation3D* GetNextAnim();
    void ApplyAnim3DToChild(CGameObject* pObj);


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    Animator3DUI();
    ~Animator3DUI();

};


