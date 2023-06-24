#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer
	: public CEntity
{
private:
	vector<CGameObject*> m_vecRoot;   // ���̾ ���� �ֻ��� �θ� ������Ʈ
	vector<CGameObject*> m_vecObj;    // �� ������ ���� �ľǵǴ� ���̾� �Ҽ� ��� ������Ʈ
	int                  m_iLayerIdx; // Scene ������ �ε���

private:
	void ClearObjectLayerIndex(CGameObject* _pObject);
	void RegisterObjectAsRoot(CGameObject* _pObj) { m_vecRoot.push_back(_pObj); }
	void RegisterObject(CGameObject* _pObj) { m_vecObj.push_back(_pObj); }
	void DeregisterObject(CGameObject* _pGameObject);
	void Clear() { m_vecObj.clear(); }

public:
	void start();
	void update();
	void lateupdate();
	void finalupdate();

public:
	vector<CGameObject*>& GetRootObjects() { return m_vecRoot; }
	vector<CGameObject*>& GetObjects() { return m_vecObj; }

	CGameObject* FindRootObject(wstring _wstrNames) const;
	void         RenewLayerIdx(int _iLayerIndex);

	int GetLayerIdx() const { return m_iLayerIdx; }
	CLONE_DISABLE(CLayer)

public:
	CLayer();
	virtual ~CLayer();

	friend class CSceneMgr;
	friend class CScene;
	friend class CGameObject;
	friend class CEventMgr;
};
