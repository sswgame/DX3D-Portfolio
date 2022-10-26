#pragma once

#include <Engine/CScript.h>


class CPlayerScript : public CScript
{
private:
	Ptr<CPrefab> m_pMissilePrefab;
	float        m_fSpeed;

	float m_fBurnStrength;
	bool  m_bBurn;


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


private:
	void Burnning();

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerScript)

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CPlayerScript();
	virtual ~CPlayerScript();
};
