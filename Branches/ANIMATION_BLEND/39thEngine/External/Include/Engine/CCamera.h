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
    vector<CGameObject*>    m_vecDeferred;          // Deferred 물체
    vector<CGameObject*>    m_vecDeferredDecal;     // Decal
    vector<CGameObject*>    m_vecForward;           // 불투명 물체    
    vector<CGameObject*>    m_vecMasked;            // 투명, 불투명 물체
    vector<CGameObject*>    m_vecForwardDecal;      // Decal
    vector<CGameObject*>    m_vecTranslucent;       // 반투명 물체
    vector<CGameObject*>    m_vecPostProcess;       // 후 처리 물체

    vector<CGameObject*>    m_vecDynamicShadow;     // 동적 그림자 물체

protected:
    CFrustum                m_Frustum;

protected:
    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    tRay                    m_ray;      // 마우스 방향을 향하는 직선

    PROJ_TYPE               m_eProjType;

    float                   m_fWidth;       // 투영 가로길이
    float                   m_fAspectRatio; // 종횡비 : (가로 / 세로)
    float                   m_fFOV;         // Filed Of View (시야각)
    float                   m_fFar;         // 최대 시야 거리

    UINT                    m_iLayerMask;
    int                     m_iCamIdx;  // RenderMgr 상에서   


public:
    void SetCameraAsMain();
    void SetCameraIndex(int _iIdx);
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetFOV(float _FOV) { m_fFOV = _FOV; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    void SetFar(float _Far) { m_fFar = _Far; }

    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    float GetFOV() { return m_fFOV; }
    float GetFar() { return m_fFar; }
    PROJ_TYPE GetProjType() { return m_eProjType; }
    const tRay& GetRay() { return m_ray; }

    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll(){ m_iLayerMask = 0xffffffff; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

private:
    void SortGameObject();  // Shader Domain 에 따른 물체 분류
    void CalRay();  // 마우스 방향으로 광선 연산

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

