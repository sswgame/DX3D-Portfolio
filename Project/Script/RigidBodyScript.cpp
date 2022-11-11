#include "pch.h"
#include "RigidBodyScript.h"

RigidBodyScript::RigidBodyScript()
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_fMass(1.f)
	, m_fFricCoeff(500.f)
	, m_vAccelRatio(1.f)
	, m_fMaxSpeed(200.f)
	, m_fMaxJumpSpeed(700.f)
{
	SetName(L"RigidBodyScript");

}

RigidBodyScript::RigidBodyScript(const RigidBodyScript& _origin)
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(_origin.m_vForce)
	, m_vAccel(_origin.m_vAccel)
	, m_vAccelA(_origin.m_vAccelA)
	, m_vVelocity(_origin.m_vVelocity)
	, m_fMass(_origin.m_fMass)
	, m_fFricCoeff(_origin.m_fFricCoeff)
	, m_vAccelRatio(_origin.m_vAccelRatio)
	, m_fMaxSpeed(_origin.m_fMaxSpeed)
{
	SetName(L"RigidBodyScript");

}

RigidBodyScript::~RigidBodyScript()
{

}


void RigidBodyScript::start()
{

}

void RigidBodyScript::update()
{

	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;					// 무거울 수록 힘이 약해진다. 
	}


	m_vAccel	+= m_vAccelA;							// 추가 가속도 ex) 중력
	m_vVelocity += m_vAccel * DT;						// 가속도에 따른 속도 변화 

	if (m_bIgnoreGravity)
		m_vVelocity.y = 0.f;							// 중력 무시  ( 떨어지거나 올라가지 않음 ) 


	Vec3 vVelocity_Dir = m_vVelocity;					// 방향 
	vVelocity_Dir.Normalize();

	if (m_vVelocity != Vec3(0.f, 0.f, 0.f))
	{
		Vec3 vFricDir = -vVelocity_Dir;
		Vec3 vFriction = vFricDir * m_fFricCoeff * DT;  // 마찰력 계산

		if (m_vVelocity.Length() < vFriction.Length())
		{
			m_vVelocity = Vec3(0.f, 0.f, 0.f);			// 마찰력에 의한 정지 
		}
		else
		{
			m_vVelocity += vFriction;					// 마찰력 적용  
		}
	}

	
	float fVelocity_Speed = m_vVelocity.Length();		// 스피드
	
	

	// 점프 일 때 속도 제한 
	if (m_vVelocity.y >= 0.f)
	{
		// 점프 속도 제한 
		if (fVelocity_Speed >= m_fMaxJumpSpeed)
			fVelocity_Speed = m_fMaxJumpSpeed;
	}
	// 앞 뒤 좌 우 방향 속도 제한 
	else
	{
		// 속도 제한 
		if (fVelocity_Speed >= m_fMaxSpeed)
			fVelocity_Speed = m_fMaxSpeed;
	}

	// Move 
	if (0.f != fVelocity_Speed)
	{
		Vec3 vDir = m_vVelocity;
		vDir.Normalize();								// 이동 방향 

		Vec3 vPos = Transform()->GetRelativePos();
		vPos += vVelocity_Dir * fVelocity_Speed * DT;
		Transform()->SetRelativePos(vPos);
	}


	m_vForce = Vec3(0.f, 0.f, 0.f);						// 힘 초기화 
	m_vAccel = Vec3(0.f, 0.f, 0.f);						// 가속도 초기화 
	m_vAccelA = Vec3(0.f, 0.f, 0.f);					// 추가 가속도 초기화

}

void RigidBodyScript::lateupdate()
{

}



void RigidBodyScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void RigidBodyScript::OnCollision(CGameObject* _OtherObject)
{

}

void RigidBodyScript::OnCollisionExit(CGameObject* _OtherObject)
{

}

void RigidBodyScript::SaveToScene(FILE* _pFile)
{

}

void RigidBodyScript::LoadFromScene(FILE* _pFile)
{

}

