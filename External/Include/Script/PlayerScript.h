#pragma once
#include <Engine/CScript.h>
#include "CObjKeyMgr.h"
class CStateMgr;
class CObjKeyMgr;
class CPlayerStat;

class PlayerScript
	: public CScript
{
private:
	CObjKeyMgr*		m_pKeyMgr;			//  키	정보 관리 
	CStateMgr*		m_pStateMgr;		// 상태 정보 관리
	CPlayerStat*	m_pStat;			// 스탯 정보 관리 

	Vec3			m_vDirection;		// 현재 플레이어가 움직이고 있는 방향 ( 단위벡터로 정의할 것)


public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

public:
	CObjKeyMgr* GetObjKeyMgr() { return m_pKeyMgr; }
	CStateMgr* GetStateMgr() { return m_pStateMgr; }
	CPlayerStat* GetPlayerStat() { return m_pStat; }

	Vec3 GetDirection() { return m_vDirection; }

public:
	void UpdateDirection(tKey_Zip _keyInfo);

	CLONE(PlayerScript)

public:
	PlayerScript();
	virtual ~PlayerScript();
};
