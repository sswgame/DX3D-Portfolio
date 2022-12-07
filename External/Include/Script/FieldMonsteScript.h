#pragma once
#include <Engine/CScript.h>

#include "FieldM_StateMgr.h"

class CGameObject;

enum class FieldMonsterType
{
	HOMONCULUS,
	DEUXIEMIE,
	NONE
};

class FieldMonsteScript :
	public CScript
{
private:
	FieldM_StateMgr* m_pMonsterMgr;
	Vec3 m_vColliderSize;

	CGameObject* m_pPlayer;
	CGameObject* m_pOwnerMonster;

	float m_fDetachRange;
	float m_fAttackRange;
	float m_fCurDistance;

	int m_iFullHP;
	int m_iCurHP;

	float m_fCoolTime;
	bool	m_bIsChasing;

	bool m_bCurAnimationDone;

	FieldMonsterType m_eMonsterType;

public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	void SetMonsterFullHP(int _hp) { m_iFullHP = _hp; }

	void SetMonsterHP(int _curHP) { m_iCurHP = _curHP; }
	int GetMonsterHP() { return m_iCurHP; }

	void SetDetectRange(float _range) { m_fDetachRange = _range; }
	void SetAttackRange(float _range) { m_fAttackRange = _range; }
	void SetColliderSize(Vec3 _size) { m_vColliderSize = _size; }
	void SetCoolTimeReset() { m_fCoolTime = 0.f; }
	void SetCurAnimationDone() { m_bCurAnimationDone = true; }

	void SetFieldMonsterType(FieldMonsterType _type) { m_eMonsterType = _type; }
	FieldMonsterType GetFieldMonsterType() { return m_eMonsterType; }

	FieldM_StateMgr* GetMonsterMGR() { return m_pMonsterMgr; }

public:
	bool DetectPlayer();
	void ChangeState();

public:
	CLONE(FieldMonsteScript)

public:
	FieldMonsteScript(const FieldMonsteScript& _origin);
	FieldMonsteScript();
	virtual ~FieldMonsteScript();
};

