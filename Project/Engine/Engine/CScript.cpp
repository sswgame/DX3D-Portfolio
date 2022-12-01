#include "pch.h"
#include "CScript.h"


CScript::CScript(int _iScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptID(_iScriptType) {}

CScript::~CScript() = default;

auto CScript::FindScalarParam(const std::string& paramName)
{
	const auto iter = std::find_if(m_vecParamInfo.begin(),
	                               m_vecParamInfo.end(),
	                               [&paramName](const tScriptParamInfo& info)
	                               {
		                               return info.strParamName == paramName;
	                               });
	return iter;
}

void CScript::RemoveScalarParam(const std::string& paramName)
{
	const auto iter = FindScalarParam(paramName);
	if (iter != m_vecParamInfo.end())
	{
		m_vecParamInfo.erase(iter);
	}
}

void CScript::RenewScalarParam(const std::string& paramName, void* pValue, std::function<void()> callback)
{
	const auto iter = FindScalarParam(paramName);
	if (iter != m_vecParamInfo.end())
	{
		tScriptParamInfo& paramInfo = *iter;
		paramInfo.pParam            = pValue;
		if (callback)
		{
			paramInfo.callback = callback;
		}
	}
}
