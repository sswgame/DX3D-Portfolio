#pragma once

#include "global.h"
#include "CComponent.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"


enum class SCRIPTPARAM_TYPE
{
	INT,
	FLOAT,
	VEC2,
	VEC4,
	TEXT,
	TEXT_READONLY,
	TEXTURE,
	PREFAB,
};

enum class SHOW_TYPE
{
	NORMAL,
	DROP_DOWN,
	CHECK_BOX,
	COLOR_PICKER,
	END
};

struct tScriptParamInfo
{
	string           strParamName;
	SCRIPTPARAM_TYPE eType;
	void*            pParam;

	SHOW_TYPE                showType;
	std::vector<std::string> vecKind;
	std::function<void()>    callback;
};


class CScript : public CComponent
{
private:
	const int                m_iScriptID;    // 스크립트 구별용도(스크립트 타입)
	vector<tScriptParamInfo> m_vecParamInfo; // 에디터 노출 변수

	bool m_bChanged = false;
public:
	int                             GetScriptType() const { return m_iScriptID; }
	const vector<tScriptParamInfo>& GetScriptParam() { return m_vecParamInfo; }

protected:
	void AddScriptParam(string                _strParamName,
	                    SCRIPTPARAM_TYPE      _eType,
	                    void*                 _pData,
	                    std::function<void()> callback = nullptr)
	{
		m_vecParamInfo.push_back(tScriptParamInfo{_strParamName, _eType, _pData, SHOW_TYPE::NORMAL, {}, callback});
	}

	void AddScriptParamAsDropDown(const std::string&       _strParameterName,
	                              SCRIPTPARAM_TYPE         _eType,
	                              void*                    _pData,
	                              std::vector<std::string> vecListMenu,
	                              std::function<void()>    callback = nullptr)
	{
		m_vecParamInfo.push_back(tScriptParamInfo{
			                         _strParameterName,
			                         _eType,
			                         _pData,
			                         SHOW_TYPE::DROP_DOWN,
			                         vecListMenu,
			                         callback
		                         });
	}

	void AddScriptParamAsCheckBox(const std::string&    _strParameterName,
	                              int*                  _pData,
	                              std::function<void()> callback = nullptr)
	{
		m_vecParamInfo.push_back(tScriptParamInfo{
			                         _strParameterName,
			                         SCRIPTPARAM_TYPE::INT,
			                         _pData,
			                         SHOW_TYPE::CHECK_BOX,
			                         {},
			                         callback
		                         });
	}

	void AddScriptParamAsColorPicker(const std::string&    _strParameterName,
	                                 Vec4*                 _pData,
	                                 std::function<void()> callback = nullptr)
	{
		m_vecParamInfo.push_back(tScriptParamInfo{
			                         _strParameterName,
			                         SCRIPTPARAM_TYPE::VEC4,
			                         _pData,
			                         SHOW_TYPE::COLOR_PICKER,
			                         {},
			                         callback
		                         });
	}

	void RemoveScalarParam(const std::string& paramName);
	void RemoveAllScalarParam() { m_vecParamInfo.clear(); }
	void RenewScalarParam(const std::string& paramName, void* pValue, std::function<void()> callback = nullptr);
private:
	auto FindScalarParam(const std::string& paramName);
public:
	virtual void start() {}
	virtual void update() {}
	virtual void lateupdate() {}
	void         finalupdate() final {}

	virtual void OnCollisionEnter(CGameObject* _OtherObject) {};
	virtual void OnCollision(CGameObject* _OtherObject) {};
	virtual void OnCollisionExit(CGameObject* _OtherObject) {};

	CScript* Clone() override = 0;

public:
	CScript(int _iScriptType);
	virtual ~CScript();
};
