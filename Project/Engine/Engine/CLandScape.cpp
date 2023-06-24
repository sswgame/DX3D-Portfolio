#include "pch.h"
#include "CLandScape.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iXFaceCount(0)
	, m_iZFaceCount(0)
	, m_pCrossBuffer(nullptr)
	, m_pWeightMapBuffer(nullptr)
	, m_iWeightWidth(0)
	, m_iWeightHeight(0)
	, m_iWeightIdx(0)
	, m_vBrushScale(1.f, 1.f)
	, m_iBrushIdx(0)
	, m_eMod(LANDSCAPE_MOD::NONE)
{
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CLandScape::~CLandScape()
{
	SAFE_DELETE(m_pCrossBuffer);
	SAFE_DELETE(m_pWeightMapBuffer);
}

void CLandScape::finalupdate()
{
	if (KEY_TAP(KEY::NUM0))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::NUM1))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::NUM2))
		m_eMod = LANDSCAPE_MOD::SPLAT;

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	Raycasting();

	// ��ŷ ������ system memory �� ������
	//m_pCrossBuffer->GetData(&out);

	if (KEY_PRESSED(KEY::LBTN))
	{
		m_vBrushScale = Vec2(0.1f, 0.1f);

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
		{
			// ���� ��ġ������ ���� ���̸� ���� ��
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // ��ŷ ������ HeightMapShader �� ����
			m_pCSHeightMap->SetBrushTex(m_pBrushArrTex);	// ����� �귯�� �ؽ��� ����
			m_pCSHeightMap->SetBrushIndex(m_iBrushIdx);		// �귯�� �ε��� ����
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // �귯�� ũ��
			m_pCSHeightMap->SetHeightMap(m_pHeightMap);
			m_pCSHeightMap->Excute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// ���� ��ġ������ ����ġ�� ������	
			m_pCSWeightMap->SetBrushArrTex(m_pBrushArrTex);
			m_pCSWeightMap->SetBrushIndex(m_iBrushIdx);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // �귯�� ũ��
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // ����ġ��, ���� ���� ����
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // ���� ĳ��Ʈ ��ġ
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->Excute();
		}
	}
}

void CLandScape::render()
{
	Transform()->UpdateData();

	// ����ġ ���� ����
	m_pWeightMapBuffer->UpdateData(PS, 17);

	// ����ġ ���� �ػ� ����
	auto vWeightMapResolution = Vec2(static_cast<float>(m_iWeightWidth), static_cast<float>(m_iWeightHeight));
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// Ÿ�� �迭 ���� ����
	float m_fTileCount = static_cast<float>(m_pTileArrTex->GetArraySize() / 2); // ����, �븻 �������־ ������ 2 ����
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fTileCount);

	// Ÿ�� �ؽ��� ����
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	GetMaterial(0)->UpdateData();
	GetMesh()->render(0);

	m_pWeightMapBuffer->Clear();
}


void CLandScape::Raycasting()
{
	// ���� ī�޶� ������
	const CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
	{
		return;
	}

	// ���� ���� ������ ������ ���÷� ����
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay&   ray         = pMainCam->GetRay();

	tRay CamRay   = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir   = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// ������ ī�޶� Ray �� ������ ����
	const tRaycastOut out = {Vec2(0.f, 0.f), 0x7fffffff, 0};
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_pHeightMap);
	m_pCSRaycast->SetFaceCount(m_iXFaceCount, m_iZFaceCount);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Excute();
}
