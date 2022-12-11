#pragma once
#include <Engine/CScript.h>

class CEditorCamScript : public CScript
{
private:
	float m_fSpeed;
	bool  m_bEnableInput = true;
public:
	virtual void update() override;


	CLONE(CEditorCamScript);
public:
	CEditorCamScript();
	virtual ~CEditorCamScript();
};
