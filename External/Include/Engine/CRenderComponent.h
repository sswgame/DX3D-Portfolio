#pragma once
#include "CComponent.h"

#include "CMaterial.h"
#include "CMesh.h"

struct tMtrlSet
{
	Ptr<CMaterial>  pMtrl;          // 현재 사용 할 메테리얼
	Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
	Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
};


class CRenderComponent : public CComponent
{
private:
	Ptr<CMesh>			m_pMesh;        // 메쉬
	vector<tMtrlSet>    m_vecMtrls;     // 재질    

	bool m_bDynamicShadow;  // 동적 그림자 생성
	bool m_bFrustumCulling; // 절두체 컬링 

public:
	void SetMesh(Ptr<CMesh> _pMesh);
	void SetSharedMaterial(Ptr<CMaterial> _pMtrl, UINT _iIdx);

	void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
	bool IsDynamicShadow() { return m_bDynamicShadow; }

	void SetFrustumCulling(bool _bSet) { m_bFrustumCulling = _bSet; }
	bool IsFrustumCulling() { return m_bFrustumCulling; }

	Ptr<CMesh>     GetMesh() { return m_pMesh; }
	Ptr<CMaterial> GetMaterial(UINT _iIdx);
	Ptr<CMaterial> GetSharedMaterial(UINT _iIdx);
	Ptr<CMaterial> GetDynamicMaterial(UINT _iIdx);

	UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

public:
	virtual void render() = 0;
	virtual void render_shadowmap();
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CRenderComponent(COMPONENT_TYPE _type);
	CRenderComponent(const CRenderComponent& _origin);
	virtual ~CRenderComponent();
};
