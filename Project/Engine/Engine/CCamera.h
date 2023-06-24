#pragma once
#include "CComponent.h"
#include "CFrustum.h"

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};


class CGameObject;

class CCamera : public CComponent
{
private:
	vector<CGameObject*> m_vecDeferred;      // Deferred ��ü
	vector<CGameObject*> m_vecDeferredDecal; // Decal
	vector<CGameObject*> m_vecDeferredParticle; // Decal

	vector<CGameObject*> m_vecForward;       // ������ ��ü    
	vector<CGameObject*> m_vecMasked;        // ����, ������ ��ü
	vector<CGameObject*> m_vecForwardDecal;  // Decal
	vector<CGameObject*> m_vecTranslucent;   // ������ ��ü
	vector<CGameObject*> m_vecPostProcess;   // �� ó�� ��ü

	vector<CGameObject*> m_vecDynamicShadow; // ���� �׸��� ��ü
	vector<CComponent*>  m_vecComponentDebug;		 // ����� ������Ʈ

protected:
	CFrustum m_Frustum;

	Matrix m_matView;
	Matrix m_matViewInv;
	Matrix m_matProj;
	Matrix m_matProjInv;

	tRay m_ray;      // ���콺 ������ ���ϴ� ����

	PROJ_TYPE m_eProjType;

	float m_fWidth;       // ���� ���α���
	float m_fAspectRatio; // ��Ⱦ�� : (���� / ����)
	float m_fFOV;         // Filed Of View (�þ߰�)
	float m_fFar;         // �ִ� �þ� �Ÿ�

	std::bitset<MAX_LAYER> m_iLayerMask;
	int                    m_iCamIdx; // RenderMgr �󿡼�   


public:
	void SetCameraAsMain();
	void SetCameraIndex(int _iIdx);
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
	void SetFOV(float _FOV) { m_fFOV = _FOV; }
	void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
	void SetFar(float _Far) { m_fFar = _Far; }
	void SetShowFrustum(bool _bShow) { m_Frustum.SetShowFrustum(_bShow); }

	float       GetWidth() const { return m_fWidth; }
	float       GetAspectRatio() const { return m_fAspectRatio; }
	float       GetFOV() const { return m_fFOV; }
	float       GetFar() const { return m_fFar; }
	bool        GetShowFrustum() { return m_Frustum.GetShowFrustum(); }
	const tRay& GetRay() const { return m_ray; }
	PROJ_TYPE   GetProjType() const { return m_eProjType; }
	CFrustum    GetFrustum() const { return m_Frustum; }

	void CheckLayerMask(int _iLayerIdx, bool enable = true);
	void CheckLayerMask(const wstring& _strLayerName, bool enable = true);
	void CheckLayerMaskAll(bool enable = true) { m_iLayerMask = (enable) ? 0xFFFF'FFFF : 0; }

	const Matrix& GetViewMat() const { return m_matView; }
	const Matrix& GetViewInvMat() const { return m_matViewInv; }
	const Matrix& GetProjMat() const { return m_matProj; }
	const Matrix& GetProjInvMat() const { return m_matProjInv; }

private:
	void SortGameObject(); // Shader Domain �� ���� ��ü �з�
	void SortDebugGameObject(CGameObject* _pObj);
	void CalRay();  // ���콺 �������� ���� ����
	bool FrustumCulling(CGameObject* pObj);

public:
	void SortShadowObject();

public:
	void finalupdate() override;
	void finalupdate_module() override;
	void render_deferred();
	void render_deferred_decal();
	void render_particle();
	void render_forward();
	void render_forward_decal();
	void render_masked();
	void render_translucent();
	void render_postprocess();

	void render_shadowmap();
	void render_debug() override;
	void render_Frustum() { m_Frustum.render(); }

public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CLONE(CCamera);
	CCamera();
	CCamera(const CCamera& _origin);
	virtual ~CCamera();

	friend class CRenderMgr;
};
