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
		m_vAccel = m_vForce / m_fMass;					// ���ſ� ���� ���� ��������. 
	}


	m_vAccel	+= m_vAccelA;							// �߰� ���ӵ� ex) �߷�
	m_vVelocity += m_vAccel * DT;						// ���ӵ��� ���� �ӵ� ��ȭ 

	if (m_bIgnoreGravity)
		m_vVelocity.y = 0.f;							// �߷� ����  ( �������ų� �ö��� ���� ) 


	Vec3 vVelocity_Dir = m_vVelocity;					// ���� 
	vVelocity_Dir.Normalize();

	if (m_vVelocity != Vec3(0.f, 0.f, 0.f))
	{
		Vec3 vFricDir = -vVelocity_Dir;
		Vec3 vFriction = vFricDir * m_fFricCoeff * DT;  // ������ ���

		if (m_vVelocity.Length() < vFriction.Length())
		{
			m_vVelocity = Vec3(0.f, 0.f, 0.f);			// �����¿� ���� ���� 
		}
		else
		{
			m_vVelocity += vFriction;					// ������ ����  
		}
	}

	
	float fVelocity_Speed = m_vVelocity.Length();		// ���ǵ�
	
	

	// ���� �� �� �ӵ� ���� 
	if (m_vVelocity.y >= 0.f)
	{
		// ���� �ӵ� ���� 
		if (fVelocity_Speed >= m_fMaxJumpSpeed)
			fVelocity_Speed = m_fMaxJumpSpeed;
	}
	// �� �� �� �� ���� �ӵ� ���� 
	else
	{
		// �ӵ� ���� 
		if (fVelocity_Speed >= m_fMaxSpeed)
			fVelocity_Speed = m_fMaxSpeed;
	}

	// Move 
	if (0.f != fVelocity_Speed)
	{
		Vec3 vDir = m_vVelocity;
		vDir.Normalize();								// �̵� ���� 

		Vec3 vPos = Transform()->GetRelativePos();
		vPos += vVelocity_Dir * fVelocity_Speed * DT;
		Transform()->SetRelativePos(vPos);
	}


	m_vForce = Vec3(0.f, 0.f, 0.f);						// �� �ʱ�ȭ 
	m_vAccel = Vec3(0.f, 0.f, 0.f);						// ���ӵ� �ʱ�ȭ 
	m_vAccelA = Vec3(0.f, 0.f, 0.f);					// �߰� ���ӵ� �ʱ�ȭ

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

