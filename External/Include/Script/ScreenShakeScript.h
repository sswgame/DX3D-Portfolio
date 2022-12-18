#pragma once

#include <Engine/CScript.h>

class ScreenShakeScript : public CScript
{
private:
	float m_fShaker;
	float m_fShakeTime;
	float m_fStrength;

public:
	void SetShakeTimer(float _fTime) { m_fShakeTime = _fTime; }
	void SetStrength(float _fTime) { m_fStrength = _fTime; }

	virtual void start();
	virtual void update();
	virtual void lateupdate();

	CLONE(ScreenShakeScript)

	ScreenShakeScript();
	ScreenShakeScript(const ScreenShakeScript& _origin);
	~ScreenShakeScript();
};
