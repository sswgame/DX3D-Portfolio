#ifndef SSAO_HLSL
#define SSAO_HLSL

#include "FullScreenTriangle.fx"
#include "value.fx"

#define DiffuseTex      g_tex_0
#define NormalDepthTex  g_tex_1
#define RandomVecTex    g_tex_2
#define InputImageTex   g_tex_3



//
// Pass1 ������ ���� ����
//

struct VertexPosNormalTex
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 tex : TEXCOORD;
};

struct VertexPosHVNormalVTex
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 NormalV : NORMAL;
    float2 tex : TEXCOORD0;
};
// RTT : Render To Texture
// 
// �� ���� ���� ���� �� ���� ���� RTT�� �����ϴ� ���ؽ� ���̴�
VertexPosHVNormalVTex SSAO_GeometryVS(VertexPosNormalTex vIn)
{
    VertexPosHVNormalVTex vOut;

    // View Space ��ȯ 
    vOut.posV = mul(float4(vIn.posL, 1.0f), g_matWV).xyz;
    vOut.NormalV = mul(vIn.normalL, (float3x3) g_matWorldInv);
    // Ŭ�� �������� ��ȯ
    vOut.posH = mul(float4(vIn.posL, 1.0f), g_matWVP);
    vOut.tex = vIn.tex;

    return vOut;
}

// �� ���� ���� ���� �� ���� �� ����  RTT �ȼ� ���̴�
float4 SSAO_GeometryPS(VertexPosHVNormalVTex pIn, uniform bool alphaClip) : SV_TARGET
{
    // ���� ���͸� �������� ����ȭ�մϴ�.
    pIn.NormalV = normalize(pIn.NormalV);

    if (alphaClip)
    {
        float4 TexColor = DiffuseTex.Sample(g_sam_LinearWrap, pIn.tex);

        clip(TexColor.a - 0.1f);
    }

    // �� ������ ���� ���Ϳ� ���� ���� ��ȯ�մϴ�.
    return float4(pIn.NormalV, pIn.posV.z);
}

//
// Pass2 ����ϴ� AO
//

// ����Ʈ r�� p ������ ���� ���̰� �־����� ���ø� ����Ʈ q�� ���� ����Ʈ p�� ��� ������ ����մϴ�.
float OcclusionFunction(float distZ)
{
    //
    // ����(q)�� ����(p) �ڿ� ������(�ݱ� �ܺ�) �� q�� �� p�� ���� �� �����ϴ�. ���� ����(q)�� ����(p)�� �ʹ� ������
    //  �츮�� ���� ����(p)-����(r)�� �� q�� �� p�� ���� �� �ִٰ� �����ϱ� ���� ����ڰ� ������ ���Ƿ� ���� �ʰ��ؾ� �ϱ� ������ �� q�� �� p�� ���� �� ���ٰ� �Ͻ��ϴ�.
    //
     // ���� �Լ��� ����Ͽ� ��� ������ �����մϴ�.
    //
    //    /|\ Occlusion
    // 1.0 |      ---------------\
    //     |      |             |  \
    //     |                         \
    //     |      |             |      \
    //     |                             \
    //     |      |             |          \
    //     |                                 \
    // ----|------|-------------|-------------|-------> zv
    //     0     Eps          zStart         zEnd
    float occlusion = 0.0f;
    if (distZ > g_SurfaceEpsilon)
    {
        float fadeLength = g_OcclusionFadeEnd - g_OcclusionFadeStart;
        // distZ�� g_OcclusionFadeStart���� g_OcclusionFadeEnd�� ���� �������� ����ŷ ���� 1���� 0���� ���������� �����մϴ�.
        occlusion = saturate((g_OcclusionFadeEnd - distZ) / fadeLength);
    }
    return occlusion;
}

struct SSAO_VS_OUT
{
    float4 posH             : SV_Position;
    float3 farPlanePoint    : POSITION;
    float2 texcoord         : TEXCOORD;
};

struct SSAO_PS_OUT
{
    float4 vAO      : SV_Target0;
    float4 vAOTemp  : SV_Target1;
    float4 vRandVec : SV_Target2;
};

#define sampleCount g_int_0

// �ﰢ���� ����Ͽ� NDC ������ �����ϴ�.
// (-1, 1)________ (3, 1)
//        |   |  /
// (-1,-1)|___|/ (1, -1)   
//        |  /
// (-1,-3)|/    
SSAO_VS_OUT SSAO_VS(uint vertexID : SV_VertexID)
{
    SSAO_VS_OUT output;

    float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
    float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);

    output.texcoord      = grid * float2(1.0f, 1.0f);
    output.posH          = float4(xy, 1.0f, 1.0f);
    output.farPlanePoint = g_FarPlanePoints[vertexID].xyz;

    return output;

}

// SSAO �ȼ� ���̴�
SSAO_PS_OUT SSAO_PS(SSAO_VS_OUT _VSin)
{
    SSAO_PS_OUT output;
    // p - ����Ϸ��� �ֺ� ��� ��� ����
    // n -- �� p�� ���� ����
    // q - ���� p���� �ݱ��� ���� ����
    // r - �� p�� ����� �� �ִ� ��

    // �� ������ ���� ���Ϳ� ���� �ȼ��� �����ɴϴ�.z ��ǥ
    float4 normalDepth = NormalDepthTex.SampleLevel(g_sam_LinearWrap, _VSin.texcoord, 0.0f);

    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;

    //
     // �� ���� ��ǥ �籸��(x, y, z)
     // p = t * pIn.ToFarPlane�� �Ǵ� t ã��
   // p.z = t * pIn.ToFarPlane.z
   // t = p.z / pIn.ToFarPlane.z
   //
   float3 p = (pz / _VSin.farPlanePoint.z) * _VSin.farPlanePoint;

   //���� ���͸� ������ [0, 1]^3���� [-1, 1]^3���� ����
   float3 randVec = RandomVecTex.SampleLevel(g_sam_LinearWrap, 4.0f * _VSin.texcoord, 0.0f).xyz;
   randVec = 2.0f * randVec - 1.0f;

   float occlusionSum = 0.0f;

   // p�� �߽����� �ϴ� �ݱ����� ������ ���� n�� ���� p ������ ��
   for (int i = 0; i < sampleCount; ++i)
   {
        //  ������ ���ʹ� ��� �����Ǿ� �ְ� �����ϰ� �����Ǿ� �ֽ��ϴ�(���� �츮�� ����ϴ� ������ ���ʹ� ���� �������� �Բ� Ŭ�����͸����� �ʽ��ϴ�).
        //   �ݻ縦 ���� �̷��� ������ ���͸� ���� ���Ϳ� ������Ű�� �����ϰ� ������ ������ ���� �մϴ�.
        //   ���� ������ ����
       float3 offset = reflect(g_OffsetVectors[i].xyz, randVec);

       // ������ ���Ͱ� (p, n)���� ���ǵ� ��� �ڿ� �ִ� ��� ������
       float flip = sign(dot(offset, n));

       // ����Ʈ p�� ��� �ݰ� ���� �ִ� �ݱ� �� ���ø�
       float3 q = p + flip * g_OcclusionRadius * offset;

       // ������ �ؽ�ó ��ǥ�� ��� ���� ������Ʈ q
       float4 projQ = mul(float4(q, 1.0f), g_ViewToTexSpace);
       projQ /= projQ.w;

       // �� q�� ���⿡�� ������ ������ �� �ִ� ���� ����� �� r�� ���� ���� ���Ѵ�(�� r�� �������� ���� ���ɼ��� ������ �̶� ������
       // �� ����� �� ���Դϴ�). �̷��� �Ϸ��� ���� �ʿ��� �� ������ ���� ���� ������� �մϴ�.
       float rz = NormalDepthTex.SampleLevel(g_sam_LinearWrap, projQ.xy, 0.0f).w;

       // ���� ���� r���� �籸�� ���� r�� ��ǥ = (rx, ry, rz)
    // �츮�� �� r�� ������ �� q������ ������ ��ġ�Ѵٴ� ���� �˰� �����Ƿ� r = t * q
       // r.z = t * q.z ==> t = t.z / q.z
       float3 r = (rz / q.z) * q;

       // �� r�� p�� ���� ó���ϴ��� �׽�Ʈ
        // - ���� dot(n, normalize(r - p))�� ���� ó���� �� r���� ���(p, n)�� ��������� �Ÿ��� �����մϴ�. �� �����
        // �� ����� ���鿡�� �� ū ����ŷ ����Ʈ�� �����մϴ�. ���ÿ� �̰��� ���� ������ �����մϴ�.
        // (p, n)�� �ִ� �� r�� ��ü �׸��ڴ� �߸��� ��� ���� �����մϴ�(g_SurfaceEpsilon ����).
        //������ ���忡�� ���� ���� �ٸ��� �����ε�, ����� ���鿡��
        // (p, n) ���� �� r�� ��ǥ�� p�� ���� �ʽ��ϴ�.
        // - ��Ŭ���� ����ġ�� ũ��� ��Ŭ���� ������ ��� ���� ������ �Ÿ��� ���� �ٸ��ϴ�. ���� ó���� �� r�� ��ǥ�� p���� ������ �ִ� ���
        // �ָ�, �׷��� ����Ʈ r�� ����Ʈ p�� �������� �ʴ� ������ ���ֵ˴ϴ�.

       float distZ = p.z - r.z;
       float dp = max(dot(n, normalize(r - p)), 0.0f);
       float occlusion = dp * OcclusionFunction(distZ);

       occlusionSum += occlusion;
   }

   occlusionSum /= sampleCount;

   float access = 1.0f - occlusionSum;

   // SSAO �̹����� ��� ��ȭ�ϸ� SSAO �̹����� ȿ���� �� �и������ϴ�. 
   output.vAO       = saturate(pow(access, 8.f));
   output.vAOTemp   = float4(0.f, 1.f, 0.f, 1.f);
   output.vRandVec  = float4(0.f, 0.f, 1.f, 1.f);

   return output;

}

//
// Pass3 �����AO
//

// ����� ���͸�



float4 BilateralPS(float4 posH : SV_position,
    float2 texcoord : TEXCOORD) : SV_Target
{
    float s_BlurWeights[12] = (float[12]) g_BlurWeights;

    // �׻� �߾� ���� ��꿡 �߰��մϴ�.
    float4 color = s_BlurWeights[g_BlurRadius] * InputImageTex.SampleLevel(g_SamBlur, texcoord, 0.0f);
    float totalWeight = s_BlurWeights[g_BlurRadius];

    float4 centerNormalDepth = NormalDepthTex.SampleLevel(g_SamBlur, texcoord, 0.0f);
    // ���� ������ ���� ���Ϳ� ���̸� �����մϴ�.
    float3 centerNormal = centerNormalDepth.xyz;
    float centerDepth = centerNormalDepth.w;

    for (int i = -g_BlurRadius; i <= g_BlurRadius; ++i)
    {
        // ��� ���� �տ� �߰��Ǿ����ϴ�.

        if (i == 0)
            continue;
#if defined BLUR_HORZ
        float2 offset = float2(g_TexelSize.x * i, 0.0f);
        float4 neighborNormalDepth = NormalDepthTex.SampleLevel(g_SamBlur, texcoord + offset, 0.0f);
#else
        float2 offset = float2(0.0f, g_TexelSize.y * i);
        float4 neighborNormalDepth = NormalDepthTex.SampleLevel(g_SamBlur, texcoord + offset, 0.0f);
#endif

        // ���� ���Ϳ� ���� ����
        float3 neighborNormal = neighborNormalDepth.xyz;
        float neighborDepth = neighborNormalDepth.w;

        //
                // �߾� ���� ���� ���� ���� �Ǵ� ���� ���� ���̰� �ʹ� ũ�� ���� ���ø� ������ �����ڸ� ������ �ִ� ������ �����ϰ�,
                // ���� ���� ���� ���� �߰��ϴ� ���� ������� ���ʽÿ�.
                // �߾� ���� ���� �߰��˴ϴ�.
                //

        if (dot(neighborNormal, centerNormal) >= 0.8f && abs(neighborDepth - centerDepth) <= 0.2f)
        {
            float weight = s_BlurWeights[i + g_BlurRadius];

            // ���� �ȼ��� ���� �߰�
#if defined BLUR_HORZ
            color += weight * InputImageTex.SampleLevel(g_SamBlur, texcoord + offset, 0.0f);
#else
            color += weight * InputImageTex.SampleLevel(g_SamBlur, texcoord + offset, 0.0f);
#endif
            totalWeight += weight;
        }

    }

    // ��ü ����ġ�� 1�� ����� ��ӵ� ���� �ȼ��� �����մϴ�.
    return color / totalWeight;
}


float4 DebugAO_PS(float4 posH : SV_position,
    float2 texCoord : TEXCOORD) : SV_Target
{
    float depth = DiffuseTex.Sample(g_sam_LinearWrap, texCoord).r;
    return float4(depth.rrr, 1.0f);
}


#endif
