#pragma once
#include "CComponent.h"

#include "CMaterial.h"
#include "CMesh.h"

struct tMtrlSet
{
	Ptr<CMaterial> pMtrl;          // ���� ��� �� ���׸���
	Ptr<CMaterial> pSharedMtrl;    // ���� ���׸���
	Ptr<CMaterial> pDynamicMtrl;   // ���� ���׸����� ���纻    
};

class CRenderComponent : public CComponent
{
private:
	Ptr<CMesh>       m_pMesh;        // �޽�
	vector<tMtrlSet> m_vecMtrls;     // ����    

	bool m_bDynamicShadow;  // ���� �׸��� ����
	bool m_bFrustumCulling; // ����ü �ø� 

	std::vector<bool> m_vecUseDynamicMaterial;

public:
	void SetMesh(Ptr<CMesh> _pMesh);
	void SetSharedMaterial(Ptr<CMaterial> _pMtrl, UINT _iIdx);

	void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
	bool IsDynamicShadow() const { return m_bDynamicShadow; }

	void SetFrustumCulling(bool _bSet) { m_bFrustumCulling = _bSet; }
	bool IsFrustumCulling() const { return m_bFrustumCulling; }

	Ptr<CMesh>     GetMesh() { return m_pMesh; }
	Ptr<CMaterial> GetMaterial(UINT _iIdx);
	Ptr<CMaterial> GetSharedMaterial(UINT _iIdx);
	Ptr<CMaterial> GetDynamicMaterial(UINT _iIdx);

	UINT GetMtrlCount() const { return static_cast<UINT>(m_vecMtrls.size()); }

	void SetUseDynamicMaterial(UINT _iIndex, bool _enable);
	bool IsUsingDynamicMaterial(UINT _iIndex);

public:
	virtual void render() = 0;
	virtual void render_shadowmap();
	void         SaveToScene(FILE* _pFile) override;
	void         LoadFromScene(FILE* _pFile) override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CRenderComponent(COMPONENT_TYPE _type);
	CRenderComponent(const CRenderComponent& _origin);
	virtual ~CRenderComponent();
};
