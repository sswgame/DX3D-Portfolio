#pragma once
#include "singleton.h"

/*
    * FXAA : Fast Approximate Anti-Aliasing
            - Post Processing 
*/

// QualitySubPix
// ������ ����
// 1.00 - �ε巯��
// 0.75 - �⺻ ���� ��
// 0.50 - �� �����ϰ� ���� �ȼ� �ٸ������ �� �����մϴ�.
// 0.25 - ��¦ ���� 
// 0.00 - ������ ����   

// QualityEdgeThreshold
// ���ϴ� ���� ��Ʈ��Ʈ�� �Ӱ谪 ����
// 0.333 - �ſ� ����(�� ����)
// 0.250 - ��ǰ��
// 0.166 - �⺻
// 0.125 - ��ǰ��
// 0.063 - �ſ� ����(����)


// QualityEdgeThresholdMin
// ��ο� ������ ó������ �ʴ� �Ӱ谪
// 0.0833 - �⺻
// 0.0625 - �ణ �� ������
// 0.0312 - ������

struct FXAA_Implement
{
	int Major       = 3;
	int Minor       = 9;
	int EnableDebug = 0;

	float QualitySubPix           = 0.75f;
	float QualityEdgeThreshold    = 0.166f;
	float QualityEdgeThresholdMin = 0.0833f;

	Vec2 TexelSize;
};


class CFXAA : public CSingleton<CFXAA>

{
	SINGLE(CFXAA);

private:
	FXAA_Implement m_sImplement;
	D3D11_VIEWPORT m_tViewPort;

	Ptr<CMaterial> m_pMtrl;

public:
	void SetViewPort(D3D11_VIEWPORT _tViewPort);

	void Init();
	void UpdateData();
	void Apply();

	void InitShader();
	void InitMaterial();

	// major = 1 ��ǰ��,   minor = 0...5
	// major = 2 ��ǰ��    minor = 0...9
	// major = 3 ��ǰ��,   minor = 9
	void SetQuality(int major, int minor);
	void SetQualitySubPix(float val);
	void SetQualityEdgeThreshold(float threshold);
	void SetQualityEdgeThresholdMin(float thresholdMin);
	void EnableDebug(bool enabled);
};
