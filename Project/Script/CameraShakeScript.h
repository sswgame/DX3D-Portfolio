#pragma once
#include <Engine\CScript.h>

class CameraShakeScript : public CScript
{
private:
	bool  m_bShake;
	int   m_iShaker;
	float m_fEffectTime;
	float m_fTimer;

	Vec3 m_vOriginPos;

public:
	virtual void update();
	virtual void lateupdate();

	void StartShake(bool _bShake, float _fTime);
	bool IsShake(bool _bShake) { return m_bShake; }

	CLONE(CameraShakeScript)
	CameraShakeScript();
	~CameraShakeScript();
};
