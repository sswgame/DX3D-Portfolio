#include "pch.h"
#include "CRigidBody.h"


#include "CTimeMgr.h"
#include "CTransform.h"
#include "CSerializer.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_fGravityAccelAlpha(-2000.f)
	, m_fMass(1.f)
	, m_fFricCoeff(500.f)
	, m_vAccelRatio(1.f)
	, m_fMaxMoveSpeed(200.f)
	, m_fMaxJumpSpeed(1200.f)
	, m_fMinJumpSpeed(-550.f)
	, m_fCurVelocity_MoveSpeed{0}
	, m_fCurVelocity_JumpSpeed{0}
	, m_bIgnoreGravity{false}
	, m_bOnGround{false} {}

CRigidBody::CRigidBody(const CRigidBody& _origin)
	: CComponent(_origin)
	, m_vForce(_origin.m_vForce)
	, m_vAccel(_origin.m_vAccel)
	, m_vAccelA(_origin.m_vAccelA)
	, m_vVelocity(_origin.m_vVelocity)
	, m_fGravityAccelAlpha(_origin.m_fGravityAccelAlpha)
	, m_fMass(_origin.m_fMass)
	, m_fFricCoeff(_origin.m_fFricCoeff)
	, m_vAccelRatio(_origin.m_vAccelRatio)
	, m_fMaxMoveSpeed(_origin.m_fMaxMoveSpeed)
	, m_fMaxJumpSpeed(_origin.m_fMaxJumpSpeed)
	, m_fMinJumpSpeed(_origin.m_fMinJumpSpeed)
	, m_fCurVelocity_MoveSpeed{0}
	, m_fCurVelocity_JumpSpeed{0}
	, m_bIgnoreGravity{false}
	, m_bOnGround{false} {}


CRigidBody::~CRigidBody() = default;


void CRigidBody::update()
{
	Vec3 vPrevPos = Transform()->GetRelativePos();		// ���� ��ġ 

	{
		UpdateGravity();
		Vec3 vNormalized_VelocityDir = UpdateVelocity();	// �ӵ� ������Ʈ 
		UpdateFriction(vNormalized_VelocityDir);			// ������ ���� 
		UpdateLimitSpeed(vNormalized_VelocityDir);			// �ӵ����� ������Ʈ 	

		Move(vNormalized_VelocityDir);						// (�յ��¿�) �̵� 
		Jump(vNormalized_VelocityDir);						// (����)     �̵� 

		m_vForce  = Vec3(0.f, 0.f, 0.f);						// ��			�ʱ�ȭ 
		m_vAccel  = Vec3(0.f, 0.f, 0.f);						// ���ӵ�		�ʱ�ȭ 
		m_vAccelA = Vec3(0.f, 0.f, 0.f);					// �߰� ���ӵ�	�ʱ�ȭ
	}

	Vec3 vCurPos = Transform()->GetRelativePos();		// ���� ��ġ 
	m_vMovedDist = vPrevPos - vCurPos; 					// �̵� �Ÿ� 

	// TEST : �ϴ� 0.f �Ʒ��� �ȶ������� ���Ƶ� - Nav Mesh �� ������ ���� 
	//Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	//if (vPos.y < 0.f)
	//{
	//	vPos.y = 0.f;
	//	GetOwner()->Transform()->SetRelativePos(vPos);
	//	m_bOnGround = true;
	//}
	//else
	//	m_bOnGround = false;
}

void CRigidBody::UpdateGravity()
{
	// �� ���� ���� �ʴٸ� 
	if (!m_bOnGround)
	{
		if (!m_bIgnoreGravity)
		{
			// �������� ���ӵ� ���� -> m_fMinJUmpSpeed �� ���� �ӵ� ���� �ɸ� 
			SetAccelAlpha(Vec3(0.f, m_fGravityAccelAlpha, 0.f));
		}
	}
	else
	{
		m_vVelocity.y = 0.f;
	}
}

Vec3 CRigidBody::UpdateVelocity()
{
	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;	// ���ſ� ���� ���� ��������. 
	}

	m_vAccel += m_vAccelA;				// �߰� ���ӵ� ex) �߷�
	m_vVelocity += m_vAccel * DT;		// ���ӵ��� ���� �ӵ� ��ȭ 

	if (m_bIgnoreGravity)
		m_vVelocity.y = 0.f;			// �߷� ����  ( �������ų� �ö��� ���� ) 

	Vec3 vNormalized_VelocityDir = m_vVelocity;					// ���� 
	vNormalized_VelocityDir.Normalize();

	return vNormalized_VelocityDir;
}


void CRigidBody::UpdateFriction(Vec3 _vVelocityDir)
{
	if (m_vVelocity != Vec3(0.f, 0.f, 0.f))
	{
		Vec3 vFricDir  = -1 * _vVelocityDir;
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
}

void CRigidBody::UpdateLimitSpeed(Vec3 _vVelocityDir)
{
	m_fCurVelocity_MoveSpeed = m_vVelocity.Length();		// X,Z	���ǵ�
	m_fCurVelocity_JumpSpeed = m_vVelocity.y;			// Y	���ǵ� 


	// ���� �� �� �ӵ� ���� 
	if (m_vVelocity.y >= 0.f)
	{
		// ���� �ӵ� ���� 
		if (m_fCurVelocity_JumpSpeed >= m_fMaxJumpSpeed)
			m_fCurVelocity_JumpSpeed = m_fMaxJumpSpeed;
	}
	// �� �� �� �� ���� �ӵ� ���� 
	else
	{
		// �ӵ� ���� 
		if (m_fCurVelocity_MoveSpeed >= m_fMaxMoveSpeed)
			m_fCurVelocity_MoveSpeed = m_fMaxMoveSpeed;
	}

	// �������� �ӵ� ����
	if (m_fCurVelocity_JumpSpeed <= m_fMinJumpSpeed)
	{
		m_fCurVelocity_JumpSpeed = m_fMinJumpSpeed;
	}
}

void CRigidBody::Move(Vec3 _vVelocityDir)
{
	// Move 
	if (0.f != m_fCurVelocity_MoveSpeed)
	{
		Vec3 vDir = m_vVelocity;
		vDir.Normalize();								// �̵� ���� 

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += _vVelocityDir.x * m_fCurVelocity_MoveSpeed * DT;
		vPos.z += _vVelocityDir.z * m_fCurVelocity_MoveSpeed * DT;
		Transform()->SetRelativePos(vPos);
	}
}


void CRigidBody::Jump(Vec3 _vVelocityDir)
{
	// Jump
	if (0.f != m_fCurVelocity_JumpSpeed)
	{
		Vec3 vDir = _vVelocityDir;										// �̵� ���� 

		Vec3 vPos = Transform()->GetRelativePos();

		if (vDir.y >= 0.f)
			vPos.y += _vVelocityDir.y * m_fCurVelocity_JumpSpeed * DT;		// JUMP
		else
			vPos.y += _vVelocityDir.y * m_fCurVelocity_JumpSpeed * -1 * DT;	// FALL

		Transform()->SetRelativePos(vPos);
	}
}

void CRigidBody::lateupdate() {}

void CRigidBody::finalupdate() {}

void CRigidBody::finalupdate_debug() {}

void CRigidBody::render() {}

void CRigidBody::SaveToScene(FILE* _pFile) {}

void CRigidBody::LoadFromScene(FILE* _pFile) {}

void CRigidBody::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << "RIGIDBODY INFO" << YAML::Value << YAML::Flow << YAML::BeginMap;

	emitter << YAML::Key << NAME_OF(m_vForce) << YAML::Value << m_vForce;
	emitter << YAML::Key << NAME_OF(m_vAccel) << YAML::Value << m_vAccel;
	emitter << YAML::Key << NAME_OF(m_vAccelA) << YAML::Value << m_vAccelA;
	emitter << YAML::Key << NAME_OF(m_vVelocity) << YAML::Value << m_vVelocity;

	emitter << YAML::Key << NAME_OF(m_fGravityAccelAlpha) << YAML::Value << m_fGravityAccelAlpha;
	emitter << YAML::Key << NAME_OF(m_fMass) << YAML::Value << m_fMass;
	emitter << YAML::Key << NAME_OF(m_fFricCoeff) << YAML::Value << m_fFricCoeff;
	emitter << YAML::Key << NAME_OF(m_vAccelRatio) << YAML::Value << m_vAccelRatio;


	emitter << YAML::Key << NAME_OF(m_fMaxMoveSpeed) << YAML::Value << m_fMaxMoveSpeed;
	emitter << YAML::Key << NAME_OF(m_fMaxJumpSpeed) << YAML::Value << m_fMaxJumpSpeed;
	emitter << YAML::Key << NAME_OF(m_fMinJumpSpeed) << YAML::Value << m_fMinJumpSpeed;

	emitter << YAML::Key << NAME_OF(m_fCurVelocity_MoveSpeed) << YAML::Value << m_fCurVelocity_MoveSpeed;
	emitter << YAML::Key << NAME_OF(m_fCurVelocity_JumpSpeed) << YAML::Value << m_fCurVelocity_JumpSpeed;

	emitter << YAML::Key << NAME_OF(m_bIgnoreGravity) << YAML::Value << m_bIgnoreGravity;
	emitter << YAML::Key << NAME_OF(m_bOnGround) << YAML::Value << m_bOnGround;
}


void CRigidBody::Deserialize(const YAML::Node& node)
{
	YAML::Node RigidBodyInfoNode = node["RIGIDBODY INFO"];

	m_vForce    = RigidBodyInfoNode[NAME_OF(m_vForce)].as<Vec3>();
	m_vAccel    = RigidBodyInfoNode[NAME_OF(m_vAccel)].as<Vec3>();
	m_vAccelA   = RigidBodyInfoNode[NAME_OF(m_vAccelA)].as<Vec3>();
	m_vVelocity = RigidBodyInfoNode[NAME_OF(m_vVelocity)].as<Vec3>();

	m_fGravityAccelAlpha = RigidBodyInfoNode[NAME_OF(m_fGravityAccelAlpha)].as<float>();
	m_fMass              = RigidBodyInfoNode[NAME_OF(m_fMass)].as<float>();
	m_fFricCoeff         = RigidBodyInfoNode[NAME_OF(m_fFricCoeff)].as<float>();
	m_vAccelRatio        = RigidBodyInfoNode[NAME_OF(m_vAccelRatio)].as<float>();

	m_fMaxMoveSpeed = RigidBodyInfoNode[NAME_OF(m_fMaxMoveSpeed)].as<float>();
	m_fMaxJumpSpeed = RigidBodyInfoNode[NAME_OF(m_fMaxJumpSpeed)].as<float>();
	m_fMinJumpSpeed = RigidBodyInfoNode[NAME_OF(m_fMinJumpSpeed)].as<float>();

	m_fCurVelocity_MoveSpeed = RigidBodyInfoNode[NAME_OF(m_fCurVelocity_MoveSpeed)].as<float>();
	m_fCurVelocity_JumpSpeed = RigidBodyInfoNode[NAME_OF(m_fCurVelocity_JumpSpeed)].as<float>();

	m_bIgnoreGravity = RigidBodyInfoNode[NAME_OF(m_bIgnoreGravity)].as<bool>();
	m_bOnGround      = RigidBodyInfoNode[NAME_OF(m_bOnGround)].as<bool>();
}
