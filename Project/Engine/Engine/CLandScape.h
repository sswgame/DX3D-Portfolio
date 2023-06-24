#pragma once
#include "CRenderComponent.h"

#include "CRaycastShader.h"
#include "CHeightMapShader.h"
#include "CWeightMapShader.h"

class CStructuredBuffer;

class CLandScape : public CRenderComponent
{
private:
	Ptr<CTexture> m_pHeightMap;

	UINT m_iXFaceCount;
	UINT m_iZFaceCount;

	Ptr<CRaycastShader> m_pCSRaycast;   // ��ŷ ���̴�
	CStructuredBuffer*  m_pCrossBuffer;	// ���콺 ��ŷ�Ǵ� ���� ���� �޴� ����

	Ptr<CHeightMapShader> m_pCSHeightMap; // ���̸� ���̴�
	Ptr<CTexture>         m_pBrushArrTex; // �귯���� �ؽ���

	Ptr<CWeightMapShader> m_pCSWeightMap;     // ����ġ ���̴�
	CStructuredBuffer*    m_pWeightMapBuffer; // ����ġ ���� ����
	UINT                  m_iWeightWidth;		// ����ġ ���� ���μ��� ��� ��
	UINT                  m_iWeightHeight;	// ����ġ ���� ���μ��� ��� ��
	UINT                  m_iWeightIdx;		// ���� ��ų ����ġ ����

	Ptr<CTexture> m_pTileArrTex;  // Ÿ�� �迭 �ؽ���

	Vec2 m_vBrushScale;
	int  m_iBrushIdx;    // �귯�� �ε���

	LANDSCAPE_MOD m_eMod; 	// ���� ����忡�� ���°�

public:
	void finalupdate() override;
	void render() override;

public:
	void SetFaceCount(UINT _iX, UINT _iZ)
	{
		m_iXFaceCount = _iX;
		m_iZFaceCount = _iZ;
	}

	void Create();

private:
	void Raycasting();

private:
	void CreateShaderMaterial();
	void CreateMesh();
	void CreateTexture();

public:
	void Deserialize(const YAML::Node& node) override;

public:
	CLONE(CLandScape);
	CLandScape();
	virtual ~CLandScape();
};
