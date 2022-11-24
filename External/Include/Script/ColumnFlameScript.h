#pragma once
#include <Engine\CScript.h>

class CGameObject;

class ColumnFlameScript : public CScript
{
private:
	float m_fFlameSpeed;
	float m_fRotSpeed;
	float m_fDamage;
	Vec4  m_vColor;

	CGameObject* m_pDecal;
	CGameObject* m_pLaser_1;
	CGameObject* m_pLaser_2;
	CGameObject* m_pDistortion_1;
	CGameObject* m_pDistortion_2;


public:
	void  SetDamage(float _fDamage) { m_fDamage = _fDamage; }
	float GetDamage() const { return m_fDamage; }

	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

	CLONE(ColumnFlameScript);

public:
	ColumnFlameScript();
	ColumnFlameScript(const ColumnFlameScript& _origin);
	virtual ~ColumnFlameScript();
};
