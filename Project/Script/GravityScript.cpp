#include "pch.h"
#include "GravityScript.h"
#include "RigidBodyScript.h"


GravityScript::GravityScript()
	: CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
	, m_bOnGround(false)
	, m_bMinHeight(0.f)
	, m_fDistFromLand(0.f)
	, m_bIgnoreGravity(false)
{
	SetName(L"GravityScript");

}

GravityScript::GravityScript(const GravityScript& _origin)
	: CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
	, m_bOnGround(false)
	, m_bMinHeight(0.f)
	, m_fDistFromLand(0.f)
	, m_bIgnoreGravity(false)
{
	SetName(L"GravityScript");
}

GravityScript::~GravityScript()
{
}


void GravityScript::start()
{
	m_bMinHeight = 0.f;
	m_fDistFromLand = 0.f; // 일단 땅 높이를 0으로 생각하겠다.. 추후에 바꿀 예정
	m_fLandHeight = 0.f;

}

void GravityScript::update()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	m_fDistFromLand = vPos.y - m_fLandHeight;

	if (vPos.y <= m_bMinHeight)
	{
		vPos.y = m_bMinHeight;
		GetOwner()->Transform()->SetRelativePos(vPos);
		m_bOnGround = true;
	}
	else
		m_bOnGround = false;

	RigidBodyScript* pRigidBody = (RigidBodyScript*)GetOwner()->GetScriptByName(L"RigidBodyScript");
	if (pRigidBody != nullptr)
	{
		// 공중에 있다면 중력의 영향을 받는다. 
		if (m_bOnGround == false)
		{
			// 중력 영향을 받을 건지 여부 
			if (!m_bIgnoreGravity)
				pRigidBody->SetAccelAlpha(Vec3(0.f, -2300.f, 0.f));

		}
		else
		{
			Vec3 vVelocity = pRigidBody->GetVelocity();
			pRigidBody->SetVelocity(Vec3(vVelocity.x, 0.f, vVelocity.z));

		}
	}


}

void GravityScript::lateupdate()
{
}


void GravityScript::SetIgnoreGravity(bool _b)
{
	m_bIgnoreGravity = _b;
	RigidBodyScript* pRigidBody = (RigidBodyScript*)GetOwner()->GetScriptByName(L"RigidBodyScript");
	if (pRigidBody)
	{
		pRigidBody->SetIgnoreGravity(_b);

	}


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

