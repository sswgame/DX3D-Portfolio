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
	bool		m_bEnable_SSAO;

	// [ FXAA ] 
	bool		m_bEnable_FXAA;

	// [ FADE_INOUT_PAPERBURN ]
	bool		m_bEnable_FadeIn_PaperBurn;
	bool		m_bEnable_FadeOut_PaperBurn;
	float		m_fPaperBurn_Timer;


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

	bool IsEnable_FadeInPaperBurn() { return m_bEnable_FadeIn_PaperBurn; }
	void Enable_FadeInPaperBurn()   { m_bEnable_FadeIn_PaperBurn = true; m_bEnable_FadeOut_PaperBurn = false; }
	void Disable_FadeInPaperBurn()  { m_bEnable_FadeIn_PaperBurn = false; m_bEnable_FadeOut_PaperBurn = true;}

	bool IsEnable_FadeOutPaperBurn() { return m_bEnable_FadeOut_PaperBurn; }
	void Enable_FadeOutPaperBurn()   { m_bEnable_FadeOut_PaperBurn = true; m_bEnable_FadeIn_PaperBurn = false; }
	void Disable_FadeOutPaperBurn()  { m_bEnable_FadeOut_PaperBurn = false;m_bEnable_FadeIn_PaperBurn = true; }

};

