#ifndef _BOSS_EFFECT
#define _BOSS_EFFECT

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
// Column Flame Shader
// DOMAIN : DOMAIN_DEFERRED
// Rasterizer : CULL_NONE
// DepthStencilState : LESS
// BlendState : DEFAULT
// =========================

#define YPOS g_float_0
#define COLOR g_vec4_0
#define SCALE g_vec4_1

VTX_OUT VS_ColumnFlame(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;

	output.vUV = _in.vUV;
    
    return output;
}


float4 PS_ColumnFlame(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;

    //_in.vUV.x *= 0.2f;
    _in.vUV.y += YPOS;
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    vOutColor *= COLOR;

    if ( 0.1f > vOutColor.b)
        vOutColor.a = 0.f;

    return vOutColor;
}



#endif
