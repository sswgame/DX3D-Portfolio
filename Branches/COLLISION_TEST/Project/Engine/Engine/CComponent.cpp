#include "pch.h"
#include "CComponent.h"

#include "CEventMgr.h"

#include "CTransform.h"
#include "CMeshRender.h"

CComponent::CComponent(COMPONENT_TYPE _eType)
	: m_eComType(_eType)
	, m_pOwner{nullptr}
	, m_bActive(true)
	, m_pDebugObj(nullptr) {}

CComponent::~CComponent() 
{
	if (nullptr != m_pDebugObj)
		delete(m_pDebugObj);
}

void CComponent::Activate()
{
	tEventInfo info = {};

	info.eType  = EVENT_TYPE::ACTIVATE_COMPONENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CComponent::Deactivate()
{
	tEventInfo info = {};

	info.eType  = EVENT_TYPE::DEACTIVATE_COMOPNENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}
