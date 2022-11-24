#include "pch.h"
#include "TestScript.h"

#include "UIImageScript.h"


TestScript::TestScript()
	: CScript{(int)SCRIPT_TYPE::TESTSCRIPT}
{
}

TestScript::~TestScript()
{
}

void TestScript::OneClick()
{
	MessageBox(nullptr, L"CL", L"C", 0);
}

void TestScript::DoubleClick()
{
	OutputDebugString(L"DOUBLE\n");
}

void TestScript::MouseHover()
{
	OutputDebugString(L"HOVER\n");
}

void TestScript::Drag()
{
	Vec2 deltaPos   = CKeyMgr::GetInst()->GetMouseDelta();
	Vec3 currentPos = Transform()->GetRelativePos();
	Transform()->SetRelativePos(currentPos + deltaPos);
}

void TestScript::start()
{
	GetOwner()->GetScript<UIImageScript>()->SetOrderZ(3);
}

void TestScript::update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		GetOwner()->GetScript<UIImageScript>()->SetOrderZ(-1);
	}
}
