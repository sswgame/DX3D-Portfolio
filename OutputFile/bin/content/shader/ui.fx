#ifndef _UI_
#define _UI_

#include "value.fx"
#include "struct.fx"

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


// =========================
// UIShader
// g_tex_0 : UI Texture
// g_int_0 : ShowAlpha
// g_int_1 : UseInfo
// g_int_2 : UseProgressBar
// g_int_3 : ProgressBarDirection
// g_float_0 : Percentage
// g_vec2_0 : TextureSize
// g_vec4_0 : DrawInfo
// Rasterizer : CULL_BACK
// BlendState : Default
// DepthStencilState : LESS
// DOMAIN : FORWARD
// =========================

#define UITexture g_tex_0
#define ShowAlpha g_int_0
#define UseInfo g_int_1
#define TextureSize g_vec2_0
#define DrawInfo g_vec4_0
#define UseProgressBar g_int_2
#define ProgressBarDirection g_int_3
#define Percentage g_float_0
#define Anisotropy g_sam_0

#define	PROGRESSBAR_RIGHTTOLEFT	0
#define	PROGRESSBAR_LEFTTORIGHT	1
#define	PROGRESSBAR_TOPTOBOTTOM	2
#define	PROGRESSBAR_BOTTOMTOTOP	3

float2 ComputeUV(float2 inputUV, float2 pos, float2 size)
{
    float2 result = (float2) 0.f;
    if (inputUV.x == 0.f)
    {
        result.x = pos.x;
    }
    else
    {
        result.x = pos.x + size.x;
    }

    if (inputUV.y == 0.f)
    {
        result.y = pos.y;
    }
    else
    {
        result.y = pos.y + size.y;
    }
    return result;
}
float2 ComputeProgressBarUV(float2 inputUV, float percentage, int direction, inout int isDiscarded)
{
	// 오른쪽 -> 왼쪽
    if (direction == PROGRESSBAR_RIGHTTOLEFT)
    {
        if (inputUV.x > percentage)
        {
            isDiscarded = true;
        }
    }

	// 왼쪽 -> 오른쪽
    if (direction == PROGRESSBAR_LEFTTORIGHT)
    {
        if (inputUV.x < 1.f - percentage)
        {
            isDiscarded = true;
        }
    }

	// 위 -> 아래
    if (direction == PROGRESSBAR_TOPTOBOTTOM)
    {
        if (inputUV.y < 1.f - percentage)
        {
            isDiscarded = true;
        }
    }

	// 아래 -> 위
    if (direction == PROGRESSBAR_BOTTOMTOTOP)
    {
        if (inputUV.y > percentage)
        {
            isDiscarded = true;
        }
    }
    return inputUV;
}

VTX_OUT VS_UI(VTX_IN input)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(input.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(input.vPos, 1.f), g_matWorld).xyz;
    output.vUV = input.vUV;

    if (UseInfo)
    {
        float2 spritePosition = DrawInfo.xy / TextureSize;
        float2 imageSizeUV = DrawInfo.zw / TextureSize;
        output.vUV = ComputeUV(input.vUV, spritePosition, imageSizeUV);
    }

    return output;
}

float4 PS_UI(VTX_OUT input) : SV_Target
{
    float4 outputColor = (float4) 0.f;
    if (g_btex_0)
    {
        if (UseProgressBar)
        {
            int isDiscarded = false;
            float2 UV = ComputeProgressBarUV(input.vUV, Percentage, ProgressBarDirection, isDiscarded);
            if (isDiscarded)
            {
                discard;
            }
            outputColor = UITexture.Sample(Anisotropy, UV);
        }
        else
        {
            outputColor = UITexture.Sample(Anisotropy, input.vUV);
        }
    }
    else
    {
        if(false ==ShowAlpha)
        {
            discard;
        }
        else
        {
			outputColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    if (outputColor.a <= 0.f)
    {
        if (ShowAlpha)
            outputColor = float4(1.f, 0.f, 1.f, 1.f);
        else
            discard;
    }
    
    return outputColor;
}
#endif