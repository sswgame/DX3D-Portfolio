#pragma once
#include "CComponent.h"

struct tStateInfo
{
	float        fStateTime;
	std::wstring strName;
	std::wstring strPrevStateName;
};

class CState
{
private:
	tStateInfo                             m_tState;
	std::function<void(const tStateInfo&)> m_updateFunc;
	std::function<void(const tStateInfo&)> m_startFunc;
	std::function<void(const tStateInfo&)> m_endFunc;

public:
	void CallUpdateFunc();

public:
	friend class CFSM;
	friend class CEventMgr;
};

class CFSM final : public CComponent
{
private:
	std::map<std::wstring, CState> m_mapState;

	CState* m_pCurrentState;

public:
	const std::wstring& GetCurrentStateName() const;
	float               GetCurrentStateTime() const;

public:
	void finalupdate() override;

	void ChangeState(const std::wstring& _strNextStateName);

	void CreateState(const std::wstring&                            _strStateName,
	                 std::function<void(const tStateInfo& _tState)> _updateFunc,
	                 std::function<void(const tStateInfo& _tState)> _startFunc = nullptr,
	                 std::function<void(const tStateInfo& _tState)> _endFunc   = nullptr);

public:
	friend class CEventMgr;
	CLONE_DISABLE(CFSM);
	CFSM();
	~CFSM();
};

#define BIND_FN(func) std::bind(func,this,std::placeholders::_1)
