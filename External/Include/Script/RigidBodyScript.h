#pragma once
#include <Engine/CScript.h>


class RigidBodyScript :
    public CScript
{
private:

    Vec3            m_vForce;           // 크기 , 방향 
    Vec3            m_vAccel;           // 가속도  
    Vec3            m_vAccelA;          // 추가 가속도 
    Vec3            m_vVelocity;        // 속도 ( 크기 : 속력 , 방향 )

    float           m_fMass;            // 질량 
    float           m_fFricCoeff;       // 마찰 계수 
    float           m_vAccelRatio;      // Accel 적용 비율 

    float           m_fMaxSpeed;
    float           m_fMaxJumpSpeed;    // 점프 최대 높이

    bool            m_bIgnoreGravity;   // 중력 영향 여부 

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


public:
    void AddForce(Vec3 _vF)                 { m_vForce += _vF; }
    void AddVelocity(Vec3 _v)               { m_vVelocity += _v; }
    void AddAccelAlpha(Vec3 _vAccelA)       { m_vAccelA += _vAccelA; }


public:
    void SetForce(Vec3 _vForce)             { m_vForce = _vForce; }
    void SetVelocity(Vec3 _v)               { m_vVelocity = _v; }
    void SetMass(float _fMass)              { m_fMass = _fMass; }
    void SetFricCoeff(float _fFricCoeff)    { m_fFricCoeff = _fFricCoeff; }
    void SetAccelAlpha(Vec3 _vAccelA)       { m_vAccelA = _vAccelA; }
    void SetMaxSpeed(float _fMaxSpeed)      { m_fMaxSpeed = _fMaxSpeed; }
    void SetIgnoreGravity(bool _b)          { m_bIgnoreGravity = _b; }

    Vec3 GetForce()                         { return m_vForce; }
    Vec3 GetVelocity()                      { return m_vVelocity; }
    Vec3 GetAccelAlpha()                    { return m_vAccelA; }
    float GetMass()                         { return m_fMass; }
    float GetFricCoeff()                    { return m_fFricCoeff; }


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(RigidBodyScript)


public:
    RigidBodyScript();
    RigidBodyScript(const RigidBodyScript& _origin);
    ~RigidBodyScript();

};

