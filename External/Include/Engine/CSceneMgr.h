#pragma once

class CScene;
class CGameObject;

class CSceneMgr
	: public CSingleton<CSceneMgr>
{
	friend class CSingleton<CSceneMgr>;
private:
	SINGLE(CSceneMgr);

private:
	CScene* m_pCurScene;
public:
	CScene* GetCurScene() const { return m_pCurScene; }

	void ChangeScene(CScene* _pNextScene);

	void SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, wstring _strLayerName);
	void AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject);
	void DisconnectParent(CGameObject* _pObject);
	void ClearLayer();

	void ChangeObjectLayerIndex(CGameObject* _pObject, UINT _iLayerIdx);
	void SwapLayer(UINT _iLayerIdx_1, UINT _iLayerIdx_2);

	// DESTROY 
	void DeleteObject(CGameObject* _pDestroyObject);


	CGameObject* FindObjectByName(const wstring& _strName);
	void         FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut);

public:
	void init();
	void progress();
};
