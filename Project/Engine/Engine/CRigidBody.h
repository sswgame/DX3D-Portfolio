#pragma once
#include "CComponent.h"

class CRigidBody
	: public CComponent
{
private:
	Vec3 m_vForce;           // ũ�� , ���� 
	Vec3 m_vAccel;           // ���ӵ�  
	Vec3 m_vAccelA;          // �߰� ���ӵ� 
	Vec3 m_vVelocity;        // �ӵ� ( ũ�� : �ӷ� , ���� )

	float m_fGravityAccelAlpha;	// ���� �ӵ�  
	float m_fMass;            // ���� 
	float m_fFricCoeff;       // ���� ��� 
	float m_vAccelRatio;      // Accel ���� ���� 

	float m_fMaxMoveSpeed;
	float m_fMaxJumpSpeed;    // ���� �ִ� �ӵ�
	float m_fMinJumpSpeed;	// ���� �ּ� �ӵ�

	float m_fCurVelocity_MoveSpeed;
	float m_fCurVelocity_JumpSpeed;

	bool m_bIgnoreGravity;   // �߷� ���� ���� 
	bool m_bOnGround;		// ���� �����ߴ��� ����  

	Vec3 m_vMovedDist;		// ������ �Ÿ� 


public:
	void AddForce(Vec3 _vF) { m_vForce += _vF; }
	void AddVelocity(Vec3 _v) { m_vVelocity += _v; }
	void AddAccelAlpha(Vec3 _vAccelA) { m_vAccelA += _vAccelA; }

	bool IsOnGround() const { return m_bOnGround; }
	void SetOnGround(bool _b) { m_bOnGround = _b; }

	Vec3 GetMoveDist(Vec3 _vDist) { return m_vMovedDist; }

public:
	void  SetForce(Vec3 _vForce) { m_vForce = _vForce; }
	void  SetVelocity(Vec3 _v) { m_vVelocity = _v; }		// �ʱ�ӵ��� ������ �մϴ�. 
	void  SetMass(float _fMass) { m_fMass = _fMass; }
	void  SetFricCoeff(float _fFricCoeff) { m_fFricCoeff = _fFricCoeff; }
	void  SetAccelAlpha(Vec3 _vAccelA) { m_vAccelA = _vAccelA; }
	void  SetMaxSpeed(float _fMaxSpeed) { m_fMaxMoveSpeed = _fMaxSpeed; }
	void  SetIgnoreGravity(bool _b) { m_bIgnoreGravity = _b; }
	void  SetGravityAccleAlpha(float _fGravityAccelAlpha) { m_fGravityAccelAlpha = _fGravityAccelAlpha; }
	Vec3  GetForce() { return m_vForce; }
	Vec3  GetVelocity() { return m_vVelocity; }
	Vec3  GetAccelAlpha() { return m_vAccelA; }
	float GetMass() { return m_fMass; }
	float GetFricCoeff() { return m_fFricCoeff; }

public:
	void update() override;
	void lateupdate() override;
	void finalupdate() override;
	void finalupdate_debug() override;
	void render();

public:
	void UpdateGravity();
	Vec3 UpdateVelocity();							// �ӵ�   ������Ʈ 
	void UpdateFriction(Vec3 _vVelocityDir);							// ������ ������Ʈ
	void UpdateLimitSpeed(Vec3 _vVelocityDir);		// �̵�/����/���� �ӵ� ���� 
	void Move(Vec3 _vVelocityDir);	// �̵� 
	void Jump(Vec3 _vVelocityDir);	// ����/���� 


public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;
	CLONE(CRigidBody);

public:
	void Serialize(YAML::Emitter& emitter) override;;
	void Deserialize(const YAML::Node& node) override;

public:
	CRigidBody();
	CRigidBody(const CRigidBody& _origin);
	virtual ~CRigidBody();
};
