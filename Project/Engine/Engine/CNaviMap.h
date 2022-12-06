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
	 * \brief NaviMap�� �ﰢ��(Cell)�� �浹 �׽�Ʈ�� �����ϴ� �Լ�
	 * \param _iCellidx NaviMap �ﰢ��(Cell)�� �ε���
	 * \param _vPos ����� World Position
	 * \param _vDir ��� ���� �浹 ����
	 * \param _fDist �浹 ���� �Ÿ�
	 * \return �浹 ���θ� ��ȯ
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
