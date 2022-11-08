#pragma once
#include <Engine\CScript.h>
#include "CHand_StateMgr.h"

class BossJugHandScript :
	public CScript
{
private:
	CHand_StateMgr* m_pStateMgr;		// 상태 정보 관리
	Vec3					m_vDirection;
	Vec3					m_vPrevDirection;

	CGameObject* m_pBossObj;

	float					m_fRunningTime;
	float					m_fSpeed;

	int						m_iOwnerHandIdx;
	int						m_iCurAttackHandIdx;

	bool					m_bAnimDone;

public:
	// [ GET ]
	int GetHandIndexNumber() { return m_iOwnerHandIdx; }
	int GetSavedCurFrm() {/*return m_pBossObj->GetSavedCurFrm();*/ }
	float GetRunningTime() { return m_fRunningTime; }
	float GetSpeed() { return m_fSpeed; }
	Vec3 GetPlayerPosition();
	Vec3 GetCurDir() { return m_vDirection; }

	//bool Get1stAttackDone() { /*return m_pBossObj->Get1stAttackDone(); */ }
	//bool Get2ndAttackDone() { /*return m_pBossObj->Get2ndAttackDone(); */ }
	//bool Get3rdAttackDone() { /*return m_pBossObj->Get3rdAttackDone(); */ }


	// [ SET ]
	void SetRunningTime(float _time) { m_fRunningTime = _time; }
	void SetCurAnimationPlayDone(bool _TorF) { m_bAnimDone = _TorF; }
	void SetMonsterDir(Vec3 _dir) { m_vDirection = _dir; }
	void SetMonsterSpeed(float _speed) { m_fSpeed = _speed; }

	void Set1stAttackDone(bool _TorF) { /*m_pBossObj->Set1stAttackDone(_TorF); */ }
	void Set2ndAttackDone(bool _TorF) { /*m_pBossObj->Set2ndAttackDone(_TorF); */ }
	void Set3rdAttackDone(bool _TorF) { /*m_pBossObj->Set3rdAttackDone(_TorF); */ }

public:
	void InitMonsterStat();
	void SaveCurFrm(int _frame) {/*m_pBossObj->SaveCurFrame(_frame);*/ }

	// void StartAttack() { m_pBossObj->StartAttack(); }

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
	BossJugHandScript(const BossJugHandScript* _origin);
	virtual ~BossJugHandScript();


};

