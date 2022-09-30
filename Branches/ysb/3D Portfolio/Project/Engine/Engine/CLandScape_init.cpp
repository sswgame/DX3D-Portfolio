#include "pch.h"
#include "CLandScape.h"

#include "CSceneMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"


void CLandScape::Create()
{
	// ���� �޽� ����
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"LandscapeMesh");

	// �޽� �����
	// ������ �����ϴ� �޽��� ����
	if (nullptr != pMesh)
	{
		// ����
		CResMgr::GetInst()->ForceDeleteRes<CTexture>(L"LandscapeMesh");
		pMesh = nullptr;
	}

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	Vtx v;

	// ���� ��ġ
	for (UINT row = 0; row < m_iZFaceCount + 1; ++row)
	{
		for (UINT col = 0; col < m_iXFaceCount + 1; ++col)
		{
			v.vPos = Vec3((float)col, 0.f, (float)row);
			v.vUV = Vec2(col, m_iZFaceCount - row);

			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);

			v.vColor = Vec4(1.f, 0.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	// �ε���
	for (UINT row = 0; row < m_iZFaceCount; ++row)
	{
		for (UINT col = 0; col < m_iXFaceCount; ++col)
		{
			// 0
			// | \
			// 2- 1
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col);

			// 1- 2
			//  \ |
			//    0
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1 + 1);
		}
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CResMgr::GetInst()->AddRes<CMesh>(L"LandscapeMesh", pMesh.Get(), true);
	SetMesh(pMesh);


	// ======================
	// ���� ���̴� �� ���� ����
	// ======================		
	Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(L"LandScapeShader");

	if (nullptr == pShader)
	{
		pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\LandScape.fx", "VS_LandScape");
		pShader->CreateHullShader(L"shader\\LandScape.fx", "HS_LandScape");
		pShader->CreateDomainShader(L"shader\\LandScape.fx", "DS_LandScape");
		pShader->CreatePixelShader(L"shader\\LandScape.fx", "PS_LandScape");
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
		//pShader->SetRSType(RS_TYPE::CULL_BACK);
		pShader->SetRSType(RS_TYPE::WIRE_FRAME);
		pShader->SetBSType(BS_TYPE::DEFAULT);
		pShader->SetDSType(DS_TYPE::LESS);

		CResMgr::GetInst()->AddRes<CGraphicsShader>(L"LandScapeShader", pShader.Get(), true);
	}

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\LandScapeMtrl.mtrl");

	if (nullptr == pMtrl)
	{
		pMtrl = new CMaterial;
		pMtrl->SetShader(pShader);
		CResMgr::GetInst()->AddRes<CMaterial>(L"material\\LandScapeMtrl.mtrl", pMtrl.Get(), true);
	}	

	SetSharedMaterial(pMtrl);
}