#include "pch.h"
#include "TestDeadScript.h"

TestDeadScript::TestDeadScript(): CScript{(int)SCRIPT_TYPE::TESTDEADSCRIPT} {}
TestDeadScript::~TestDeadScript() {}

void TestDeadScript::update()
{
	if (KEY_TAP(KEY::F9))
	{
		GetOwner()->Destroy();
	}
}
