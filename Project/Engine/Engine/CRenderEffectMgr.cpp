#include "pch.h"
#include "CRenderEffectMgr.h"
#include "CRenderMgr.h"
#include "CFXAA.h"
#include "CSSAO.h"

#include "CKeyMgr.h"


CRenderEffectMgr::CRenderEffectMgr()
{

}

CRenderEffectMgr::~CRenderEffectMgr()
{

}

void CRenderEffectMgr::Init()
{
	CFXAA::GetInst()->Init();


}

void CRenderEffectMgr::Apply(EFFECT_TYPE _eType)
{
	// SSAO Àû¿ë
	static bool bSSAO = false;

	

	
	switch (_eType)
	{
	case EFFECT_TYPE::FXAA:
	{

			CFXAA::GetInst()->Apply();
	}
		break;
	case EFFECT_TYPE::SSAO:
	{	
		if (KEY_TAP(KEY::Y))
		bSSAO = !bSSAO;

		if(bSSAO)
			CSSAO::GetInst()->Apply();
	}
		break;

	case EFFECT_TYPE::SSAO_BLUR:
	{
		if (bSSAO)
			CSSAO::GetInst()->BlurAmbientMap();
	}
	break;

	}


}

void CRenderEffectMgr::SetViewPort(D3D11_VIEWPORT _tViewPort)
{
	m_tViewPort = _tViewPort;
}
