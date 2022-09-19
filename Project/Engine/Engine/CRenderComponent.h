#pragma once
#include "CComponent.h"

#include "CMaterial.h"
#include "CMesh.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;        // 메쉬
    Ptr<CMaterial>  m_pMtrl;        // 현재 사용중인 재질
    Ptr<CMaterial>  m_pSharedMtrl;  // 공유 재질
    Ptr<CMaterial>  m_pDynamicMtrl; // 별도로 가지는 재질

    bool            m_bDynamicShadow; // 동적 그림자 생성
    bool            m_bFrustumCulling; // 절두체 컬링 

public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetSharedMaterial(Ptr<CMaterial> _pMtrl);

    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }

    void SetFrustumCulling(bool _bSet) { m_bFrustumCulling = _bSet; }
    bool IsFrustumCulling() { return m_bFrustumCulling; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();




public:
    virtual void render() = 0;
    virtual void render_shadowmap();
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};

