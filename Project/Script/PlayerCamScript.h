#pragma once
#include <Engine/CScript.h>

enum class CAMERA_MODE
{
    FIRST_PERSON,   // 1��Ī
    THIRD_PERSON,   // 3��Ī 
    FREE,

    END,

};

class PlayerCamScript :
    public CScript
{
private:
    CGameObject*    m_pCam;         // ī�޶�
    CAMERA_MODE     m_eCamMode;     // ī�޶� �Կ� ��� ���� 


private:
    Vec3    m_vPos;                 // ī�޶��� ��ġ 
    Vec3    m_vTargetPos;           // 3��Ī ( ������ �� Target ��ġ ) - [Player ��ġ] 
    Vec3    m_vPrevTargetPos;
    Vec3    m_vTargetPosAcc;        // ���� ī�޶� ��ġ�� Ÿ�� ��ġ���� �Ÿ� ���� - �Ÿ����̸�ŭ ������ �� ( �ð������� �ڿ������� )
    
    float   m_fCamHeight;           // ī�޶� [����] ���� - Player ���� ��� ���� 
    float   m_fDistance;            // ĳ���Ϳ� ī�޶���� ���� [�Ÿ�] 
    float   m_fMinDist;             // ĳ���Ϳ� ī�޶���� �ּ� [�Ÿ�]   
    float   m_fMaxDist;             // ĳ���Ϳ� ī�޶���� �ִ� [�Ÿ�] 

private:
    Vec3 UpdateCameraRelativePos();

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

    void LateUpdateFirstPersonMode();
    void LeteUpdateThirdPersonMode();

    void SetCameraMode(CAMERA_MODE _eMode) { m_eCamMode = _eMode; }
    CAMERA_MODE GetCameraMode() { return m_eCamMode; }



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
	virtual ~PlayerCamScript();

};

