#pragma once
#include "CEntity.h"

class CCamera;

enum class PLANE
{
    PL_LEFT,
    PL_RIGHT,
    PL_UP,
    PL_DOWN,
    PL_NEAR,
    PL_FAR,
    END,
};

class CFrustum :
    public CEntity
{
private:
    Vec3        m_ProjPos[8];
    Vec3        m_WorldPos[8];

    Vec4        m_arrPlane[(UINT)PLANE::END];

    CCamera*    m_pCam;

public:
    void finalupdate();

    bool PointCheck(Vec3 _vPos);
    bool SphereCheck(Vec3 _vPos, float _fRadius);

public:
    CLONE(CFrustum)
public:
    CFrustum();
    ~CFrustum();

    friend class CCamera;
};

