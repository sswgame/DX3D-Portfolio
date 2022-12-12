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
	bool		m_bEnable_SSAO;
	bool		m_bEnable_FXAA;


public:
	void Init();
	void Apply(EFFECT_TYPE _eType);

public:
	bool IsEnable_SSAO() { return m_bEnable_SSAO; }
	void Enable_SSAO() { m_bEnable_SSAO = true; }
	void Disable_SSAO() { m_bEnable_SSAO = false; }

	bool IsEnable_FXAA() { return m_bEnable_FXAA; }
	void Enable_FXAA() { m_bEnable_FXAA = true; }
	void Disable_FXAA() { m_bEnable_FXAA = false; }

};

