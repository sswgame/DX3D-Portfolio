#pragma once
#include <Engine/CScript.h>

class PaperBurnScript :
	public CScript
{
private:
	bool		m_bApply;
	bool		m_bFinish;

	float		m_fStrength;
	Vec4		m_vColor;

	int			m_iDir;


public:

	void On();
	void Off();
	void Stop();
	void ReturnOriginalState();

	void SetDir(int _dir) { m_iDir = _dir; }


	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(PaperBurnScript)

public:
	PaperBurnScript(const PaperBurnScript& _origin);
	PaperBurnScript();
	virtual ~PaperBurnScript();
};

