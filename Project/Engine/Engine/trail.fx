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
// Trail - Player ���� ( Mesh ) 4
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

    float fTimer = TIMER;
    float3 vLocalPos = _in.vPos;
    float3 vVector = _in.vPos;
    vVector = normalize(vVector);
    float   fRand = GaussianSample(g_noise_cloud, _in.vUV).r;

    // �ܻ������Ʈ�� �ø���.
    vLocalPos.x += vVector.x * cos(sin(fTimer)) * fRand * 10.f;
    vLocalPos.z += fTimer * fRand * 80.f;


    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vUV = _in.vUV;
    //output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    // output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;

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

    float   fRimWidth = 0.7f;
    float4  CameraPos = CAMPOS;
    float3  vCamBetweenObjVector = CameraPos.xyz - vWorldPos;
    float3  vCamPos = normalize(vCamBetweenObjVector);
    float   RimLightColor = smoothstep(1.f - fRimWidth, 1.f, 1 - max(0, dot(_in.vViewNormal, vCamPos)));

    // ���İ� ���� 
    float   fAlpha = 0.7f;
    float   fTimer = TIMER;
    float   fRand = GaussianSample(g_noise_cloud, _in.vUV).r;
    float   fNoise = g_noise_cloud.Sample(g_sam_0, _in.vUV).g;

    fAlpha -= fTimer * 1.8f * fNoise;


    // ���� ���� ( ������Ʈ ���� )
    float3 RGB = float3(((102.f - (fTimer / fAlpha) * 2.f) / 255.f) + RimLightColor * 0.4f
                        ,   ((153.f - (fTimer / fAlpha) * 2.f) / 255.f) + RimLightColor * 0.4f
                        ,   ((255.f / 255.f) + RimLightColor * 0.4f));


    // ���� ����  
    vOutColor = float4(RGB, fAlpha);
    return vOutColor;

}


// =========================
// Std3D Sword Trail Shader
// g_int_0 : Shader Type
// g_tex_0 : Output Texture
// g_tex_1 : Normal Map
//
// DOMAIN : Masked
// Rasterizer : CULL_BACK
// DepthStencilState : LESS
// BlendState : DEFAULT
// =========================
VTX_OUT VS_SwordTrail(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_SwordTrail(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) float4(1.f, 1.f, 1.f, 0.5f);

    // ������Ʈ ���� ����
   if (g_btex_0)
    {
       vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }

   float   fNoise = g_noise_cloud.Sample(g_sam_0, _in.vUV).g;


   // �׳� ������ ������ �̷����ϰ����� 
   /*
      if (vOutColor.x <= 0.3f)
       vOutColor.w = 0.f;
   else
   {
       vOutColor.w = 0.5f;

       vOutColor.x = 0.f   / 255.f;
       vOutColor.y = 102.f / 255.f;
       vOutColor.z = 230.f / 255.f;
   }
   */




    return vOutColor;
}




#endif
