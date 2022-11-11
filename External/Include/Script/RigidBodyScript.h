#pragma once
#include <Engine/CScript.h>


class RigidBodyScript :
    public CScript
{
private:

    Vec3            m_vForce;           // ũ�� , ���� 
    Vec3            m_vAccel;           // ���ӵ�  
    Vec3            m_vAccelA;          // �߰� ���ӵ� 
    Vec3            m_vVelocity;        // �ӵ� ( ũ�� : �ӷ� , ���� )

    float           m_fMass;            // ���� 
    float           m_fFricCoeff;       // ���� ��� 
    float           m_vAccelRatio;      // Accel ���� ���� 

    float           m_fMaxSpeed;
    float           m_fMaxJumpSpeed;    // ���� �ִ� ����

    bool            m_bIgnoreGravity;   // �߷� ���� ���� 

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

