#include "pch.h"
#include "CFSM.h"
#include "CTimeMgr.h"
#include "CEventMgr.h"

CFSM::CFSM()
	: CComponent{COMPONENT_TYPE::FINITE_STATE_MACHINE}
	, m_pCurState(nullptr)
{
}

CFSM::~CFSM()
{
	Safe_Del_Map(m_mapState);
}


void CFSM::update()
{
	if (m_pCurState != nullptr)
		m_pCurState->Update();
}

void CFSM::lateupdate()
{
	if (m_pCurState != nullptr)
		m_pCurState->LateUpdate();
}

void CFSM::finalupdate()
{
}

CState* CFSM::GetState(wstring _stateTypeName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_stateTypeName);
	if (iter != m_mapState.end())
		return nullptr;

	return iter->second;
}

void CFSM::AddState(wstring _sStateType, CState* _pState)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_sStateType);
	if (iter != m_mapState.end())
		return;
	_pState->SetName(_sStateType);
	_pState->SetOwner(GetOwner());
	m_mapState.insert(make_pair(_sStateType, _pState));
}

void CFSM::ChangeState(wstring _sStateType)
{
	static wstring sStateTypeName;
	sStateTypeName = _sStateType;
	tEventInfo info{};

	info.eType  = EVENT_TYPE::CHANGE_FSM_STATE;
	info.lParam = (DWORD_PTR)this;
	info.wParam = (DWORD_PTR)sStateTypeName.c_str();

	CEventMgr::GetInst()->AddEvent(info);
}


void CFSM::DeleteState(wstring _sStateType)
{
	map<wstring, CState*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); ++iter)
	{
		if (iter->first == _sStateType)
		{
			SAFE_DELETE(iter->second);
			iter->second = nullptr;
			break;
		}
	}
	m_mapState.erase(_sStateType);
}
