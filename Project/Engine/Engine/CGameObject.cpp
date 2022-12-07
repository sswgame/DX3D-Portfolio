#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CEventMgr.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CLight2D.h"
#include "CRenderComponent.h"
#include "CNaviMap.h"
#include "CNaviAgent.h"
#include "CUIBase.h"

#include "CScript.h"

namespace GAMEOBJECT
{
	constexpr int INVALID_INDEX = -1;
}

CGameObject::CGameObject()
	: m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_pParent(nullptr)
	, m_iLayerIdx(GAMEOBJECT::INVALID_INDEX)
	, m_bDead(false)
	, m_bActive(true)
	, m_bDynamicShadow(false)
	, m_bFrustumCulling(false)
	, m_pUIBase{nullptr} {}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_pParent(nullptr)
	, m_iLayerIdx(GAMEOBJECT::INVALID_INDEX)
	, m_bDead(false)
	, m_bActive(true)
	, m_pUIBase{nullptr}
{
	for (auto& pComponent : _origin.m_arrCom)
	{
		if (nullptr != pComponent)
		{
			AddComponent(pComponent->Clone());
		}
	}

	for (auto& pScript : _origin.m_vecScript)
	{
		AddComponent(pScript->Clone());
	}
	for (const auto& pChild : _origin.m_vecChild)
	{
		AddChild(pChild->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::CheckLayerRecursive(const CGameObject* _pInnerChild)
{
	for (const auto& pInner : _pInnerChild->m_vecChild)
	{
		if (pInner->m_iLayerIdx == GAMEOBJECT::INVALID_INDEX)
		{
			pInner->m_iLayerIdx = _pInnerChild->m_iLayerIdx;
		}
		CheckLayerRecursive(pInner);
	}
}

void CGameObject::start()
{
	for (const auto& pComponent : m_arrCom)
	{
		if (nullptr != pComponent)
		{
			pComponent->start();
		}
	}

	for (const auto& pScript : m_vecScript)
	{
		pScript->start();
	}

	for (const auto& pChild : m_vecChild)
	{
		pChild->start();
	}
}

void CGameObject::update()
{
	for (const auto& pComponent : m_arrCom)
	{
		if (nullptr != pComponent && pComponent->IsActive())
		{
			pComponent->update();
		}
	}

	for (const auto& pScript : m_vecScript)
	{
		if (pScript->IsActive())
		{
			pScript->update();
		}
	}

	for (const auto& pChild : m_vecChild)
	{
		if (pChild->IsActive())
		{
			pChild->update();
		}
	}
}

void CGameObject::lateupdate()
{
	for (const auto& pComponent : m_arrCom)
	{
		if (nullptr != pComponent && pComponent->IsActive())
		{
			pComponent->lateupdate();
		}
	}

	for (const auto& pScript : m_vecScript)
	{
		if (pScript->IsActive())
		{
			pScript->lateupdate();
		}
	}

	for (const auto& pChild : m_vecChild)
	{
		if (pChild->IsActive())
		{
			pChild->lateupdate();
		}
	}
}

void CGameObject::finalupdate()
{
	for (const auto& pComponent : m_arrCom)
	{
		if (nullptr != pComponent)
		{
			pComponent->finalupdate();
			if (nullptr != pComponent && nullptr != pComponent->GetDebugObj())
			{
				pComponent->finalupdate_debug();
			}
		}
	}

	// Layer�� ������ �Ѿ�� (ex-DebugObj)
	if (m_iLayerIdx == GAMEOBJECT::INVALID_INDEX)
	{
		return;
	}

	// Layer �� ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer    = pCurScene->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);

	// �ڽ� object final update
	for (const auto& pChild : m_vecChild)
	{
		pChild->finalupdate();
	}
}

void CGameObject::finalupdate_module()
{
	for (const auto& pComponent : m_arrCom)
	{
		if (nullptr != pComponent)
		{
			pComponent->finalupdate_module();
		}
	}

	for (const auto& pChild : m_vecChild)
	{
		pChild->finalupdate_module();
	}
}

void CGameObject::render()
{
	if (m_pRenderComponent->IsActive())
		m_pRenderComponent->render();

	if (nullptr != Collider2D())
		Collider2D()->render();

	if (nullptr != Collider3D())
	{
		Collider3D()->render();
	}
}

CScript* CGameObject::GetScript(UINT _iScriptIndex)
{
	const auto iter = std::find_if(m_vecScript.begin(),
	                               m_vecScript.end(),
	                               [_iScriptIndex](CScript* pScript)
	                               {
		                               return pScript->GetScriptType() == _iScriptIndex;
	                               });
	if (iter != m_vecScript.end())
	{
		return *iter;
	}
	return nullptr;
}

CScript* CGameObject::GetScriptByName(const wstring& _strScriptName) const
{
	const auto iter = std::find_if(m_vecScript.begin(),
	                               m_vecScript.end(),
	                               [&_strScriptName](const CScript* pScript)
	                               {
		                               return pScript->GetName() == _strScriptName;
	                               });
	if (iter != m_vecScript.end())
	{
		return *iter;
	}
	return nullptr;
}

void CGameObject::RenewLayerIndex(int _newLayerIndex)
{
	m_iLayerIdx = _newLayerIndex;
	for (const auto& pChild : m_vecChild)
	{
		pChild->RenewLayerIndex(_newLayerIndex);
	}
}


void CGameObject::active() const
{
	for (size_t i = 1; i < std::size(m_arrCom); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->active();
		}
	}

	for (const auto& pScript : m_vecScript)
	{
		pScript->active();
	}

	for (const auto& pChild : m_vecChild)
	{
		pChild->active();
	}
}

void CGameObject::deactive() const
{
	for (size_t i = 1; i < std::size(m_arrCom); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->deactive();
		}
	}

	for (const auto& pScript : m_vecScript)
	{
		pScript->deactive();
	}

	for (const auto& pChild : m_vecChild)
	{
		pChild->deactive();
	}
}


CGameObject* CGameObject::GetChild(const std::wstring& childName) const
{
	const auto iter = std::find_if(m_vecChild.begin(),
	                               m_vecChild.end(),
	                               [&childName](const CGameObject* pChild)
	                               {
		                               return pChild->GetName() == childName;
	                               });

	if (iter != m_vecChild.end())
	{
		return *iter;
	}
	return nullptr;
}

void CGameObject::SortChild(std::function<bool(CGameObject*, CGameObject*)> func)
{
	std::sort(m_vecChild.begin(), m_vecChild.end(), func);
	CEventMgr::GetInst()->SetOccurObjEvent();
}

void CGameObject::Deregister()
{
	if (GAMEOBJECT::INVALID_INDEX == m_iLayerIdx)
	{
		return;
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(m_iLayerIdx);
	pCurLayer->DeregisterObject(this);
}

void CGameObject::DisconnectBetweenParent()
{
	assert(m_pParent);

	const auto iter = std::find(m_pParent->m_vecChild.begin(), m_pParent->m_vecChild.end(), this);
	if (iter != m_pParent->m_vecChild.end())
	{
		m_pParent->m_vecChild.erase(iter);
		m_pParent = nullptr;
	}
}

void CGameObject::Activate()
{
	tEventInfo info{};
	info.eType  = EVENT_TYPE::ACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;
	CEventMgr::GetInst()->AddEvent(info);
}

void CGameObject::Deactivate()
{
	tEventInfo info{};
	info.eType  = EVENT_TYPE::DEACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;
	CEventMgr::GetInst()->AddEvent(info);
}

bool CGameObject::IsAncestor(const CGameObject* _pObject) const
{
	const CGameObject* pTarget = m_pParent;
	while (pTarget)
	{
		if (pTarget == _pObject)
		{
			return true;
		}
		pTarget = pTarget->m_pParent;
	}
	return false;
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	const int iChildLayerOriginalIdx = _pChild->m_iLayerIdx;

	// �ڽ����� ������ ������Ʈ�� ��Ʈ ������Ʈ�̰�, Ư�� ���̾� �Ҽ��̶��
	if (nullptr == _pChild->GetParent() && GAMEOBJECT::INVALID_INDEX != iChildLayerOriginalIdx)
	{
		// ���̾�� ��Ʈ ������Ʈ�μ� ��� ���� ���⼭ ������ ���̾ -1�� �����Ŵ
		_pChild->Deregister();
		//���� ���� ���̾�� �����ϴ� �ڵ�
		_pChild->m_iLayerIdx = iChildLayerOriginalIdx;
	}

	// �ٸ� �θ������Ʈ�� �̹� �ִٸ�
	if (_pChild->GetParent())
	{
		_pChild->DisconnectBetweenParent();
	}

	//��Ʈ������Ʈ�� �ƴϰ�, ������ �ƹ��� ���̾ �������� �ʴٸ�, �θ��� ���̾ ���󰣴�
	if (GAMEOBJECT::INVALID_INDEX == _pChild->m_iLayerIdx)
	{
		_pChild->m_iLayerIdx = m_iLayerIdx;
	}
	m_vecChild.push_back(_pChild);
	_pChild->m_pParent = this;
	CheckLayerRecursive(_pChild);
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eType = _pComponent->GetType();

	if (COMPONENT_TYPE::SCRIPT != eType)
	{
		assert(nullptr == m_arrCom[(UINT)eType]);

		m_arrCom[static_cast<UINT>(eType)] = _pComponent;
		_pComponent->m_pOwner              = this;

		switch (_pComponent->GetType())
		{
		case COMPONENT_TYPE::UIIMAGE:
		case COMPONENT_TYPE::UIPANEL:
		case COMPONENT_TYPE::UITEXT:
			{
				assert(!m_pUIBase);
				m_pUIBase = static_cast<CUIBase*>(_pComponent);
			}
			break;
		case COMPONENT_TYPE::MESHRENDER:
		case COMPONENT_TYPE::TILEMAP:
		case COMPONENT_TYPE::PARTICLESYSTEM:
		case COMPONENT_TYPE::LANDSCAPE:
		case COMPONENT_TYPE::DECAL:
		case COMPONENT_TYPE::SKYBOX:
			{
				// �ϳ��� ������Ʈ�� Render ����� ���� ������Ʈ�� 2���̻� ���� �� ����.
				assert(!m_pRenderComponent);
				m_pRenderComponent = static_cast<CRenderComponent*>(_pComponent);
			}
			break;
		}
	}
	else
	{
		m_vecScript.push_back(static_cast<CScript*>(_pComponent));
		_pComponent->m_pOwner = this;
	}
}

void CGameObject::DeleteComponent(COMPONENT_TYPE _eType)
{
	SAFE_DELETE(m_arrCom[(UINT)_eType]);

	switch (_eType)
	{
	case COMPONENT_TYPE::MESHRENDER:
	case COMPONENT_TYPE::TILEMAP:
	case COMPONENT_TYPE::PARTICLESYSTEM:
	case COMPONENT_TYPE::LANDSCAPE:
	case COMPONENT_TYPE::DECAL:
	case COMPONENT_TYPE::SKYBOX:
		{
			// m_pRenderComponent �� ������ �����Ѵ�. 
			if (nullptr != m_pRenderComponent)
				m_pRenderComponent = nullptr; // SAFE_DELETE �� �߱⶧���� �����Ⱚ�� ����ִ�. 
		}
		break;
	}
}

void CGameObject::DeleteScript(UINT _ScriptID)
{
	const auto iter = std::find_if(m_vecScript.begin(),
	                               m_vecScript.end(),
	                               [_ScriptID](CScript* pScript)
	                               {
		                               return pScript->GetScriptType() == _ScriptID;
	                               });
	if (iter != m_vecScript.end())
	{
		CScript* pScript = *iter;
		SAFE_DELETE(pScript);
		m_vecScript.erase(iter);
	}
}

void CGameObject::Destroy()
{
	if (m_bDead)
	{
		return;
	}

	tEventInfo info = {};
	info.eType      = EVENT_TYPE::DELETE_OBJ;
	info.lParam     = (DWORD_PTR)this;
	CEventMgr::GetInst()->AddEvent(info);
}


#include "CCamera.h"
#include "CCollider2D.h"
//#include "CCollider3D.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CParticleSystem.h"
#include "CTileMap.h"

void CGameObject::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);
	fwrite(&m_bActive, sizeof(BYTE), 1, _pFile);
	fwrite(&m_bDynamicShadow, sizeof(BYTE), 1, _pFile);
	fwrite(&m_bFrustumCulling, sizeof(BYTE), 1, _pFile);

	// Component ����
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			SaveWStringToFile(ToWString((COMPONENT_TYPE)i), _pFile);
			m_arrCom[i]->SaveToScene(_pFile);
		}
	}
	SaveWStringToFile(L"END", _pFile);
}

void CGameObject::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);
	fread(&m_bActive, sizeof(BYTE), 1, _pFile);
	fread(&m_bDynamicShadow, sizeof(BYTE), 1, _pFile);
	fread(&m_bFrustumCulling, sizeof(BYTE), 1, _pFile);
	// Component �ҷ�����
	wstring strComponentName;

	while (true)
	{
		LoadWStringFromFile(strComponentName, _pFile);
		if (strComponentName == L"END")
			break;

		if (strComponentName == ToWString(COMPONENT_TYPE::TRANSFORM))
		{
			AddComponent(new CTransform);
			Transform()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::CAMERA))
		{
			AddComponent(new CCamera);
			Camera()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER2D))
		{
			AddComponent(new CCollider2D);
			Collider2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER3D)) { }
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR2D))
		{
			AddComponent(new CAnimator2D);
			Animator2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR3D)) { }
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT2D))
		{
			AddComponent(new CLight2D);
			Light2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT3D)) { }


		else if (strComponentName == ToWString(COMPONENT_TYPE::BOUNDINGBOX)) { }
		else if (strComponentName == ToWString(COMPONENT_TYPE::MESHRENDER))
		{
			AddComponent(new CMeshRender);
			MeshRender()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::PARTICLESYSTEM))
		{
			AddComponent(new CParticleSystem);
			ParticleSystem()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::TILEMAP))
		{
			AddComponent(new CTileMap);
			TileMap()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LANDSCAPE)) { }
		else if (strComponentName == ToWString(COMPONENT_TYPE::DECAL)) { }
		else if (strComponentName == ToWString(COMPONENT_TYPE::NAVIMAP))
		{
			AddComponent(new CNaviMap);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::NAVIAGENT))
		{
			//AddComponent(new CNaviAgent);
		}
	}
}

void CGameObject::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << "NAME" << YAML::Value << ToString(GetName());
	emitter << YAML::Key << NAME_OF(m_bActive) << YAML::Value << m_bActive;
	emitter << YAML::Key << NAME_OF(m_bDynamicShadow) << YAML::Value << m_bDynamicShadow;
	emitter << YAML::Key << NAME_OF(m_bFrustumCulling) << YAML::Value << m_bFrustumCulling;

	// Component ����
	emitter << YAML::Key << "COMPONENTS";
	emitter << YAML::Value << YAML::BeginSeq;
	for (size_t i = 0; i < std::size(m_arrCom); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			emitter << YAML::BeginMap;
			emitter << YAML::Key << ToString(static_cast<COMPONENT_TYPE>(i));
			emitter << YAML::Value << YAML::BeginMap;
			m_arrCom[i]->Serialize(emitter);
			emitter << YAML::EndMap;
			emitter << YAML::EndMap;
		}
	}
	emitter << YAML::EndSeq;
}

void CGameObject::Deserialize(const YAML::Node& node)
{
	const std::string name = node["NAME"].as<std::string>();
	SetName(ToWString(name));
	m_bActive         = node[NAME_OF(m_bActive)].as<bool>();
	m_bDynamicShadow  = node[NAME_OF(m_bDynamicShadow)].as<bool>();
	m_bFrustumCulling = node[NAME_OF(m_bFrustumCulling)].as<bool>();

	const YAML::Node& components = node["COMPONENTS"];
	for (size_t i = 0; i < components.size(); ++i)
	{
		const std::string componentName = (components[i].begin())->first.as<std::string>();
		CComponent*       pComponent    = CComponent::MakeComponent(ToWString(componentName));
		AddComponent(pComponent);
		pComponent->Deserialize(components[i].begin()->second);
	}
}
