#ifndef _TRAIL
#define _TRAIL

#include "value.fx"

struct VTX_IN
{
    float3 vPos             : POSITION;
    float2 vUV              : TEXCOORD;

    float3 vTangent         : TANGENT;
    float3 vNormal          : NORMAL;
    float3 vBinormal        : BINORMAL;

    float4 vWeights         : BLENDWEIGHT;
    float4 vIndices         : BLENDINDICES;
};

struct VTX_OUT
{
    float4 vPosition        : SV_Position;
    float2 vUV              : TEXCOORD;

    float3 vViewPos         : POSITION;
    float3 vViewTangent     : TANGENT;
    float3 vViewNormal      : NORMAL;
    float3 vViewBinormal    : BINORMAL;
};

// =========================
// Trail
// DOMAIN : TRANSLUCENT
// Rasterizer : CULL_BACK
// DepthStencilState : LESS
// BlendState : DEFAULT
// =========================

#define CAMPOS  g_vec4_0;
#define TIMER   g_float_0;


VTX_OUT VS_Trail(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    //vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (TIMER * 4.f)) * 0.01;
    float fTimer = TIMER;
    float3 vLocalPos = _in.vPos;
    float3 vVector = _in.vPos;
    vVector = normalize(vVector);

    //vLocalPos.x += vVector.x * fTimer * 10.f;
   // vLocalPos.z += vVector.z * fTimer * 10.f;

    //vLocalPos.x += cos(fTimer * 3.141592f * 5.f);
    //vLocalPos.z += sin(fTimer * 3.141592f * 5.f);



    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vUV = _in.vUV;
    //output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
   // output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;

   // output.vPosition.x += fTimer;
   // output.vPosition.y += fTimer;
   // output.vPosition.z += fTimer;


    return output;
}


float4 PS_Trail(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;

    // ���� ���� ��ȯ ( VIEW -> WORLD -> LOCAL )
    float3 vViewPos = _in.vViewPos;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;

    // ������Ʈ ���� 
    /*
        �ܰ����� ���� ���� �κ� [ ������Ʈ ] �� ����ϰ� ���� ��� smoothstep ����
        min ���� �����ش�. �ܰ����� ���̸� �а� �ϰ� ������ max ���� 1���� ���� ���� ���ش�.

        [ smoothstep(min, max, x ) ]
            *   x�� [min, max] ������ ���� ��쿡 ���ؼ� [0, 1] ���̿��� �ε巴�� ���ϴ�
            *   Hermite �������� �����Ѵ�. x�� min���� �۴ٸ� 0�� �����ϰ�, max���� ũ�ٸ� 1�� �����Ѵ�.
    */

    float   fRimWidth               = 0.7f;
    float4  CameraPos               = CAMPOS;
    float3  vCamBetweenObjVector    = CameraPos.xyz - vWorldPos;
    float3  vCamPos                 = normalize(vCamBetweenObjVector);
    float   RimLightColor           = smoothstep(1.f - fRimWidth, 1.f, 1 - max(0, dot(_in.vViewNormal, vCamPos)));


    // ���İ� ���� 
    float  fAlpha = 0.51f;
    float fTimer = TIMER;
    //fAlpha += fTimer;

    // ���� ���� ( ������Ʈ ���� )
    float3 RGB = float3(    ((102.f - (fTimer / fAlpha) * 5.f) / 255.f) + RimLightColor / 2.f
                        ,   ((180.f - (fTimer / fAlpha) * 5.f) / 255.f) + RimLightColor / 2.f
                        ,   ((250.f - (fTimer / fAlpha) * 5.f) / 255.f) + RimLightColor / 2.f);

    if (length(RGB) <= 0.7f ||
        RimLightColor <= 0.01f)
        fAlpha = 0.f;





    // ���� ����  
    vOutColor = float4(RGB, fAlpha);
    return vOutColor;

}

#endif
