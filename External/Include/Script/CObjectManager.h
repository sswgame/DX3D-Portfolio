#pragma once
#include <Engine/CMgrScript.h>

class CObjectManager final
	: public CMgrScript<CObjectManager>
{
private:
	CGameObject* m_pPlayer;
	CGameObject* m_pPlayerCamera;
	CGameObject* m_pBossCombatMgr;


public:
	CGameObject* GetPlayer() const { return m_pPlayer; }
	CGameObject* GetPlayerCamera() const { return m_pPlayerCamera; }
	CGameObject* GetBossCombatMgr() const { return m_pBossCombatMgr; }

public:
	void start() override;

public:
	CObjectManager();
	virtual ~CObjectManager();
};
