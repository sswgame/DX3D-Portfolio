#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float3 vViewPos : POSITION1;
    float2 vUV : TEXCOORD;
};

// =====================================
// PostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite
#define PostProcessTarget   g_tex_0
#define IsBind              g_btex_0
// =====================================
VTX_OUT VS_PostProcess(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;    
    
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
    float2 vScreenUV = _in.vPosition.xy / vResolution;    
    
    if (IsBind)
    {        
        //vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (fAccTime * 4.f)) * 0.01;
                
        _in.vUV.x += fAccTime * 0.1f;        
        vScreenUV += (g_noise_cloud.Sample(g_sam_0, _in.vUV).rg - 0.5f) * 0.03f;
        vScreenUV = saturate(vScreenUV);              
                
        vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }    
    
    return vOutColor;
}


// =====================================
// FlamePostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite
// =====================================
#define SPEED g_float_0

VTX_OUT VS_FlamePostProcess(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_FlamePostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (IsBind)
    {
        // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
        float2 vScreenUV = _in.vPosition.xy / vResolution;
        float4 vMaskColor = g_noise_cloud.Sample(g_sam_0, vScreenUV);
        
        if (vMaskColor.r == 0.0f)
        {
            float X = vScreenUV.x + SPEED;
            float Y = vScreenUV.y + SPEED;
            vScreenUV.x += sin(X - Y) * 0.01 * sin(Y);
            vScreenUV.y += cos(X + Y) * 0.01 * cos(Y);
        }
        
        vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return vOutColor;
}




#endif
