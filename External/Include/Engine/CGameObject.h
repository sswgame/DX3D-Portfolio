#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }

class CComponent;
class CTransform;
class CCollider2D;
class CAnimator2D;
class CAnimator3D;
class CCamera;
class CLight2D;
class CLight3D;
class CMeshRender;
class CTimeMap;
class CParticleSystem;
class CSkyBox;
class CDecal;
class CScript;
class CRenderComponent;
class CLandScape;
class CCollider3D;
class CFSM;

class CGameObject
	: public CEntity
{
private:
	vector<CGameObject*> m_vecChild;
	vector<CScript*>     m_vecScript;

	CComponent*       m_arrCom[static_cast<UINT>(COMPONENT_TYPE::END)];
	CRenderComponent* m_pRenderComponent;

	CGameObject* m_pParent;
	int          m_iLayerIdx; // ���� ������Ʈ �Ҽ� ���̾� �ε���   
	bool         m_bDead;

	bool m_bActive;
	bool m_bDynamicShadow;  // ���� �׸��� ����
	bool m_bFrustumCulling; // ����ü �ø� ��� ����

private:
	/**
	 * \brief �������谡 ��� ������ ���ӿ�����Ʈ�� ���̾ ��� �ݿ��ϱ� ���� �Լ�
	 * \param _pInnerChild �ڽ��� �ڽ�
	 */
	void CheckLayerRecursive(const CGameObject* _pInnerChild);

public:
	void start();
	void update();
	void lateupdate();
	void finalupdate();
	void finalupdate_module();
	void render();

private:
	void active() const;
	void deactive() const;

public:
	CGameObject*                GetParent() const { return m_pParent; }
	const vector<CGameObject*>& GetChild() const { return m_vecChild; }
	CGameObject*                GetChild(const std::wstring& childName);
	void                        SortChild(std::function<bool(CGameObject*, CGameObject*)> func);
	// Deregister ==> ��� ���(���->�̵��)
	// Unregister ==> ��� �ȵ�(��� x == ���->�̵��, ���ʿ� ��ϵ��� ����)
	void Deregister();
	void DisconnectBetweenParent();
	void Activate();
	void Deactivate();

	bool IsDead() const { return m_bDead; }
	bool IsActive() const { return m_bActive; }
	bool IsAncestor(const CGameObject* _pObject) const;

public:
	void AddChild(CGameObject* _pChild);
	void AddComponent(CComponent* _pComponent);

	CComponent*       GetComponent(COMPONENT_TYPE _eType) const { return m_arrCom[static_cast<UINT>(_eType)]; }
	CRenderComponent* GetRenderComponent() const { return m_pRenderComponent; }

	void DeleteComponent(COMPONENT_TYPE _eType);
	void DeleteScript(UINT _ScriptID);

	void Destroy();

	GET_COMPONENT(Transform, TRANSFORM)
	GET_COMPONENT(MeshRender, MESHRENDER)
	GET_COMPONENT(Camera, CAMERA)
	GET_COMPONENT(Collider2D, COLLIDER2D)
	GET_COMPONENT(Animator2D, ANIMATOR2D)
	GET_COMPONENT(Animator3D, ANIMATOR3D)
	GET_COMPONENT(TileMap, TILEMAP)
	GET_COMPONENT(ParticleSystem, PARTICLESYSTEM)
	GET_COMPONENT(SkyBox, SKYBOX)
	GET_COMPONENT(Light2D, LIGHT2D)
	GET_COMPONENT(Light3D, LIGHT3D)
	GET_COMPONENT(Decal, DECAL)
	GET_COMPONENT(LandScape, LANDSCAPE)
	GET_COMPONENT(Collider3D, COLLIDER3D);
	GET_COMPONENT(FSM, FINITE_STATE_MACHINE);

	const vector<CScript*>& GetScripts() const { return m_vecScript; }
	CScript*                GetScript(UINT _iScriptIndex);
	CScript*                GetScriptByName(const wstring& _strScriptName) const;

	int  GetLayerIndex() const { return m_iLayerIdx; }
	void RenewLayerIndex(int _newLayerIndex);

	template <typename T>
	T* GetScript();

public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;
	CLONE(CGameObject)

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CGameObject();
	CGameObject(const CGameObject& _origin);
	virtual ~CGameObject();

	friend class CEventMgr;
	friend class CScene;
	friend class CLayer;
	friend class CCollider3D;
};

template <typename T>
T* CGameObject::GetScript()
{
	for (auto& pScript : m_vecScript)
	{
		if (dynamic_cast<T*>(pScript))
		{
			return static_cast<T*>(pScript);
		}
	}
	return nullptr;
}
