#pragma once
#include <Engine\CScript.h>

class CGameObject;

class ColumnFlameScript : public CScript
{
private:
	float m_fFlameSpeed;
	float m_fRotSpeed;
	Vec4  m_vColor;
	
	CGameObject* m_pDecal;
	CGameObject* m_pLaser_1;
	CGameObject* m_pLaser_2;


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

	CLONE(ColumnFlameScript);

public:
	ColumnFlameScript();
	ColumnFlameScript(const ColumnFlameScript& _origin);
	virtual ~ColumnFlameScript();
};
