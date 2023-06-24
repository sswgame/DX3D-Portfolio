#pragma once
#include <Engine\CState.h>

class CAnimation3D;
class FieldM_StateMgr;

class Homon_Walk :
	public CState
{
public:
	CAnimation3D* m_pAnimation;
	FieldM_StateMgr* m_pOwnerMGR;

	float				m_fSpeed;			// �ӵ� 

	bool				m_bDetectTarget;

	Vec3				m_vPatrolPoint;		// Patrol ������ ( ���� )	
	float				m_fPatrolRange;		// Patrol ���� 
	Vec3				m_vPatrolTargetPos;	// Patrol target Pos 
	bool				m_bPatrolFinish;	// Patrol Target Pos �� ���� �ߴ��� ����


public:
	void Enter() override;
	void Exit() override;

public:
	void Update() override;

	void Trace(CGameObject* _pTarget);
	void Patrol();
	void UpdatePatrolPoint(Vec3 _vPos);
	void UpdatePatrolTargetPos();
	void UpdatePos();


public:
	CLONE(Homon_Walk);
	Homon_Walk();
	virtual ~Homon_Walk();
};

