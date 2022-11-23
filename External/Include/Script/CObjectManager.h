#pragma once
#include <Engine/CMgrScript.h>

class CObjectManager final
	: public CMgrScript<CObjectManager>
{
private:
	CGameObject* m_pPlayer;
	CGameObject* m_pPlayerCamera;

public:
	CGameObject* GetPlayer() const { return m_pPlayer; }
	CGameObject* GetPlayerCamera() const { return m_pPlayerCamera; }

public:
	void start() override;

public:
	CObjectManager();
	virtual ~CObjectManager();
};
