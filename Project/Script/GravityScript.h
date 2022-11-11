#pragma once
#include <Engine/CScript.h>
class GravityScript :
    public CScript
{
private:
    bool    m_bOnGround;
    bool    m_bIgnoreGravity;   // 중력영향을 받지 않게 한다.

    float   m_bMinHeight;       // 최소 착지 높이 
    float   m_fDistFromLand;    // 땅과의 거리 차이 
    float   m_fLandHeight;      // 땅 높이 


public:
    bool Isfalling() { return !m_bOnGround; } // Gravity Script 를 소유한 객체가 현재 중력에 의해 떨어지고 있는지 확인한다. 

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

