#pragma once
#include <Engine/CScript.h>

enum class CAMERA_MODE
{
    FIRST_PERSON,   // 1��Ī
    THIRD_PERSON,   // 3��Ī   
    FREE,

    END,
};

enum class CAMERA_OPTION
{
    DEFAULT,
    CAMERA_LOCK,    // Ư�� ��ġ ���� ���� 

    END,
};

class PlayerCamScript :
    public CScript
{
private:
    bool m_bFocus_Cursor = false;

private:
    CGameObject*    m_pCam;                 // ī�޶�
    CAMERA_MODE     m_eCamMode;             // ī�޶� �Կ� ��� ���� 
    CAMERA_OPTION   m_eCamOption;           // ī�޶� �Կ� �ɼ� ���� 
    CGameObject*    m_pPlayer;

private:
    Vec3            m_vPos;                 // ī�޶��� ��ġ 
    Vec3            m_vTargetPos;           // 3��Ī ( ������ �� Target ��ġ ) - [Player ��ġ] 
    Vec3            m_vPrevTargetPos;
    Vec3            m_vTargetPosAcc;        // ���� ī�޶� ��ġ�� Ÿ�� ��ġ���� �Ÿ� ���� - �Ÿ����̸�ŭ ������ �� ( �ð������� �ڿ������� )

    float           m_fCamHeight;           // ī�޶� [����] ���� - Player ���� ��� ���� ( Y )
    float           m_fDistance;            // ĳ���Ϳ� ī�޶���� ���� [�Ÿ�] ( X Z )
    float           m_fMinDist;             // ĳ���Ϳ� ī�޶���� �ּ� [�Ÿ�] ( X Z )  
    float           m_fMaxDist;             // ĳ���Ϳ� ī�޶���� �ִ� [�Ÿ�] ( X Z )

private:
    // [ CAMERA LOCK MODE ]
    CGameObject*    m_pCamLock_Point;       // ī�޶� ��� ���� �� ���̴� Ÿ�ٿ� ���̴� ��   
    CGameObject*    m_pCamLock_TargetObj;   // ī�޶� ��� ���� �� ������ �� Ÿ�� ������Ʈ  
    Vec3            m_vCamLock_TragetPos;   // ī�޶� ��� ���� �� Ÿ�� ������Ʈ�� ��ġ
    
    bool            m_bCameraLock_Success;
    float           m_fCamRotSpeed;
    int             m_iCamRotDir;
    bool            m_bRotFinish;

    Vec3            m_vCamDir_CamLock;
    Vec3            m_vTargetDir_CamLock;
    Vec4            m_vCamPoint_Color;


public:
    Vec3 UpdateCameraRelativePos();
    void UpdateThirdPersonMode_CameraLock();
    bool RotateCamera(Vec3 _vFrom, Vec3 _vTo);

    void UpdateCameraLockPoint();

    void Reset_CameraLock();
    void Reset_Deafult();

    
public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


public:
    void UpdateFirstPersonMode();
    void UpdateThirdPersonMode();
    void UpdateFreeMode();


    void LateUpdateFirstPersonMode();
    void LeteUpdateThirdPersonMode();

    void SetCameraMode(CAMERA_MODE _eMode)      { m_eCamMode = _eMode; }
    void SetCameraOption(CAMERA_OPTION _eMode)  { m_eCamOption = _eMode; }

    CAMERA_MODE GetCameraMode()         { return m_eCamMode; }
    CAMERA_OPTION GetCameraOption()     { return m_eCamOption; }

    void SetCameraLockTargetObj(CGameObject* _pObj, Vec3 _vPos);


    // [ FIRST PERSON ] ( 1��Ī ���� �Լ��� ) 
    // ------------------------------------------------ FIRST_PERSON start
public:

    // ------------------------------------------------ FIRST_PERSON end 




    // [ THIRD PERSON ] ( 3��Ī ���� �Լ��� ) 
    // ------------------------------------------------ THIRD_PERSON start

public:
    // Rotate vertically around the object
    void RotateX(float rad);                // (note that the Euler angle radians around the x-axis are limited to [0, pi/3])
    void RotateY(float rad);                // Rotate around the object horizontally
    void Approach(float dist);              // Closer object
    void Translate(const Vec3& direction, float magnitude);


public:
    // [ GET PART ]
    Vec3 GetTargetPosition() const;     // Get the position of the current tracking object
    float GetDistance() const;          // Get the distance from the object
    Vec3 GetForwardAxis();


    // [ SET PART ]
       // (note that the Euler angle radians around the x-axis are limited to [0, pi/3])
    void SetRotationX(float rad);                          // Set the initial radians around the X-axis
    void SetRotationY(float rad);                          // Set the radian of the initial y -axis
    void SetTargetPos(const Vec3& target);                 // Set and bind the location of the object to be tracked
    void SetDistance(float dist);                          // Set the initial distance
    void SetDistanceMinMax(float minDist, float maxDist);  // Set the minimum maximum allowable distance

    // ------------------------------------------------ THIRD_PERSON end 




    CLONE(PlayerCamScript)

public:
    PlayerCamScript();
    PlayerCamScript(const PlayerCamScript& _origin);
    virtual ~PlayerCamScript();

};

