#pragma once
#include "CComponent.h"
#include "CFrustum.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};


class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*>    m_vecDeferred;          // Deferred ��ü
    vector<CGameObject*>    m_vecDeferredDecal;     // Decal
    vector<CGameObject*>    m_vecForward;           // ������ ��ü    
    vector<CGameObject*>    m_vecMasked;            // ����, ������ ��ü
    vector<CGameObject*>    m_vecForwardDecal;      // Decal
    vector<CGameObject*>    m_vecTranslucent;       // ������ ��ü
    vector<CGameObject*>    m_vecPostProcess;       // �� ó�� ��ü

    vector<CGameObject*>    m_vecDynamicShadow;     // ���� �׸��� ��ü

protected:
    CFrustum                m_Frustum;

protected:
    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    PROJ_TYPE               m_eProjType;

    float                   m_fWidth;       // ���� ���α���
    float                   m_fAspectRatio; // ��Ⱦ�� : (���� / ����)
    float                   m_fFOV;         // Filed Of View (�þ߰�)
    float                   m_fFar;         // �ִ� �þ� �Ÿ�

    UINT                    m_iLayerMask;
    int                     m_iCamIdx;  // RenderMgr �󿡼�   




public:
    void SetCameraAsMain();
    void SetCameraIndex(int _iIdx);
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetFOV(float _FOV) { m_fFOV = _FOV; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    void SetFar(float _Far) { m_fFar = _Far; }
    void SetShowFrustum(bool _bShow) { m_Frustum.SetShowFrustum(_bShow); }
    
    
    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    float GetFOV() { return m_fFOV; }
    float GetFar() { return m_fFar; }
    bool GetShowFrustum() { return m_Frustum.GetShowFrustum(); }

    PROJ_TYPE GetProjType() { return m_eProjType; }

    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll(){ m_iLayerMask = 0xffffffff; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

private:
    void SortGameObject();  // Shader Domain �� ���� ��ü �з�

public:
    void SortShadowObject();

public:    
    virtual void finalupdate() override;
    virtual void finalupdate_module() override;
    void render_deferred();
    void render_deferred_decal();
    void render_forward();
    void render_forward_decal();
    void render_masked();
    void render_translucent();
    void render_postprocess();

    void render_shadowmap();
    void render_frustum();
public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CCamera)

public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;
};

