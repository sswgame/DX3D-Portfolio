//**************************************************************************
// Modified by X_Jun(MKXJun)
// Source at https://github.com/NVIDIAGameWorks/Falcor/blob/master/Source/RenderPasses/Antialiasing/FXAA/FXAA.slang
// Licensed under the MIT License.
//
/***************************************************************************
 # Copyright (c) 2015-21, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/

#ifndef FXAA_HLSL
#define FXAA_HLSL


#define DEBUG_OUTPUT
#include "FullScreenTriangle.fx"
#include "value.fx"

#ifndef FXAA_QUALITY__PRESET
#define FXAA_QUALITY__PRESET 39
#endif

 //   FXAA ���߷� - ���߷�, �߼�
#if (FXAA_QUALITY__PRESET == 10)
#define FXAA_QUALITY__PS 3 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.5, 3.0, 12.0 };
#endif

#if (FXAA_QUALITY__PRESET == 11)
#define FXAA_QUALITY__PS 4 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 3.0, 12.0 };
#endif

#if (FXAA_QUALITY__PRESET == 12)
#define FXAA_QUALITY__PS 5 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 4.0, 12.0 };
#endif

#if (FXAA_QUALITY__PRESET == 13)
#define FXAA_QUALITY__PS 6 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 4.0, 12.0 };
#endif

#if (FXAA_QUALITY__PRESET == 14)
#define FXAA_QUALITY__PS 7 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 4.0, 12.0 };
#endif

#if (FXAA_QUALITY__PRESET == 15)
#define FXAA_QUALITY__PS 8 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 12.0 };
#endif

//   FXAA ǰ�� - �߰�, �� ����
#if (FXAA_QUALITY__PRESET == 20)
#define FXAA_QUALITY__PS 3 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.5, 2.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 21)
#define FXAA_QUALITY__PS 4 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 22)
#define FXAA_QUALITY__PS 5 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 23)
#define FXAA_QUALITY__PS 6 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 24)
#define FXAA_QUALITY__PS 7 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 3.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 25)
#define FXAA_QUALITY__PS 8 
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 26)
#define FXAA_QUALITY__PS 9
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 27)
#define FXAA_QUALITY__PS 10
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 28)
#define FXAA_QUALITY__PS 11
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif

#if (FXAA_QUALITY__PRESET == 29)
#define FXAA_QUALITY__PS 12
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif

//   FXAA ��ǰ��
#if (FXAA_QUALITY__PRESET == 39)
#define FXAA_QUALITY__PS 12
static const float s_SampleDistances[FXAA_QUALITY__PS] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 };
#endif


//
// ��� ���
//


float LinearRGBToLuminance(float3 LinearRGB)
{
    //return dot(LinearRGB, float3(0.299f, 0.587f, 0.114f));
    //return dot(LinearRGB, float3(0.212671f, 0.715160, 0.072169));
    //return sqrt(dot(LinearRGB, float3(0.212671f, 0.715160, 0.072169)));
    //return dot(LinearRGB, half3(0.2126729f, 0.7151522f, 0.0721750f));
    return log2(1 + dot(LinearRGB, float3(0.212671, 0.715160, 0.072169)) * 50) / 4;
}



/*--------------------------------------------------------------------------*/
/*
* - QualitySubPix
    ������ ���� 
    1.00 - �ε巯��
    0.75 - �⺻ ���� ��
    0.50 - �� �����ϰ� ���� �ȼ� �ٸ������ �� �����մϴ�.
    0.25 - ��¦ ����
    0.00 - ������ ����

* - QualityEdgeThreshold
    ���ϴ� ���� ��Ʈ��Ʈ�� �Ӱ谪 ����
    0.333 - �ſ� ����(�� ����)
    0.250 - ��ǰ��
    0.166 - �⺻
    0.125 - ��ǰ��
    0.063 - �ſ� ����(����)
    * - QualityEdfeThresholdMin

    ��ο� ������ ó������ �ʴ� �Ӱ谪
    0.0833 - �⺻
    0.0625 - �ణ �� ������
    0.0312 - ������
*/
/*--------------------------------------------------------------------------*/
#define PostProcessTarget           g_tex_0

#define DebugMode                   g_int_0
#define QualitySubPix               g_float_0
#define QualityEdgeThreshold        g_float_1
#define QualityEdfeThresholdMin     g_float_2

#define TexelSize                   g_vec2_0
/*--------------------------------------------------------------------------*/

struct PS_IN
{
    float4 posH     : SV_Position;
    float2 texcoord : TEXCOORD;
};

float4 PS_FXAA(PS_IN Input) : SV_Target
{
                
    float2 posM = Input.texcoord;
    float4 color = PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0);

    //   N
    // W M E
    //   S
    float lumaM = LinearRGBToLuminance(color.rgb);
    float lumaS = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(0, 1)).rgb);
    float lumaE = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(1, 0)).rgb);
    float lumaN = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(0, -1)).rgb);
    float lumaW = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(-1, 0)).rgb);

    //
    // ��� ����ϰ� ��Ƽ�ٸ���� ���� ���θ� �����մϴ�.

    //

    // ��� ��� ���� 5���� �ȼ� �� �ִ�/�ּ� ��� ��⸦ ã���ϴ�.
    float lumaRangeMax = max(lumaM, max(max(lumaW, lumaE), max(lumaN, lumaS)));
    float lumaRangeMin = min(lumaM, min(min(lumaW, lumaE), min(lumaN, lumaS)));
    float lumaRange = lumaRangeMax - lumaRangeMin;
    // ��� ��ȭ�� �ִ� ���� ���� ������谡 �ִ� �Ӱ谪 �����̰ų� 
    // ���� �Ӱ谪 �����̸� ���� ������ ���� ������ �ǹ��ϸ� 
    // ��Ƽ ���ϸ������ �������� �ʽ��ϴ�.
    bool earlyExit = lumaRange < max(QualityEdfeThresholdMin, lumaRangeMax* QualityEdgeThreshold);

    // �Ӱ谪�� �������� ������ ���� ����
    if (earlyExit)
        return color;

    //
    // ��谡 ���÷� �������� �������� ����
    //

    //           
    //  NW N NE          
    //  W  M  E
    //  WS S SE     
    //  edgeHorz = |(NW - W) - (W - WS)| + 2|(N - M) - (M - S)| + |(NE - E) - (E - SE)|
    //  edgeVert = |(NE - N) - (N - NW)| + 2|(E - M) - (M - W)| + |(SE - S) - (S - WS)|
    float lumaNW = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(-1, -1)).rgb);
    float lumaSE = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(1, 1)).rgb);
    float lumaNE = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(1, -1)).rgb);
    float lumaSW = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0, int2(-1, 1)).rgb);

    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;

    // ���� �� ���� ��� ���
    float edgeHorz = abs(lumaNWSW - 2.0 * lumaW) + abs(lumaNS - 2.0 * lumaM) * 2.0 + abs(lumaNESE - 2.0 * lumaE);
    float edgeVert = abs(lumaSWSE - 2.0 * lumaS) + abs(lumaWE - 2.0 * lumaM) * 2.0 + abs(lumaNWNE - 2.0 * lumaN);

    // ���� ���� ������� ���� ���� ������� �Ǵ�
    bool horzSpan = edgeHorz >= edgeVert;

    //
    //���⸦ ����ϰ� ����� ������ �����մϴ�.
    //
    float luma1 = horzSpan ? lumaN : lumaW;
    float luma2 = horzSpan ? lumaS : lumaE;

    float gradient1 = luma1 - lumaM;
    float gradient2 = luma2 - lumaM;
    // �ش� ������ ���⸦ ã�� ���� �ļ� �񱳸� ���� ũ�⸦ �����մϴ�.
    float gradientScaled = max(abs(gradient1), abs(gradient2)) * 0.25f;
    // ��� ������ ���� ���ĸ���
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    //
    // ���� �ȼ��� �׶��̼� �������� ���� �̵�texel
    //
    float lengthSign = horzSpan ? TexelSize.y : TexelSize.x;
    lengthSign = is1Steepest ? -lengthSign : lengthSign;

    float2 posB = posM.xy;
    // ���� �ؼ� ������
    if (!horzSpan)
        posB.x += lengthSign * 0.5;
    if (horzSpan)
        posB.y += lengthSign * 0.5;

    //
    // posB�� ������ �� �ȼ��� �ֵ� ����� ����մϴ�.
    //
    float luma3 = luma1 + lumaM;
    float luma4 = luma2 + lumaM;
    float lumaLocalAvg = luma3;
    if (!is1Steepest)
        lumaLocalAvg = luma4;
    lumaLocalAvg *= 0.5f;



    //
    //��ȸ�� ���� ������ �Ǵ� ������ �ƴ� ������ ������ ������ ������ ��ȸ�մϴ�
    //

    // �׵θ��� ���� �������� ������
    // 0    0    0
    // <-  posB ->
    // 1    1    1
    float2 offset;
    offset.x = (!horzSpan) ? 0.0 : TexelSize.x;
    offset.y = (horzSpan) ? 0.0 : TexelSize.y;
    // ���� ������
    float2 posN = posB - offset * s_SampleDistances[0];
    // ����Ƽ�� ������
    float2 posP = posB + offset * s_SampleDistances[0];

    //������ ������ ���� �ֵ� ���� ���� ���� �߰� ���� �ֵ����� ���̸� ����մϴ�.
    float lumaEndN = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posN, 0).rgb);
    float lumaEndP = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posP, 0).rgb);
    lumaEndN -= lumaLocalAvg;
    lumaEndP -= lumaLocalAvg;

    // �������� �縶�� ���̰� ���� �׷����Ʈ���� ũ�� �����ڸ��� ���ʿ� �����մϴ�.
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    bool doneNP = doneN && doneP;

    // �����ڸ��� �ƴ� ������ �������� ������ �ش� �������� ��� Ȯ���մϴ�.
    if (!doneN)
        posN -= offset * s_SampleDistances[1];
    if (!doneP)
        posP += offset * s_SampleDistances[1];

    // ������ �����ڸ��� ���� �Ǵ� �ݺ� Ƚ���� ������ ������ ��� �ݺ��մϴ�.
    if (!doneNP)
    {
        [unroll] // [Ư��] : ������ ������ ������ ������ Ǳ�ϴ�. ���������� ������ ����Ǵ� �ִ� Ƚ���� ������ �� �ֽ��ϴ�. [loop] �Ӽ� �� ȣȯ���� �ʽ��ϴ� .
        for (int i = 2; i < FXAA_QUALITY__PS; ++i)
        {
            if (!doneN)
                lumaEndN = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posN.xy, 0).rgb) - lumaLocalAvg;
            if (!doneP)
                lumaEndP = LinearRGBToLuminance(PostProcessTarget.SampleLevel(g_sam_LinearClamp, posP.xy, 0).rgb) - lumaLocalAvg;

            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            doneNP = doneN && doneP;

            if (!doneN)
                posN -= offset * s_SampleDistances[i];
            if (!doneP)
                posP += offset * s_SampleDistances[i];
            // ������ �����ڸ��� ���ʿ� �����ϸ� ����
            if (doneNP)
                break;
        }
    }

    // �� ���������� �Ÿ��� ���������� ���
    float distN = horzSpan ? (posM.x - posN.x) : (posM.y - posN.y);
    float distP = horzSpan ? (posP.x - posM.x) : (posP.y - posM.y);

    // ��� ������ ���� �������� �� ������� Ȯ���ϰ� �Ÿ��� �����մϴ�.
    bool directionN = distN < distP;
    float dist = min(distN, distP);

    // �� �� ������ �Ÿ�
    float spanLength = (distP + distN);

    // ���� ����� ������ ���� �̵��� �ȼ� ������
    float pixelOffset = -dist / spanLength + 0.5f;
    /*--------------------------------------------------------------------------*/

        // ���� �ȼ��� �ֵ��� posB�� ������ �� �ȼ��� �ֵ� ��հ����� ������ ����
        bool isLumaMSmaller = lumaM < lumaLocalAvg;

        // �̰��� ���� ������� ����
        bool goodSpanN = (lumaEndN < 0.0) != isLumaMSmaller;
        bool goodSpanP = (lumaEndP < 0.0) != isLumaMSmaller;
        bool goodSpan = directionN ? goodSpanN : goodSpanP;

        //�׷��� ���� ��� ������ ����
        float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
        /*--------------------------------------------------------------------------*/

            // 3x3 �������� �ȼ��� ��� ��ȭ ã��
            //      [1  2  1]
            // 1/12 [2 -12 2]
            //      [1  2  1]
            float subpixNSWE = lumaNS + lumaWE;
            float subpixNWSWNESE = lumaNWSW + lumaNESE;
            float subpixA = (2.0 * subpixNSWE + subpixNWSWNESE) * (1.0 / 12.0) - lumaM;
            // �� ��� ��ȭ�� ������� �����ȼ� �������� ����մϴ�.
            float subpixB = saturate(abs(subpixA) * (1.0 / lumaRange));
            float subpixC = (-2.0 * subpixB + 3.0) * subpixB * subpixB;
            float subpix = subpixC * subpixC * QualitySubPix;

            // ���� ū ������ ����
            float pixelOffsetSubpix = max(pixelOffsetGood, subpix);
            /*--------------------------------------------------------------------------*/

                if (!horzSpan)
                    posM.x += pixelOffsetSubpix * lengthSign;
                if (horzSpan)
                    posM.y += pixelOffsetSubpix * lengthSign;


                if (DebugMode)
                {
                    float3 Color = float3(1.0f - 2.0f * pixelOffsetSubpix, 2.0f * pixelOffsetSubpix, 0.0f);
                    return float4(Color, 1.0f);
                }
                else
                {
                    lumaM += 0.9f; // LumaM(�ֵ�) ���� ������ ���� ������ ���� £�����ϴ�. 
                    float3 Color = PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0).xyz;
                    return float4(Color, lumaM);
                }

          
    #ifdef DEBUG_OUTPUT
        float3 Color = float3(1.0f - 2.0f * pixelOffsetSubpix, 2.0f * pixelOffsetSubpix, 0.0f);
        return float4(Color, 1.0f);
    #else
        float3 Color = PostProcessTarget.SampleLevel(g_sam_LinearClamp, posM, 0).xyz;
        return float4(Color, lumaM);
    #endif
}


float4 PassThroughPS(float4 posH : SV_Position, float2 texCoord : TEXCOORD) : SV_Target
{
    uint2 pos = posH.xy;
    return PostProcessTarget[pos];
}

#endif
