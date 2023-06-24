#include "pch.h"
#include "CatmullRomSpline.h"
#include <Engine/CtimeMgr.h>


CatmullRomSpline::CatmullRomSpline()
{
}

CatmullRomSpline::~CatmullRomSpline()
{
}


void CatmullRomSpline::CreateRoute(Vec3 _p0, Vec3 _p1, Vec3 _p2, Vec3 _p3)
{

    SetP0(_p0);
    SetP1(_p1);
    SetP2(_p2);
    SetP3(_p3);

}

float CatmullRomSpline::GetAngleRadian(Vec3 _v1, Vec3 _v2)
{
    Vec3 vFrom = _v1;
    vFrom.Normalize();

    Vec3 vTo = _v2;
    vTo.Normalize();

    float fCos = vFrom.Dot(vTo);
    if (fCos >= 1.f)  fCos = 1.f;
    if (fCos <= -1.f) fCos = -1.f;

    float fRad = acosf(fCos);					// ���̰� ( Radian )
    float fAng = fRad * (180.f / XM_PI);		// ���̰� ( Angle )	}

    return fRad;
}


Vec3 CatmullRomSpline::CalculatePosByCatmullRom()
{
    Vec3 ResultPos = Vec3(0.f, 0.f, 0.f);
    Vec3 P0 = m_SplinePos[(UINT)CATMULL_ROM_SPLINE::P0];	// P0 ��ġ - P0 --> P2 
    Vec3 P1 = m_SplinePos[(UINT)CATMULL_ROM_SPLINE::P1];	// P1 ��ġ - Start ( ���� ��ġ ) t = 0.f
    Vec3 P2 = m_SplinePos[(UINT)CATMULL_ROM_SPLINE::P2];	// P2 ��ġ - End   ( �� ��ġ   ) t = 1.f
    Vec3 P3 = m_SplinePos[(UINT)CATMULL_ROM_SPLINE::P3];	// P3 ��ġ - P1 --> P3 
    float t = m_fInterPolation_Control_Coefficient;			// t  ���� ���� ���
    ResultPos = XMVectorCatmullRom(P0, P1, P2, P3, t); // Dx11 DirectX Math Ĺ�ַ� ���ö��� �����

    return ResultPos;
}

void CatmullRomSpline::Update()
{
    if (!m_bStart)
        return;
    // End ���� ���� 
    if (m_fInterPolation_Control_Coefficient >= 1.f)
    {
        m_fInterPolation_Control_Coefficient = 1.f;
        m_bIsEnd = true;
    }
    // Start ���� 
    else if (m_fInterPolation_Control_Coefficient <= 0.f)
    {
        m_fInterPolation_Control_Coefficient = 0.f;
    }

    // P1 -------> P2
    // t : ( Start ) 0.f ------ 1.f ( End )
    m_fInterPolation_Control_Coefficient += DT * m_fSpeed;

}

