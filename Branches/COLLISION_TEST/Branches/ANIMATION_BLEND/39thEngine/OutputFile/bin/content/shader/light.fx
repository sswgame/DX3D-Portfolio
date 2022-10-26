#ifndef _LIGHT
#define _LIGHT

#include "value.fx"


// ========================
// Directional Light Shader
// MRT      : Light
// mesh     : RectMesh
// RS       : CULL_BACK
// DS       : NO_TEST_NO_WRITE
// BS       : ONE_ONE

#define LightIdx        g_int_0
#define PositionTarget  g_tex_0
#define NormalTarget    g_tex_1
#define ShadowMap       g_tex_2

#define LightVP         g_mat_0
// ========================

struct VS_DIR_IN
{
    float3 vPos : POSITION;
};

struct VS_DIR_OUT
{
    float4 vPosition : SV_Position;
};

VS_DIR_OUT VS_Directional(VS_DIR_IN _in)
{
    VS_DIR_OUT output = (VS_DIR_OUT) 0.f;

    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    
    return output;
}

struct PS_DIR_OUT
{
    float4 vDiffuse : SV_Target0;
    float4 vSpecular : SV_Target1;
    float4 vShadowPow : SV_Target2;
};

PS_DIR_OUT PS_Directional(VS_DIR_OUT _in)
{
    PS_DIR_OUT output = (PS_DIR_OUT) 0.f;
    
    float2 vUV = _in.vPosition.xy / vResolution.xy;
    
    float3 vViewPos = PositionTarget.Sample(g_sam_0, vUV).xyz;
    float3 vViewNormal = NormalTarget.Sample(g_sam_0, vUV).xyz;
    float fShadowPow = 0.f;
    
    // 물체가 그려지지 않은 영역은 빛을 받을 수 없다.
    if (vViewPos.z == 0.f)
        clip(-1);
        
    tLightColor color = (tLightColor) 0.f;
    CalculateLight3D(vViewPos, vViewNormal, LightIdx, color);    
        
    // 그림자 판정
    // ViewPos -> WorldPos
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    
    // WorldPos -> Light 투영
    float4 vLightProj = mul(float4(vWorldPos, 1.f), LightVP);
    
    // w 로 나눠서 실제 xy 투영좌표를 구함
    vLightProj.xyz /= vLightProj.w;    
    
    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 vShadowMapUV = float2((vLightProj.x / 2.f) + 0.5f, -(vLightProj.y / 2.f) + 0.5f);  
    float fShadowMapDepth = ShadowMap.Sample(g_sam_0, vShadowMapUV).r;   
    
    // 광원에 기록된 깊이보다, 물체의 깊이가 더 멀 때, 그림자 판정
    if (0.f != fShadowMapDepth    
        && 0.f <= vShadowMapUV.x && vShadowMapUV.x <= 1.f
        && 0.f <= vShadowMapUV.y && vShadowMapUV.y <= 1.f
        && vLightProj.z >= fShadowMapDepth + 0.0001f)
    {
        fShadowPow = 0.9f;
    }     
    
    output.vDiffuse     = (color.vDiff + color.vAmb);
    output.vSpecular    = color.vSpec;
    output.vShadowPow   = fShadowPow;
    
    output.vDiffuse.a   = 1.f;
    output.vSpecular.a  = 1.f;
    output.vShadowPow.a = 1.f;
        
    return output;
}



// ==================
// Point Light Shader
// MRT      : Light
// Mesh     : Sphere
// RS_TYPE  : CULL_FRONT, 광원영역(Volume Mesh) 안으로 들어왔을 때를 대비
// BS_TYPE  : ONE_ONE, 기존에 그려진 빛(타겟) 을 누적
// DS_TYPE  : NO_TEST_NO_WRITE

//#define LightIdx        g_int_0
//#define PositionTarget  g_tex_0
//#define NormalTarget    g_tex_1
// ==================
VS_DIR_OUT VS_Point(VS_DIR_IN _in)
{
    VS_DIR_OUT output = (VS_DIR_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
    return output;
}


PS_DIR_OUT PS_Point(VS_DIR_OUT _in)
{
    PS_DIR_OUT output = (PS_DIR_OUT) 0.f;
        
    float2 vUV = _in.vPosition.xy / vResolution.xy;
    float3 vViewNormal = NormalTarget.Sample(g_sam_0, vUV).xyz;
        
    float3 vViewPos = PositionTarget.Sample(g_sam_0, vUV).xyz;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    if (length(vLocalPos) < 0.5f)
    {
        tLightColor color = (tLightColor) 0.f;
        CalculateLight3D(vViewPos, vViewNormal, LightIdx, color);
                      
        output.vDiffuse = color.vDiff + color.vAmb;
        output.vSpecular = color.vSpec;
        output.vShadowPow = (float4) 0.f;
    
        output.vDiffuse.a = 1.f;
        output.vSpecular.a = 1.f;
        output.vShadowPow.a = 1.f;
    }
    else
    {
        clip(-1);
        //output.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);
    }
         
    return output;
}





// =================
// Spot Light Shader
// =================









// ==============================
// Merge Shader
// Mesh : RectMesh
#define ColorTarget g_tex_0
#define DiffuseTarget g_tex_1
#define SpecularTarget g_tex_2
#define ShadowPowTarget g_tex_3
// ==============================
struct VS_MERGE_IN
{
    float3 vPos : POSITION;
};

struct VS_MERGE_OUT
{
    float4 vPosition : SV_Position;
};

VS_MERGE_OUT VS_Merge(VS_MERGE_IN _in)
{
    VS_MERGE_OUT output = (VS_MERGE_OUT) 0.f;

    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    
    return output;
}

float4 PS_Merge(VS_MERGE_OUT _in) : SV_Target0
{
    float4 vOutColor = (float4) 0.f;
    
    float2 vUV = _in.vPosition.xy / vResolution.xy;
    
    float3 vColor = ColorTarget.Sample(g_sam_0, vUV).xyz;
    float3 vDiffuse = DiffuseTarget.Sample(g_sam_0, vUV).xyz;
    float3 vSpecular = SpecularTarget.Sample(g_sam_0, vUV).xyz;
    float  fShadowPow = ShadowPowTarget.Sample(g_sam_0, vUV).x;
        
    vOutColor = float4(vColor * vDiffuse * (1.f - fShadowPow) 
                       + vSpecular * (1.f - fShadowPow), 1.f);
    
    
    return vOutColor;
}



// ===============
// ShdowMap Shader
// MRT : ShadowMap MRT
// RS : CULL_BACK
// BS : Default
// DS : Less
// ===============
struct VS_SHADOW_IN
{
    float3 vPos : POSITION; 
};

struct VS_SHADOW_OUT
{
    float4 vPosition : SV_Position;
    float4 vProjPos : POSITION;
};

VS_SHADOW_OUT VS_ShadowMap(VS_SHADOW_IN _in)
{
    VS_SHADOW_OUT output = (VS_SHADOW_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vProjPos = output.vPosition;    
    
    return output;
}

float PS_ShadowMap(VS_SHADOW_OUT _in) : SV_Target
{
    float fOut = 0.f;    
    fOut = _in.vProjPos.z / _in.vProjPos.w;
    return fOut;
}





#endif