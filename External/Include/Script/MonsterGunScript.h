#pragma once
#include <Engine/CScript.h>

class MonsterGunScript : public CScript
{
private:
public:
	void start() override;
public:
	CLONE(MonsterGunScript);
	MonsterGunScript();
	virtual ~MonsterGunScript();
};
