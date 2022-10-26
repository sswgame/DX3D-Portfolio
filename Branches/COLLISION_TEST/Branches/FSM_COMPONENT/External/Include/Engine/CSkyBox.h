#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
	SPHERE,
	CUBE,
};


class CSkyBox : public CRenderComponent
{
private:
	SKYBOX_TYPE m_eType;

public:
	void SetSkyboxType(SKYBOX_TYPE _eType);


public:
	virtual void finalupdate() override;
	virtual void UpdateData() override;
	virtual void render() override;


public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CSkyBox)
public:
	CSkyBox();
	virtual ~CSkyBox();
};
