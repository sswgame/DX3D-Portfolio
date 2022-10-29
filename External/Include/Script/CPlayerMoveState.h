#pragma once
#include <Engine/CState.h>
#include "CObjKeyMgr.h"

// player 위칙 관련 함수 정의 
class CPlayerMoveState :
    public CState
{
    class CTransform*   m_pTransform;
    tKey_Zip            m_tCurKeyInfo;

    float               m_fMoveForwardDT;
    float               m_fMoveSideDT;

public:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void LateUpdate();

public:
    // APPLY TO PLAYER OBJ
    void LookAt(const Vec3& target, const Vec3& up = { 0.0f, 1.0f, 0.0f });         //포인트 관찰
    void LookTo(const Vec3& direction, const Vec3& up = { 0.0f, 1.0f, 0.0f });      //특정 방향을 본다
    void Translate(const Vec3& direction, float magnitude);                         //특정 방향으로 이동

    void Strafe(float distance);            //  [ Strafing ] 현재 앞 방향 기준 좌/우 이동
    void Walk(float distance);              //  수평 지면에서 전방/후방 이동(1인칭 시점)
    void MoveForward(float distance);       //  카메라 바로 앞쪽으로 전진/후진 이동(자유 시야각)

    // 이 두 함수는 Player 전용 Camera 로 옮길 것 같음.. 
    void Pitch(float radian);               //  [ 상/하 보기 ]  -> radian (+) : 상 / radian (-) : 하
    void Yaw(float radian);                 //  [ 좌/우 보기 ]  -> radian (+) : 좌 / radian (-) : 우




    // CALCULATE POSITION
public:
    // [ SET PART ]
    //  [ Scale ]
    void SetScale(const Vec3& scale);                       //객체 스케일 설정
    void SetScale(float x, float y, float z);               //객체 스케일 설정
   
    // [ Rotation ]
    void SetRotation(const Vec3& eulerAnglesInRadian);      //오브젝트 오일러 각도 설정(라디안).객체는 Z-X-Y 순서로 회전합니다.
    void SetRotation(float x, float y, float z);
    
    void Rotate(const Vec3& eulerAnglesInRadian);                                //오일러 각도 회전 객체 지정
    void RotateAxis(const Vec3& axis, float radian);                             //축을 중심으로 회전할 중심점을 원점으로 지정
    void RotateAround(const Vec3& point, const Vec3& axis, float radian);        //축을 중심으로 회전할 점 지정

    // [ Position ]
    void SetPosition(const Vec3& position);                                     //객체 위치 설정
    void SetPosition(float x, float y, float z);
    
    void SetCurKeyInfo(tKey_Zip _keyInfo);



    // [ GET PART ]
public:
    // [ Scale ]
    Vec3 GetScale() const;           //객체의 크기를 가져옵니다.

    // [ Rotation ]
    Vec3 GetRotation() const;        //객체의 오일러 각도를 가져옵니다(라디안).객체는 Z-X-Y 순서로 회전됩니다.
    Vec3 GetPosition() const;        //객체의 위치를 ​​얻는다

    // [ Axis ]
    Vec3 GetRightAxis() const;       // 오른쪽 축 
    Vec3 GetUpAxis() const;          // 위쪽 축 
    Vec3 GetForwardAxis() const;     // 앞 축 

    // [ Matrix ]
    Matrix GetLocalToWorldMatrix() const;    // 세계 변환 행렬을 가져옵니다.
    Matrix GetWorldToLocalMatrix() const;    // 역 세계 변환 행렬을 가져옵니다.


private:
    Vec3 GetEulerAnglesFromRotationMatrix(const Matrix& rotationMatrix);    //회전 행렬에서 회전 오일러 각도를 가져옵니다.


    CLONE(CPlayerMoveState)
public:
    CPlayerMoveState();
    CPlayerMoveState(const CPlayerMoveState& _origin);
    virtual ~CPlayerMoveState();

};

