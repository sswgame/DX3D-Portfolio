#include "pch.h"
#include "GravityScript.h"

GravityScript::GravityScript()
	: CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
{
}

GravityScript::GravityScript(const GravityScript& _origin)
	: CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)

{
}

GravityScript::~GravityScript()
{
}


void GravityScript::start()
{
}

void GravityScript::update()
{
}

void GravityScript::lateupdate()
{
}

void GravityScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void GravityScript::OnCollision(CGameObject* _OtherObject)
{
}

void GravityScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void GravityScript::SaveToScene(FILE* _pFile)
{
}

void GravityScript::LoadFromScene(FILE* _pFile)
{
}

