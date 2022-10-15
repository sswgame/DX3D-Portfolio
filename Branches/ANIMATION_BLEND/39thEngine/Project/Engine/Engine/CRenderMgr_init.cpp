#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CResMgr.h"

#include "CMRT.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

void CRenderMgr::init()
{
	// PostProcess Texture 만들기
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	Ptr<CTexture> pPostProcessTex = CResMgr::GetInst()->CreateTexture(L"PostProcessTex",
	                                                                  (UINT)vResolution.x,
	                                                                  (UINT)vResolution.y,
	                                                                  DXGI_FORMAT_R8G8B8A8_UNORM,
	                                                                  D3D11_BIND_SHADER_RESOURCE,
	                                                                  true);


	CreateMRT();
	CreateMaterial();
}


void CRenderMgr::CreateMRT()
{
	// =============
	// SwapChain MRT
	// =============
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(1, &pRTTex, pDSTex);


	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();


	// ============
	// Deferred MRT
	// ============
	{
		Ptr<CTexture> arrTex[8] =
		{
			CResMgr::GetInst()->CreateTexture(L"ColorTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R8G8B8A8_UNORM,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),

			CResMgr::GetInst()->CreateTexture(L"NormalTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R32G32B32A32_FLOAT,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),

			CResMgr::GetInst()->CreateTexture(L"PositionTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R32G32B32A32_FLOAT,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),

			CResMgr::GetInst()->CreateTexture(L"DataTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R32G32B32A32_FLOAT,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true)
		};

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(4, arrTex, pDSTex);
	}


	// ==================
	// Deferred Decal MRT
	// ==================
	{
		Ptr<CTexture> arrTex[8] =
		{
			CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex")
		};

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED_DECAL] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->Create(1, arrTex, nullptr);
	}


	// =========
	// Light MRT
	// =========
	{
		Ptr<CTexture> arrTex[8] =
		{
			CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R8G8B8A8_UNORM,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),

			CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R8G8B8A8_UNORM,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),

			CResMgr::GetInst()->CreateTexture(L"ShadowPowerTargetTex",
			                                  (UINT)vResolution.x,
			                                  (UINT)vResolution.y,
			                                  DXGI_FORMAT_R32_FLOAT,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),
		};

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(3, arrTex, nullptr);
	}

	// =============
	// ShadowMap MRT
	// =============
	{
		Ptr<CTexture> arrTex[8] =
		{
			CResMgr::GetInst()->CreateTexture(L"ShadowMapTex",
			                                  4096,
			                                  4096,
			                                  DXGI_FORMAT_R32_FLOAT,
			                                  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			                                  true),
		};

		// Depth Stencil Texture 만들기
		Ptr<CTexture> pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"ShadowMapDSTex",
		                                                                   4096,
		                                                                   4096,
		                                                                   DXGI_FORMAT_D32_FLOAT,
		                                                                   D3D11_BIND_DEPTH_STENCIL,
		                                                                   true);


		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP]->Create(1, arrTex, pDepthStencilTex);
	}
}

void CRenderMgr::CreateMaterial()
{
	// Merge Shader
	m_pMergeShader = new CGraphicsShader;
	m_pMergeShader->CreateVertexShader(L"shader\\light.fx", "VS_Merge");
	m_pMergeShader->CreatePixelShader(L"shader\\light.fx", "PS_Merge");

	m_pMergeShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_NONE);

	m_pMergeShader->SetRSType(RS_TYPE::CULL_BACK);
	m_pMergeShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	m_pMergeShader->SetBSType(BS_TYPE::DEFAULT);

	// Merge Material
	m_pMergeMtrl = new CMaterial;
	m_pMergeMtrl->SetShader(m_pMergeShader);

	m_pMergeMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex"));
	m_pMergeMtrl->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex"));
	m_pMergeMtrl->SetTexParam(TEX_PARAM::TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularTargetTex"));
	m_pMergeMtrl->SetTexParam(TEX_PARAM::TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"ShadowPowerTargetTex"));


	Ptr<CGraphicsShader> pShader = nullptr;
	Ptr<CMaterial>       pMtrl   = nullptr;

	// Directional Light Shader	
	pShader = new CGraphicsShader;
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_Directional");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_Directional");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONEONE_BLEND);

	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"DirLightShader", pShader.Get(), true);

	// Directional Light Material		
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);

	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"ShadowMapTex"));

	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\DirLightMtrl.mtrl", pMtrl.Get(), true);


	// Point Light Shader	
	pShader = new CGraphicsShader;
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_Point");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_Point");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONEONE_BLEND);

	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"PointLightShader", pShader.Get(), true);

	// Point Light Material		
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);

	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));

	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\PointLightMtrl.mtrl", pMtrl.Get(), true);


	// ShadowMap Material
	pShader = new CGraphicsShader;
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_NONE);
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_ShadowMap");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_ShadowMap");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"ShadowMapShader", pShader.Get(), true);

	// ShadowMap Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);

	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\ShadowMap.mtrl", pMtrl.Get(), true);


	// Grid Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\tool.fx", "VS_Grid");
	pShader->CreatePixelShader(L"Shader\\tool.fx", "PS_Grid");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TOOL);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	// g_int_0 : 간격 단계
	// g_float_0 : 카메라 높이에 따른 그리드의 투명도
	// g_float_1 : 선의 두께
	// g_vec4_0  : 카메라 위치
	// g_vec4_1  : Grid 색상
	// g_tex_0  : Position Target
	pShader->AddScalarParamInfo(L"Line Thickness", SCALAR_PARAM::FLOAT_1);
	pShader->AddScalarParamInfo(L"Grid Color", SCALAR_PARAM::VEC4_1);
	pShader->AddTexParamInfo(L"Position Target", TEX_PARAM::TEX_0);

	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"GridShader", pShader.Get(), true);

	// Grid Material		
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);

	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));

	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\GridMtrl.mtrl", pMtrl.Get(), true);


	// Deferred Decal Shader
	pShader = new CGraphicsShader;

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL);
	pShader->CreateVertexShader(L"Shader\\std3d_deferred.fx", "VS_Deferred_Decal");
	pShader->CreatePixelShader(L"Shader\\std3d_deferred.fx", "PS_Deferred_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"DeferredDecalShader", pShader.Get(), true);

	// Deferred Decal Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\DeferredDecalMtrl.mtrl", pMtrl.Get(), true);


	// Forward Decal Shader
	pShader = new CGraphicsShader;

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_FORWARD_DECAL);
	pShader->CreateVertexShader(L"Shader\\std3d_deferred.fx", "VS_Deferred_Decal");
	pShader->CreatePixelShader(L"Shader\\std3d_deferred.fx", "PS_Deferred_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	CResMgr::GetInst()->AddRes<CGraphicsShader>(L"ForwardDecalShader", pShader.Get(), true);

	// Forward Decal Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\ForwardDecalMtrl.mtrl", pMtrl.Get(), true);
}

void CRenderMgr::ClearMRT()
{
	for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
	{
		if (nullptr != m_arrMRT[i])
			m_arrMRT[i]->Clear();
	}
}

void CRenderMgr::ClearTextureRegister()
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		CTexture::Clear(i);
	}
}
