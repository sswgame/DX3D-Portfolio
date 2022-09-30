#pragma once
#include "CComponent.h"

#include "CMaterial.h"
#include "CMesh.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;        // �޽�
    Ptr<CMaterial>  m_pMtrl;        // ���� ������� ����
    Ptr<CMaterial>  m_pSharedMtrl;  // ���� ����
    Ptr<CMaterial>  m_pDynamicMtrl; // ������ ������ ����

    bool            m_bDynamicShadow; // ���� �׸��� ����
    bool            m_bFrustumCulling; // ����ü �ø� 

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

