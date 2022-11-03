#pragma once
#include <Engine/CScript.h>
#include "CObjKeyMgr.h"
class CStateMgr;
class CObjKeyMgr;
class CPlayerStat;
class CCamera;


class PlayerScript
	: public CScript
{
private:
	CObjKeyMgr*		m_pKeyMgr;			//  Ű	���� ���� 
	CStateMgr*		m_pStateMgr;		// ���� ���� ����
	CPlayerStat*	m_pStat;			// ���� ���� ���� 
	CGameObject*	m_pCamera;			// ī�޶� ���� 


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
	CGameObject* GetCamera() { return m_pCamera; }


	void SetCamera(CGameObject* _pCameraObj) { m_pCamera = _pCameraObj; }

public:
	void UpdateCamera();


	CLONE(PlayerScript)

public:
	PlayerScript();
	virtual ~PlayerScript();
};
