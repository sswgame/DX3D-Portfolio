#pragma once
#include <Engine/CScript.h>

class TestScript
	: public CScript
{
public:
	void OneClick();
	void DoubleClick();
	void MouseHover();
	void Drag();
	void start() override;
	void update() override;
public:
	CLONE(TestScript);
	TestScript();
	virtual ~TestScript();
};
