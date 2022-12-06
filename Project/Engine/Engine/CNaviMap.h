#pragma once
#include "CComponent.h"

class CNaviMapData;

class CNaviMap : public CComponent
{
private:
	CNaviMapData* m_pNeviMapData;

	Ptr<CMesh>     m_pMesh;
	Ptr<CMaterial> m_pMaterial;

public:
	void          SetNaviMapData(CNaviMapData* _pData);
	CNaviMapData* GetNaviMapData() { return m_pNeviMapData; }

	/**
	 * \brief NaviMap의 삼각형(Cell)과 충돌 테스트를 진행하는 함수
	 * \param _iCellidx NaviMap 삼각형(Cell)의 인덱스
	 * \param _vPos 대상의 World Position
	 * \param _vDir 대상 기준 충돌 방향
	 * \param _fDist 충돌 감지 거리
	 * \return 충돌 여부를 반환
	 */
	bool CheckIntersect(int _iCellidx, Vec3 _vPos, Vec3 _vDir, float& _fDist);

	void UpdateData() override;
	void finalupdate() override;
	void finalupdate_debug() override;
	void render_debug() override;

	CLONE(CNaviMap);
	
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CNaviMap();
	CNaviMap(const CNaviMap& _origin);
	virtual ~CNaviMap();
};
