#pragma once
#include "ComponentUI.h"

class Collider3DUI : public ComponentUI
{
private:
	virtual void render_update() override;

public:
	Collider3DUI();
	virtual ~Collider3DUI();
};
