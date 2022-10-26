#include "pch.h"
#include "CFSM.h"
#include "CTimeMgr.h"
#include "CEventMgr.h"

void CState::CallUpdateFunc()
{
	if (nullptr == m_updateFunc)
	{
		return;
	}
	m_tState.fStateTime += DT;
	m_updateFunc(m_tState);
}

CFSM::CFSM()
	:
	CComponent{COMPONENT_TYPE::FINITE_STATE_MACHINE}
  , m_pCurrentState{nullptr} {}

CFSM::~CFSM() {}

void CFSM::CreateState(const std::wstring&                            _strStateName,
                       std::function<void(const tStateInfo& _tState)> _updateFunc,
                       std::function<void(const tStateInfo& _tState)> _startFunc/*=nullptr*/,
                       std::function<void(const tStateInfo& _tState)> _endFunc/*=nullptr*/)
{
	const auto iter = m_mapState.find(_strStateName);
	assert(iter == m_mapState.end() && "ALREADY HAS A STATE WITH THE NAME");

	CState& newState          = m_mapState[_strStateName];
	newState.m_tState.strName = _strStateName;
	newState.m_updateFunc     = _updateFunc;
	newState.m_startFunc      = _startFunc;
	newState.m_endFunc        = _endFunc;
}

void CFSM::finalupdate()
{
	if (nullptr == m_pCurrentState)
	{
		return;
	}
	m_pCurrentState->CallUpdateFunc();
}

void CFSM::ChangeState(const std::wstring& _strNextStateName)
{
	static std::wstring s_temp{};
	s_temp = _strNextStateName;
	tEventInfo info{};
	info.eType  = EVENT_TYPE::CHANGE_AI_STATE;
	info.lParam = (DWORD_PTR)this;
	info.wParam = (DWORD_PTR)s_temp.c_str();

	CEventMgr::GetInst()->AddEvent(info);
}


const std::wstring& CFSM::GetCurrentStateName() const
{
	assert(m_pCurrentState);
	return m_pCurrentState->m_tState.strName;
}

float CFSM::GetCurrentStateTime() const
{
	assert(m_pCurrentState);
	return m_pCurrentState->m_tState.fStateTime;
}
