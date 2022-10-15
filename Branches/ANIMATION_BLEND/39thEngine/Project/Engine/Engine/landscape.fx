#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"

// ================
// Landscape Shader
#define FaceXCount              g_int_0
#define FaceZCount              g_int_1  

#define HeightMap               g_tex_0
#define HeightMapResolution     g_vec2_0

#define HeightMapResolution     g_vec2_0
#define WeightMapResolution     g_vec2_1

#define TileTexArr              g_texarr_0
#define TileCount               g_float_0

StructuredBuffer<float4>        WEIGHT_MAP : register(t17);
// ================
struct VTX_IN
{
    float3 vPos : POSITION;    
    float2 vUV : TEXCOORD;    
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float2 vFullUV : TEXCOORD1;
    float3 vViewPos : POSITION1;        
};


VS_OUT VS_LandScape(VTX_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
       
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.vFullUV = _in.vUV / float2(FaceXCount, FaceZCount);    
    output.vViewPos = mul(float4(output.vPos, 1.f), g_matWV).xyz;
        
    return output;
}


// Hull Shader (덮개 쉐이더)
struct PatchTessFactor // 패치 당 분할 레벨 값
{
    float EdgeParam[3] : SV_TessFactor;
    float InsideParam : SV_InsideTessFactor;
};

// 패치 상수 함수(Patch Constant Function) - 패치당 한번씩 실행되는 함수, 
// 패치를 어떻게 분할 할 것인지를 반환해 줘야 한다.
PatchTessFactor HS_PatchConstant(InputPatch<VS_OUT, 3> _Patch, uint _PatchIdx : SV_PrimitiveID)
{
    PatchTessFactor param = (PatchTessFactor) 0.f;
    
    float3 vViewSidePos = (_Patch[0].vViewPos + _Patch[2].vViewPos) / 2.f;
    float3 vViewUpDownPos = (_Patch[1].vViewPos + _Patch[2].vViewPos) / 2.f;
    float3 vViewSlidePos = (_Patch[0].vViewPos + _Patch[1].vViewPos) / 2.f;
            
    param.EdgeParam[0] = GetTessFactor(vViewUpDownPos, 1.f, 32.f, 2000.f, 10000.f);
    param.EdgeParam[1] = GetTessFactor(vViewSidePos, 1.f, 32.f, 2000.f, 10000.f);
    param.EdgeParam[2] = GetTessFactor(vViewSlidePos, 1.f, 32.f, 2000.f, 10000.f);
    param.InsideParam = param.EdgeParam[2];
    
    return param;
}



[domain("tri")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_PatchConstant")]
[maxtessfactor(64.0)]
//[partitioning("integer")] 
[partitioning("fractional_odd")]
VS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _Patch, uint _Idx : SV_OutputControlPointID, uint _PatchIdx : SV_PrimitiveID)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _Patch[_Idx].vPos;
    output.vUV = _Patch[_Idx].vUV;    
    output.vViewPos = _Patch[_Idx].vViewPos;
    output.vFullUV = _Patch[_Idx].vFullUV;

    return output;
}


// -----> Tessellator



// Domain Shader
struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD; 
    float2 vFullUV : TEXCOORD1;
    float3 vViewPos : POSITION; 
    
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    float3 vViewNormal : NORMAL;
};


[domain("tri")]
DS_OUT DS_LandScape(float3 _vLocation : SV_DomainLocation, const OutputPatch<VS_OUT, 3> _Patch, PatchTessFactor _param)
{
    DS_OUT output = (DS_OUT) 0.f;

    float3 vLocalPos = _Patch[0].vPos * _vLocation[0] + _Patch[1].vPos * _vLocation[1] + _Patch[2].vPos * _vLocation[2];
    output.vUV = _Patch[0].vUV * _vLocation[0] + _Patch[1].vUV * _vLocation[1] + _Patch[2].vUV * _vLocation[2];      
    output.vFullUV = _Patch[0].vFullUV * _vLocation[0] + _Patch[1].vFullUV * _vLocation[1] + _Patch[2].vFullUV * _vLocation[2];
   
    
    // 지형 전체기준 UV 로 전환
    float2 vLandscapeUVStep = float2(1.f / HeightMapResolution.x, 1.f / HeightMapResolution.y);

    float2 vLandscapeUV = float2(output.vUV.x / (float)FaceXCount, output.vUV.y / (float)FaceZCount);
    float2 vLandScapeUpUV = float2(vLandscapeUV.x, vLandscapeUV.y - vLandscapeUVStep.y);
    float2 vLandScapeDownUV = float2(vLandscapeUV.x, vLandscapeUV.y + vLandscapeUVStep.y);
    float2 vLandScapeLeftUV = float2(vLandscapeUV.x - vLandscapeUVStep.x, vLandscapeUV.y);
    float2 vLandScapeRightUV = float2(vLandscapeUV.x + vLandscapeUVStep.x, vLandscapeUV.y);

    // 각 정점들이 자기 위치에 맞는 높이값을 높이맵에서 추출 한 후, 자신의 로컬 높이로 지정
    vLocalPos.y = HeightMap.SampleLevel(g_sam_0, vLandscapeUV, 0).r;
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;
    float2 vLandscapeLocalposStep = float2(FaceXCount / HeightMapResolution.x, FaceZCount / HeightMapResolution.y);

    //float3 vLocalUpPos = float3(vLocalPos.x, 0.f, vLocalPos.z + vLandscapeLocalposStep.y);
    //float3 vLocalDownPos = float3(vLocalPos.x, 0.f, vLocalPos.z - vLandscapeLocalposStep.y);
    //float3 vLocalLeftPos = float3(vLocalPos.x - vLandscapeLocalposStep.x, 0.f, vLocalPos.z);
    //float3 vLocalRightPos = float3(vLocalPos.x + vLandscapeLocalposStep.x, 0.f, vLocalPos.z);


    float3 vLocalUpPos = float3(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vLandScapeUpUV, 0).r, vLocalPos.z + vLandscapeLocalposStep.y);
    float3 vLocalDownPos = float3(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vLandScapeDownUV, 0).r, vLocalPos.z - vLandscapeLocalposStep.y);
    float3 vLocalLeftPos = float3(vLocalPos.x - vLandscapeLocalposStep.x, HeightMap.SampleLevel(g_sam_0, vLandScapeLeftUV, 0).r, vLocalPos.z);
    float3 vLocalRightPos = float3(vLocalPos.x + vLandscapeLocalposStep.x, HeightMap.SampleLevel(g_sam_0, vLandScapeRightUV, 0).r, vLocalPos.z);
    
    // Tangent, Binormal, Normal 재계산        
    output.vViewTangent = normalize(mul(float4(vLocalRightPos - vLocalLeftPos, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(vLocalUpPos - vLocalDownPos, 0.f), g_matWV).xyz);
    output.vViewNormal = normalize(cross(output.vViewBinormal, output.vViewTangent).xyz);
    
    // 투영좌표계까지 연산
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);    

    return output;
}


struct PS_OUT
{
    float4 vColor       : SV_Target0;
    float4 vViewNormal  : SV_Target1;
    float4 vViewPos     : SV_Target2;
    float4 vData        : SV_Target3;
};

PS_OUT PS_LandScape(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vColor = float4(0.8f, 0.8f, 0.8f, 1.f);        
    
    float3 vViewNormal = _in.vViewNormal;    

    // 타일 배열텍스쳐가 있으면
    if (g_btexarr_0)
    {
        float2 derivX = ddx(_in.vUV); // 인접픽셀과 x축 편미분값을 구한다
        float2 derivY = ddy(_in.vUV); // 인접픽셀과 y축 편미분값을 구한다

        // 타일 색상
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int)WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;

        int iMaxWeightIdx = 0;
        float fMaxWeight = 0.f;
        for (int i = 0; i < 4; ++i)
        {
            //vColor += TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            vColor += TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, i), 5.f) * vWeight[i];

            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
        output.vColor = float4(vColor.rgb, 1.f);

        // 타일 노말
        //float3 vTangentSpaceNormal = TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), derivX, derivY).xyz;
        float3 vTangentSpaceNormal = TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), 5.f).xyz;
        vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;

        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
        vViewNormal = normalize(mul(vTangentSpaceNormal, matTBN));
    }
 
    output.vViewPos = float4(_in.vViewPos, 1.f);    
    output.vViewNormal = float4(vViewNormal, 1.f);        
    
    return output;
}




#endif