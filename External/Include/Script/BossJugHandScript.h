#pragma once
#include <Engine\CScript.h>

#include "HandStateMgrScript.h"

class BossJugHandScript :
	public CScript
{
private:
	HandStateMgrScript* m_pHandStateMgr;

	Vec3            m_vDirection;
	Vec3            m_vPrevDirection;


	float					m_fRunningTime;
	float					m_fSpeed;

	int						m_iOwnerHandIdx;
	int						m_iCurAttackHandIdx;

	bool					m_bAnimDone;

public:
	// [ GET ]
	int GetHandIndexNumber() { return m_iOwnerHandIdx; }
	int GetCurAttackHandIdx() { return m_iCurAttackHandIdx; }
	float GetRunningTime() { return m_fRunningTime; }
	float GetSpeed() { return m_fSpeed; }
	Vec3 GetCurDir() { return m_vDirection; }
	Vec3 GetPlayerPosition();

	int  GetSavedMidFrm() { return GetOwner()->GetScript<HandStateMgrScript>()->GetSavedMidFrame(); }
	bool Get1stAttackDone() { return GetOwner()->GetScript<HandStateMgrScript>()->Get1stAttackDone(); }
	bool Get2ndAttackDone() { return GetOwner()->GetScript<HandStateMgrScript>()->Get2ndAttackDone(); }
	bool Get3rdAttackDone() { return GetOwner()->GetScript<HandStateMgrScript>()->Get3rdAttackDone(); }


	// [ SET ]
	void SetHandStateMgr(HandStateMgrScript* _pMgr) { m_pHandStateMgr = _pMgr; }
	void SetRunningTime(float _time) { m_fRunningTime = _time; }
	void SetCurAnimationPlayDone(bool _TorF) { m_bAnimDone = _TorF; }
	void SetMonsterDir(Vec3 _dir) { m_vDirection = _dir; }
	void SetMonsterSpeed(float _speed) { m_fSpeed = _speed; }

	void SetAllAttackDone(bool _TorF) { GetOwner()->GetScript<HandStateMgrScript>()->SetAllAttackDone(_TorF); }
	void Set1stAttackDone(bool _TorF) { GetOwner()->GetScript<HandStateMgrScript>()->Set1stAttackDone(_TorF); }
	void Set2ndAttackDone(bool _TorF) { GetOwner()->GetScript<HandStateMgrScript>()->Set2ndAttackDone(_TorF); }
	void Set3rdAttackDone(bool _TorF) { GetOwner()->GetScript<HandStateMgrScript>()->Set3rdAttackDone(_TorF); }
	void SetSaveMidFrm(int _frame) { GetOwner()->GetScript<HandStateMgrScript>()->SetSaveMidFrame(_frame); }

public:
	void InitMonsterStat();


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject)	override;
	virtual void OnCollision(CGameObject* _OtherObject)			override;
	virtual void OnCollisionExit(CGameObject* _OtherObject)		override;

public:
	virtual void SaveToScene(FILE* _pFile)						override;
	virtual void LoadFromScene(FILE* _pFile)					override;

public:
	CLONE(BossJugHandScript);
	BossJugHandScript();
	BossJugHandScript(const BossJugHandScript& _origin);
	virtual ~BossJugHandScript();


};

