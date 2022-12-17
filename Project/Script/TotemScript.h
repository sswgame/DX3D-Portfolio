#pragma once

#include <Engine/CScript.h>

class TotemScript :
    public CScript
{
private:

public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject)	override;
	virtual void OnCollision(CGameObject* _OtherObject)			override;
	virtual void OnCollisionExit(CGameObject* _OtherObject)		override;

	CLONE(TotemScript)


public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	TotemScript();
	virtual ~TotemScript();
};

