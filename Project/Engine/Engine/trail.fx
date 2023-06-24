#ifndef _TRAIL
#define _TRAIL

#include "value.fx"

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;

    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
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
    VTX_OUT output = (VTX_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    float fTimer = TIMER;
    float3 vLocalPos = _in.vPos;
    float3 vVector = _in.vPos;
    vVector = normalize(vVector);
    float fRand = GaussianSample(g_noise_cloud, _in.vUV).r;

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

    float fRimWidth = 0.7f;
    float4 CameraPos = CAMPOS;
    float3 vCamBetweenObjVector = CameraPos.xyz - vWorldPos;
    float3 vCamPos = normalize(vCamBetweenObjVector);
    float RimLightColor = smoothstep(1.f - fRimWidth, 1.f, 1 - max(0, dot(_in.vViewNormal, vCamPos)));

    // ���İ� ���� 
    float fAlpha = 0.7f;
    float fTimer = TIMER;
    float fRand = GaussianSample(g_noise_cloud, _in.vUV).r;
    float fNoise = g_noise_cloud.Sample(g_sam_0, _in.vUV).g;

    fAlpha -= fTimer * 1.8f * fNoise;


    // ���� ���� ( ������Ʈ ���� )
    float3 RGB = float3(((102.f - (fTimer / fAlpha) * 2.f) / 255.f) + RimLightColor * 0.4f
                        , ((153.f - (fTimer / fAlpha) * 2.f) / 255.f) + RimLightColor * 0.4f
                        , ((255.f / 255.f) + RimLightColor * 0.4f));


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
#define PostProcessTarget   g_tex_1
#define IsBind              g_btex_1

VTX_OUT VS_SwordTrail(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_SwordTrail(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) float4(1.f, 1.f, 1.f, 0.5f);

    if (IsBind)
    {
        //vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (fAccTime * 4.f)) * 0.01;
        float2 vScreenUV = _in.vPosition.xy / vResolution;
        _in.vUV.x += fAccTime * 0.1f;
        vScreenUV += (g_noise_cloud.Sample(g_sam_0, _in.vUV).rg - 0.5f) * 0.1f;
        vScreenUV = saturate(vScreenUV);

        vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
       // vOutColor.w = 0.3f;
       // vOutColor.rgb = vOutColor.rgb * vOutColor.w + (1.f - vOutColor.w) * vTexColor.rgb;
       // vOutColor.b += 0.6f;

        // ���� ���İ��� ���� �ȵǰ� �������� ����� �ؽ�ó�� �̿��ؼ� �� �ڵ尡 �ִ°��� 
        // Sword_Trail_1.png
        // ���� �κ� 
        float4 vTexColor = (float4) float4(0.f, 0.f, 0.f, 0.f);
        if (g_btex_0)
        {
            vTexColor = g_tex_0.Sample(g_sam_0, _in.vUV);
            if (vTexColor.x <= 0.3f)
            {

            }
            // �� �κ� 
            else
            {
                // ���� ���´�. 
                if (g_btex_0)
                {
                    if (vTexColor.r <= 0.6f)
                        vTexColor.rgb = float3(102.f / 255.f, 153.f / 255.f, 250.f / 255.f);
                    vTexColor.w = 0.5f;
                    vOutColor.rgb = vOutColor.rgb + (vTexColor.rgb * vTexColor.w);
                }
            }
        }
            
        


    }
    else
    {
        if (g_btex_0)
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }


    return vOutColor;
}

#endif
