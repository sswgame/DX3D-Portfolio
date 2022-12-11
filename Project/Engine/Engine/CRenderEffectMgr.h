#pragma once

enum class EFFECT_TYPE
{
	FXAA,
	SSAO,
	SSAO_BLUR,

	END,
};

class CRenderEffectMgr
	: public CSingleton<CRenderEffectMgr>
{
	SINGLE(CRenderEffectMgr);

private:
	D3D11_VIEWPORT m_tViewPort;

public:
	void Init();
	void Apply(EFFECT_TYPE _eType);

	void SetViewPort(D3D11_VIEWPORT _tViewPort);
};
