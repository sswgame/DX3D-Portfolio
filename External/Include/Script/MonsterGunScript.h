#pragma once
#include <Engine/CScript.h>

class CState;

struct tDeuxiemieData
{
	int hp = 100;
};

class MonsterGunScript final
	: public CScript
{
private:
	tDeuxiemieData m_monsterData;

private:
	void InitAnim();

public:
	void start() override;

public:
	const tDeuxiemieData& GetMonsterData() const;
	tDeuxiemieData&       GetMonsterData();

public:
	CLONE(MonsterGunScript);
	MonsterGunScript();
	virtual ~MonsterGunScript();
};
