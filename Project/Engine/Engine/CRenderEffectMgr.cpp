#include "pch.h"
#include "CRenderEffectMgr.h"
#include "CRenderMgr.h"
#include "CFXAA.h"
#include "CSSAO.h"

#include "CKeyMgr.h"
#include "CMRT.h"



CRenderEffectMgr::CRenderEffectMgr()
	: m_bEnable_FXAA(true)
	, m_bEnable_SSAO(true)
{

}

CRenderEffectMgr::~CRenderEffectMgr()
{

}

void CRenderEffectMgr::Init()
{
	CFXAA::GetInst()->Init();
	CSSAO::GetInst()->Init();


}

void CRenderEffectMgr::Apply(EFFECT_TYPE _eType)
{

	switch (_eType)
	{
	case EFFECT_TYPE::FXAA:
	{
		if (!m_bEnable_FXAA)
			break;

		CRenderMgr::GetInst()->CopyTargetToPostProcess();
		D3D11_VIEWPORT SwapChain_VP = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->GetViewPort();
		CFXAA::GetInst()->SetViewPort(SwapChain_VP);
		CFXAA::GetInst()->Apply();
	}
	break;
	case EFFECT_TYPE::SSAO:
	{
		if (!m_bEnable_SSAO)
			break;

		CSSAO::GetInst()->Apply();
	}
	break;

	case EFFECT_TYPE::SSAO_BLUR:
	{
		if (!m_bEnable_SSAO)
			break;

		CSSAO::GetInst()->BlurAmbientMap();
	}
	break;

	}


}
