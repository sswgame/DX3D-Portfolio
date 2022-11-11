#pragma once
#include <Engine/CScript.h>
class GravityScript :
    public CScript
{
private:
    bool    m_bOnGround;
    bool    m_bIgnoreGravity;   // �߷¿����� ���� �ʰ� �Ѵ�.

    float   m_bMinHeight;       // �ּ� ���� ���� 
    float   m_fDistFromLand;    // ������ �Ÿ� ���� 
    float   m_fLandHeight;      // �� ���� 


public:
    bool Isfalling() { return !m_bOnGround; } // Gravity Script �� ������ ��ü�� ���� �߷¿� ���� �������� �ִ��� Ȯ���Ѵ�. 

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
    float   GetLandHeight() { return m_fLandHeight; }
    float   GetDistanceFromLand() { return m_fDistFromLand; }

    bool    IsIgnoreGravity() { return m_bIgnoreGravity; }
    void    SetIgnoreGravity(bool _b);



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(GravityScript)


public:
    GravityScript();
    GravityScript(const GravityScript& _origin);
    ~GravityScript();
};

