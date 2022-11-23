#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"


CLayer::CLayer()
	:
	m_iLayerIdx(-1) {}

CLayer::~CLayer()
{
	Safe_Del_Vec(m_vecRoot);
}



void CLayer::start()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->start();
	}
}

void CLayer::update()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		if (m_vecRoot[i]->IsActive())
			m_vecRoot[i]->update();
	}
}

void CLayer::lateupdate()
{
	for (size_t i = 0; i < m_vecRoot.size(); ++i)
	{
		if (m_vecRoot[i]->IsActive())
			m_vecRoot[i]->lateupdate();
	}
}

void CLayer::finalupdate()
{
	vector<CGameObject*>::iterator iter = m_vecRoot.begin();

	for (; iter != m_vecRoot.end();)
	{
		(*iter)->finalupdate();

		if ((*iter)->IsDead())
		{
			iter = m_vecRoot.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::DeregisterObject(CGameObject* _pObj)
{
	//if (_pObj->GetParent())
		//return;

	if (_pObj->GetParent()) 
	{
		_pObj->DisconnectBetweenParent();
		ClearObjectLayerIndex(_pObj);

	}

	vector<CGameObject*>::iterator iter = m_vecRoot.begin();
	for (; iter != m_vecRoot.end(); ++iter)
	{
		if ((*iter) == _pObj)
		{
			m_vecRoot.erase(iter);
			ClearObjectLayerIndex(_pObj); // Layer Index = -1;
			return;
		}
	}
}

CGameObject* CLayer::FindRootObject(wstring _wstrNames) const
{
	for (int i = 0; i < m_vecRoot.size(); ++i)
	{
		if (_wstrNames == m_vecRoot[i]->GetName())
		{
			return m_vecRoot[i];
		}
	}

	return nullptr;
}

void CLayer::RenewLayerIdx(int _LayerIdx)
{

	assert(!(_LayerIdx < 0 || MAX_LAYER <= _LayerIdx));

	m_iLayerIdx = _LayerIdx;
	for (int i = 0; i < m_vecRoot.size(); ++i)
	{
		m_vecRoot[i]->m_iLayerIdx = _LayerIdx;
		// 자식 오브젝트 
		vector<CGameObject*> vecChildObj = m_vecRoot[i]->GetChild();
		for (int k = 0; k < vecChildObj.size(); ++k)
		{
			vecChildObj[k]->RenewLayerIndex(_LayerIdx);
		}
	}

}

void CLayer::ClearObjectLayerIndex(CGameObject* _pObj)
{
	_pObj->m_iLayerIdx = -1;

	vector<CGameObject*> vecChild = _pObj->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		ClearObjectLayerIndex(vecChild[i]);

	}
}
