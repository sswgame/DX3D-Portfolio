#pragma once

#include <Engine/CScript.h>


struct tStateInfo;

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

	void UpdateFunc(const tStateInfo& info);
	void StartFunc(const tStateInfo& info);
	void EndFunc(const tStateInfo& info);

	void Test(const tStateInfo& info);
private:
	void Burnning();

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerScript)

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};
