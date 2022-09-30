#pragma once
#include "CRenderComponent.h"

enum class DECAL_TYPE
{
	SPHERE,
	CUBE,
};

class CDecal : public CRenderComponent
{
private:
	DECAL_TYPE    m_eType;
	bool          m_bUseDeferredLighting;
	Ptr<CTexture> m_pDecalTex;

public:
	void SetDecalType(DECAL_TYPE _eType);
	void SetDeferredLighting(bool _bLighting);
	void SetDecalTexture(Ptr<CTexture> _pTex) { m_pDecalTex = _pTex; }

public:
	virtual void finalupdate() override;
	virtual void UpdateData() override;
	virtual void render() override;


public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CDecal)
public:
	CDecal();
	virtual ~CDecal();
};
