#pragma once
#include "ComponentUI.h"

class Animator3DUI : public ComponentUI
{
private:
	void RenderCurrentAnimInfo();
	void RenderAnimList();
	void RenderOpenTool();

public:
	void render_update() override;
public:
	Animator3DUI();
	virtual ~Animator3DUI();
};
