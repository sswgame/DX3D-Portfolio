#pragma once
#include "CTexture.h"
#include "singleton.h"

class CSSAO
	: public CSingleton<CSSAO>
{
	SINGLE(CSSAO);

private:
	// SSAO �⺻ ����, ���� ����
	uint32_t m_SampleCount        = 14;           // ���� ������ ��
	uint32_t m_BlurCount          = 4;            // �� ��
	float    m_OcclusionRadius    = 2.f;          // �ݱ��� �ݰ� ���ø�
	float    m_OcclusionFadeStart = 0.2f;         // ������� ���� ��ȯ�� �����ϱ� ���� ���� �Ÿ� ��
	float    m_OcclusionFadeEnd   = 2.0f;         // ������ ���ܵ��� ���� �Ÿ� ��
	float    m_SurfaceEpsilon     = 0.05f;        // �ڱ� ������ �����ϱ� ���� �Ÿ� ��
	float    m_BlurWeights[11]    = {0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f};
	uint32_t m_BlurRadius         = 5;

	Vec4 m_Offsets[14] = {};

private:
	// UpdateData SSAO Implement 
	tSSAO_ChangesOnResize m_tSSAO_ChangesOnResize;
	tSSAO_ChangesRarely   m_tSSAO_ChangesRarely;

	D3D11_VIEWPORT m_tViewPort;

private:
	Ptr<CTexture> m_pNormalDepthTexture;           // �Ϲ�/���� �ؽ�ó
	Ptr<CTexture> m_pAOTexture;                    // �ں��Ʈ ��Ŭ���� ��
	Ptr<CTexture> m_pAOTempTexture;                // �߰� �ں��Ʈ ��Ŭ���� ��
	Ptr<CTexture> m_pRandomVectorTexture;          // ���� ���� �ؽ�ó

public:
	void Init();
	void Apply();


	void Init_SSAO_Texture();
	void Init_SSAO_NormalDepthMtrl(); // SSAO_Geometry
	void Init_SSAO_Mtrl();
	void Init_SSAO_BlurHorzMtrl();
	void Init_SSAO_BlurVertMtrl();
	void Init_SSAO_DebugMtrl();

	void UpdateData_SSAO_ChangesOnResize();
	void UpdateData_SSAO_ChangesRarely();

	void UpdateData_SSAO();

public:
	void RenderToSSAOTexture();
	void BlurAmbientMap();
	void BilateralBlurX();
	void BilateralBlurY();

public:
	void BuildRandomVectorTexture();
	void BuildOffsetVectors();

public:
	void Begin();
	void End();

public:
	void SetSampleCount(uint32_t count) { m_SampleCount = count; }
};
