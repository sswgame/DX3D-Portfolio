#pragma once
#include "CEntity.h"

enum class EFFECT_TYPE
{
	FXAA,
	SSAO,
	SSAO_BLUR,

	FADE_IN_PAPERBURN,
	FADE_OUT_PAPERBURN,

	END,
};


class CRenderEffectMgr
	: public CSingleton<CRenderEffectMgr>
{
	SINGLE(CRenderEffectMgr);
private:
	// [ SSAO ]
	bool m_bEnable_SSAO;

	// [ FXAA ] 
	bool m_bEnable_FXAA;

	// [ FADE_INOUT_PAPERBURN ]
	bool  m_bEnable_FadeIn_PaperBurn;
	bool  m_bEnable_FadeOut_PaperBurn;
	float m_fPaperBurn_Timer;
	bool  m_bFadeInOutFinished = false;
	float m_fDuration;
public:
	void Init();
	void Apply(EFFECT_TYPE _eType);

public:
	// [ SSAO ]
	bool IsEnable_SSAO() { return m_bEnable_SSAO; }
	void Enable_SSAO() { m_bEnable_SSAO = true; }
	void Disable_SSAO() { m_bEnable_SSAO = false; }

	// [ FXAA ] 
	bool IsEnable_FXAA() { return m_bEnable_FXAA; }
	void Enable_FXAA() { m_bEnable_FXAA = true; }
	void Disable_FXAA() { m_bEnable_FXAA = false; }

	// [ FADE_IN_OUT_PAPERBURN ]
	void Init_FadePaperBurn();
	void Apply_FadeInOut_PaperBurn();

	bool IsFadeOutFinished() const { return m_bEnable_FadeOut_PaperBurn && m_bFadeInOutFinished; }
	bool IsFadeInFinished() const { return m_bEnable_FadeIn_PaperBurn && m_bFadeInOutFinished; }

	bool IsEnable_FadeInPaperBurn() { return m_bEnable_FadeIn_PaperBurn; }

	void Enable_FadeInPaperBurn(float _fDuration = 1.f)
	{
		m_bFadeInOutFinished = false;
		m_bEnable_FadeIn_PaperBurn = true;
		m_bEnable_FadeOut_PaperBurn = false;
		m_fPaperBurn_Timer = 0.f;
		m_fDuration = ClampData(_fDuration, 1.f, D3D11_FLOAT32_MAX);
	}

	void Disable_FadeInPaperBurn()
	{
		m_bEnable_FadeIn_PaperBurn = false;
	}


	bool IsEnable_FadeOutPaperBurn() { return m_bEnable_FadeOut_PaperBurn; }

	void Enable_FadeOutPaperBurn(float _fDuration = 1.f)
	{
		m_bFadeInOutFinished = false;
		m_bEnable_FadeOut_PaperBurn = true;
		m_bEnable_FadeIn_PaperBurn = false;
		m_fPaperBurn_Timer = 1.f;
		m_fDuration = ClampData(_fDuration, 1.f, D3D11_FLOAT32_MAX);
	}

	void Disable_FadeOutPaperBurn()
	{
		m_bEnable_FadeOut_PaperBurn = false;
	}
};
