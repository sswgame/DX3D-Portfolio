#pragma once
#include "CComponent.h"

class CNaviAgent : public CComponent
{
private:
	CNaviMap* m_pTargetNaviMap;
	int       m_iCurCellIndex;
	bool      m_bGravity;


	Vec3 m_vPrevPos;

private:
	void FindCurIndex();
	bool CheckLinkedCell();
	void SetYPos();

public:
	void      SetNaviMap(CNaviMap* _pNaviMap);
	CNaviMap* GetNaviMap() { return m_pTargetNaviMap; }
	UINT      GetCurCellIndex() { return m_iCurCellIndex; }


	void update() override;
	void lateupdate() override;
	void finalupdate() override;
	void finalupdate_debug() override;

	void UpdateData() override;
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

	CLONE(CNaviAgent);
public:
	CNaviAgent();
	CNaviAgent(const CNaviAgent& _origin);
	virtual ~CNaviAgent();
};
