#pragma once
#include <Engine/CEntity.h>
#include <Engine/CKeyMgr.h>

// 1. �������� ������Ʈ �ű��
// 2. ������Ʈ ��Ȯ�� 

/*
* PLAYER 
* 
    [ Moving ]
Move Forward            - W
Move Backward           - S
Move Left               - A
Move Right              - D

Jump                    - Space
Interact                - E
Sprint                  - Left Shift
Walk                    - Left Alt

    [ Combat ]
Light Attack            - Mouse Left Click
Heavy Attack            - Mouse Right Click
Parry                   - Q
Camera lock             - Middle Mouse button

* UI
* 
    [ Shortcuts ]
Previous Quick Access   - 1
Next Quick Access       - 2
Spell Switch            - 3
Use Quick Access        - F

    [ Menu ]
Show HUD                - H
Quick Access            - I

*/

enum PLAYER_KEY_OPTION
{
    // Key State 
    NONE            = 0,            // 0000 0000 0000 0000
    TAP             = 1 << 0,       // 0000 0000 0000 0001
    PRESSED         = 1 << 1,       // 0000 0000 0000 0010
    AWAY            = 1 << 2,       // 0000 0000 0000 0100

    // Moving  Key
    FORWARD         = 1 << 4,       // 0000 0000 0001 0000
    BACKWARD        = 1 << 5,       // 0000 0000 0010 0000
    LEFT            = 1 << 6,       // 0000 0000 0100 0000
    RIGHT           = 1 << 7,       // 0000 0000 1000 0000

    JUMP            = 1 << 8,       // 0000 0001 0000 0000
    INTERACT        = 1 << 9,       // 0000 0010 0000 0000
    SPRINT          = 1 << 10,      // 0000 0100 0000 0000
    WALK            = 1 << 11,      // 0000 1000 0000 0000

    // Combat Key 
    LIGHT_ATTACK    = 1 << 12,       // 0001 0000 0000 0000
    HEAVY_ATTACK    = 1 << 13,       // 0010 0000 0000 0000
    PARRY           = 1 << 14,       // 0100 0000 0000 0000
    CAMERA_LOCK     = 1 << 15,       // 1000 0000 0000 0000

    END             = 1 << 16,
};

// unsigned short - 16 bit flags [ 0000 0000 0000 0000 ]
typedef unsigned int KeyOptionFlags; // 32 bit flags [ 0000 0000 0000 0000 ] [ 0000 0000 0000 0000 ]

struct tMovingKey
{
    KEY eForward;
    KEY eBackWard;
    KEY eLeft;
    KEY eRight;

    KEY eJump;
    KEY eInteract;
    KEY eSprint;
    KEY eWalk;
};

struct tCombatKey
{
    KEY eLightAttack;
    KEY eHeavyAttack;
    KEY eParry;
    KEY eCameraLock;
};

struct tKey_Zip
{
    KEY_STATE           eKeyState;
    KEY                 eKey;    
    KeyOptionFlags      iKeyFlags;

};


class CObjKeyMgr
    : public CEntity
{
private:
    CScript*            m_pOwnerScript;

    tMovingKey          m_tMovingKey;       // �̵� ���� Ű ���� 
    tCombatKey          m_tCombatKey;       // ���� ���� Ű ���� 
 
    tKey_Zip            m_tPrevKeyInfo;     // ������ ���� Ű���� ����
    tKey_Zip            m_tCurKeyInfo;      // ����   ���� Ű���� ���� 


public:
    void	update();
    void	lateupdate();

public:
    void Init();
    void CheckKeyTap();
    void CheckKeyPressed();
    void CheckKeyAway();

public:
    // MOVING KEY SETTING
    void SetMovingForwardKey(KEY _eKey)          { m_tMovingKey.eForward     = _eKey; }
    void SetMovingBackwardKey(KEY _eKey)         { m_tMovingKey.eBackWard    = _eKey; }
    void SetMovingLeftKey(KEY _eKey)             { m_tMovingKey.eLeft        = _eKey; }
    void SetMovingRightKey(KEY _eKey)            { m_tMovingKey.eRight       = _eKey; }
    void SetMovingJumpKey(KEY _eKey)             { m_tMovingKey.eJump        = _eKey; }
    void SetMovingInteractKey(KEY _eKey)         { m_tMovingKey.eInteract    = _eKey; }
    void SetMovingSprintKey(KEY _eKey)           { m_tMovingKey.eSprint      = _eKey; }
    void SetMovingWalkKey(KEY _eKey)             { m_tMovingKey.eWalk        = _eKey; }

    // COMBAT KEY SETTING
    void SetCombatLightAttackKey(KEY _eKey)      { m_tCombatKey.eLightAttack = _eKey; }
    void SetCombatHeavyAttackKey(KEY _eKey)      { m_tCombatKey.eHeavyAttack = _eKey; }
    void SetCombatParryKey(KEY _eKey)            { m_tCombatKey.eParry       = _eKey; }
    void SetCombatCameraLockKey(KEY _eKey)       { m_tCombatKey.eCameraLock  = _eKey; }

    void SetOwnerScript(CScript* _pOwner) { m_pOwnerScript = _pOwner; }


public:
    tMovingKey GetMovingKey()                   { return m_tMovingKey; }
    tCombatKey GetCombatKey()                   { return m_tCombatKey; }
    KeyOptionFlags  GetKeyOptionFlags()          { return m_tCurKeyInfo.iKeyFlags; }
    KEY_STATE       GetCurKeyState()                { return m_tCurKeyInfo.eKeyState; }
    KEY             GetCurKey()                     { return m_tCurKeyInfo.eKey; }


    tKey_Zip GetCurKeyInfo() { return m_tCurKeyInfo; }
    tKey_Zip GetPrevKeyInfo() { return m_tPrevKeyInfo; }

    KEY_STATE GetCurKeyStateByFlags();
    KEY GetCurKeyByFlags();


    
    CLONE(CObjKeyMgr)
public:
    CObjKeyMgr();
    CObjKeyMgr(const CObjKeyMgr& _origin);
    virtual ~CObjKeyMgr();


};

