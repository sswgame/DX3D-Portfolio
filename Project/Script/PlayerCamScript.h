#pragma once
#include <Engine/CScript.h>

enum class CAMERA_MODE
{
    FIRST_PERSON,   // 1인칭
    THIRD_PERSON,   // 3인칭 
    FREE,

    END,

};

class PlayerCamScript :
    public CScript
{
private:
    CGameObject*    m_pCam;         // 카메라
    CAMERA_MODE     m_eCamMode;     // 카메라 촬영 모드 설정 


private:
    Vec3    m_vPos;                 // 카메라의 위치 
    Vec3    m_vTargetPos;           // 3인칭 ( 기준이 될 Target 위치 ) - [Player 위치] 
    Vec3    m_vPrevTargetPos;
    Vec3    m_vTargetPosAcc;        // 현재 카메라가 위치와 타겟 위치와의 거리 차이 - 거리차이만큼 움직일 것 ( 시간에따라서 자연스럽게 )
    
    float   m_fCamHeight;           // 카메라 [높이] 설정 - Player 기준 상대 높이 
    float   m_fDistance;            // 캐릭터와 카메라와의 고정 [거리] 
    float   m_fMinDist;             // 캐릭터와 카메라와의 최소 [거리]   
    float   m_fMaxDist;             // 캐릭터와 카메라와의 최대 [거리] 

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



    // [ FIRST PERSON ] ( 1인칭 전용 함수들 ) 
    // ------------------------------------------------ FIRST_PERSON start
public:

    // ------------------------------------------------ FIRST_PERSON end 




    // [ THIRD PERSON ] ( 3인칭 전용 함수들 ) 
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

