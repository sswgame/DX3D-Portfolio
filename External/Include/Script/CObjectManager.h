#pragma once
#include <Engine/CMgrScript.h>

enum class MAP_TYPE
{
	_01,
	_02,

	END
};

class CObjectManager final
	: public CMgrScript<CObjectManager>
{
private:
	CGameObject* m_pPlayer;
	CGameObject* m_pPlayerCamera;
	CGameObject* m_pBossCombatMgr;

	std::vector<CGameObject*> m_vecDonDestroy;

	std::array<Vec3, (UINT)MAP_TYPE::END> m_arrStartingPoint;

	std::map<MAP_TYPE, std::vector<CGameObject*>> m_mapSceneObject;
	MAP_TYPE                                      m_currentMap;

public:
	CGameObject* GetPlayer() const { return m_pPlayer; }
	CGameObject* GetPlayerCamera() const { return m_pPlayerCamera; }
	CGameObject* GetBossCombatMgr() const { return m_pBossCombatMgr; }

	void SetBossCombatMgr(CGameObject* _pMgr) { m_pBossCombatMgr = _pMgr; }
	void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }

	void                             RemoveFromDontDestroyList(CGameObject* pGameObject);
	CGameObject*                     IsInDontDestroyList(const CGameObject* pGameObject);
	void                             AddToDontDestroy(CGameObject* pGameObject);
	const std::vector<CGameObject*>& GetDontDestroyList() const { return m_vecDonDestroy; }

	Vec3 GetStartingPoint(MAP_TYPE _type) const { return m_arrStartingPoint[static_cast<UINT>(_type)]; }

	MAP_TYPE GetCurrentMapType() const { return m_currentMap; }
	void     SetMapType(MAP_TYPE _type) { m_currentMap = _type; }

	void                             SetSceneObject(CGameObject* pGameObject, MAP_TYPE type);
	const std::vector<CGameObject*>& GetSceneObjectList(MAP_TYPE _type) const;

public:
	void start() override;

public:
	CObjectManager();
	virtual ~CObjectManager();
};
