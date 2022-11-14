#include "pch.h"
#include "CObjectManager.h"

#include "PlayerScript.h"

CObjectManager::CObjectManager()
	: m_pPlayer{nullptr}
	, m_pPlayerCamera{nullptr}
{
}

CObjectManager::~CObjectManager()
{
}

void CObjectManager::start()
{
	/*CLayer* pPlayerLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(GAME::LAYER::PLAYER);
	m_pPlayer            = pPlayerLayer->GetRootObjects().front();
	assert(m_pPlayer && "NO PLAYER");

	PlayerScript* pPlayerScript = (PlayerScript*)(m_pPlayer->GetScript((int)(SCRIPT_TYPE::PLAYERCAMSCRIPT)));
	m_pPlayerCamera             = pPlayerScript->GetCamera();
	assert(m_pPlayerCamera && "NO PLAYER CAMERA");*/
}
