#pragma once
#include <Engine/CState.h>
#include "CObjKeyMgr.h"
#include "CTranslateMgr.h"

class CStateMgr;

// JUMP 
// Up -> Air -> Down 
enum class JUMP_STATE
{
    NONE,

    FIRST_JUMP,     // ù��° ����
    SECOND_JUMP,    // �ι��� ���� (���� ����)
    LIMIT_JUMP,     // ���� Ƚ�� ���� 

    JUMP_UP,
    JUMP_AIR,
    JUMP_DOWN,

    END,
};

class CPlayerJumpState :
    public CState
{
private:
    CTranslateMgr*  m_pTranslateMgr; // �̵� ���� �Ŵ���
    CStateMgr*      m_pStateMgr;


private:
    // [ ���� ���� ���� ]
    bool             m_bJumpAir_AnimStart;
    bool             m_bJumpDown_AnimStart;

    float m_fPrevHeight;          // �÷��̾��� ���� y �� 
    float m_fCurHeight;           // �÷��̾��� ���� y �� 
    float m_fCurHeightDir;

    JUMP_STATE       m_ePrevState;
    JUMP_STATE       m_eCurState;

    tKey_Zip         m_tPrevKeyInfo;          // ���� Ű ����
    tKey_Zip         m_tCurKeyInfo;          // ���� Ű ����


    JUMP_STATE       m_eJumpState;
    int              m_iJumpCnt;        // ���� Ƚ��  

    CGameObject*    m_pJumpEffect_Land;



public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();

    void  PlayJumpAnim(wstring _sName, bool _bRepeat = true, float _fSpeed = 1.f);
    void  SetCurKeyInfo(tKey_Zip _keyInfo) { m_tCurKeyInfo = _keyInfo; }
    float GetHeightDir() { return m_fCurHeightDir; }
public:
    void Jump(Vec3 _velocity);
    void ResetForDoubleJump();

    void UpdateJump();
    void UpdateJumpState();
    void UpdateJumpAnimation();


    CLONE(CPlayerJumpState)
public:
    CPlayerJumpState();
    CPlayerJumpState(const CPlayerJumpState& _origin);
    virtual ~CPlayerJumpState();

};

