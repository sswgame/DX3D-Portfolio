#pragma once
#include <Engine/CState.h>
#include "CObjKeyMgr.h"
class CStateMgr;
class PlayerScript;


enum class LIGHT_ATTACK_STATE
{
    NONE,

    COMBO_1,
    COMBO_2,
    COMBO_3,
    COMBO_END,


    END,

};

struct LightAttTimer
{
    float   fCombo1_Timer;
    float   fCombo2_Timer;
    float   fCombo3_Timer;

    void Clear()
    {
        fCombo1_Timer = 0.f;
        fCombo2_Timer = 0.f;
        fCombo3_Timer = 0.f;
    }

};

class CPlayerLightAttackState :
    public CState
{
private:
    PlayerScript*       m_pPlayerScript;
    CStateMgr*          m_pStateMgr;
    tKey_Zip            m_tCurKeyInfo;          // ���� Ű ����
    tKey_Zip            m_tPrevKeyInfo;


    LIGHT_ATTACK_STATE  m_eLA_State;            // ����� �޺� ���� ���� 
    LightAttTimer       m_tAttTimer;            // ����� �޺� �� Ÿ�̸�
    CAnimation3D*       m_pCurAnim;

    bool                m_bComboAtt_Start;
    bool                m_bCombo1_2_Success;      // Combo 2 �� �Ѿ� �� �� �ִ��� üũ 
    bool                m_bCombo2_3_Success;      // Combo 3 �� �Ѿ� �� �� �ִ��� üũ 


public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();

    void UpdateLightAttackState();
    void UpdateLightAttaclAnim();

    void PlayLightAttackAnim(wstring _sName, bool _bRepeat, float _fSpeed = 1.f);

    CLONE(CPlayerLightAttackState)

public:
    CPlayerLightAttackState();
    CPlayerLightAttackState(const CPlayerLightAttackState& _origin);
    virtual ~CPlayerLightAttackState();

};

