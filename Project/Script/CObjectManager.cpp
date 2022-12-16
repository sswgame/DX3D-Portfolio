#include "pch.h"
#include "CObjectManager.h"

#include <Engine/CFSM.h>

#include "ObjMgrState_IDLE.h"
#include "ObjMgrState_LOADING.h"
#include "PlayerScript.h"

CObjectManager::CObjectManager()
	: m_pPlayer{nullptr}
	, m_pPlayerCamera{nullptr}
	, m_pBossCombatMgr{nullptr}
	, m_currentMap{MAP_TYPE::_01} {}

CObjectManager::~CObjectManager() = default;


void CObjectManager::start()
{
	m_arrStartingPoint[(UINT)MAP_TYPE::_01] = Vec3{-80, -209, - 8806};
	CFSM* pFSM                              = new CFSM{};
	GetOwner()->AddComponent(pFSM);
	pFSM->AddState(L"IDLE", new ObjMgrState_IDLE{});
	pFSM->AddState(L"LOADING", new ObjMgrState_LOADING{});
	pFSM->ChangeState(L"IDLE");
}

void CObjectManager::RemoveFromDontDestroyList(CGameObject* pGameObject)
{
	m_vecDonDestroy.erase(std::remove(m_vecDonDestroy.begin(), m_vecDonDestroy.end(), pGameObject));
}

CGameObject* CObjectManager::IsInDontDestroyList(const CGameObject* pGameObject)
{
	const auto iter = std::find(m_vecDonDestroy.begin(), m_vecDonDestroy.end(), pGameObject);
	if (iter != m_vecDonDestroy.end())
	{
		return *iter;
	}
	return nullptr;
}

void CObjectManager::AddToDontDestroy(CGameObject* pGameObject)
{
	if (IsInDontDestroyList(pGameObject))
	{
		return;
	}
	m_vecDonDestroy.push_back(pGameObject);
}

void CObjectManager::SetSceneObject(CGameObject* pGameObject, MAP_TYPE type)
{
	auto&      vecSceneObject = m_mapSceneObject[type];
	const auto iter           = std::find(vecSceneObject.begin(), vecSceneObject.end(), pGameObject);
	if (iter == vecSceneObject.end())
	{
		vecSceneObject.push_back(pGameObject);
	}
}

const std::vector<CGameObject*>& CObjectManager::GetSceneObjectList(MAP_TYPE _type) const
{
	LOG_ASSERT(_type!=MAP_TYPE::END, "INVALID MAP_TYPE");

	return m_mapSceneObject.at(_type);
}

bool CObjectManager::CheckAllMonsterDead() const
{
	CLayer*     pMonsterLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER");
	const auto& vecMonster    = pMonsterLayer->GetRootObjects();

	const size_t count = std::count_if(vecMonster.begin(),
	                                   vecMonster.end(),
	                                   [](const CGameObject* pMonster)
	                                   {
		                                   return pMonster->GetName() != L"JUG" && pMonster->IsDead();
	                                   });

	//보스만 남았거나, 존재는 하지만 모두 죽은 경우
	if (vecMonster.size() == 1 || vecMonster.size() - 1 == count)
	{
		return true;
	}

	return false;
}
