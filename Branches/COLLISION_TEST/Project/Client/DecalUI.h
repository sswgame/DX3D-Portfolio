#pragma once
#include "ComponentUI.h"
class DecalUI final
    : public ComponentUI
{
private:
	CDecal* m_pTargetDecal;

public:
	void render_update() override;

private:
	void TextureSelect(void* _pTextureName);

public:
	DecalUI();
	virtual ~DecalUI();
};

