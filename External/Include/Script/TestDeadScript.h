#pragma once
#include <Engine/CScript.h>

class TestDeadScript : public CScript
{
public:
	void update() override;

public:
	CLONE(TestDeadScript);
	TestDeadScript();
	virtual ~TestDeadScript();
};
