#pragma once
#include <Engine\CScript.h>

enum class ENERGYBALL_MODE
{
	NONE,		// ���ڸ��� �״�� ����
	ROTATION,	// �߽����� �ΰ� ȸ����
	MISSILE		// Ÿ���� ���� ���ư� ������ 
};

class EnergyBallScript : public CScript
{
private:
	ENERGYBALL_MODE m_eCurMode;
	Vec3            m_vTargetPos;
	Vec3            m_vDir;
	float           m_fRadius;
	float           m_fSpeed;
	float           m_fRotTime;
	bool            m_bFinish;

public:
	void            SetCurMode(ENERGYBALL_MODE _eMode) { m_eCurMode = _eMode; }
	void            SetTargetPos(Vec3 _vPos);
	void            SetRadius(float _fRadius) { m_fRadius = _fRadius; }
	void            SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	ENERGYBALL_MODE GetCurMode() { return m_eCurMode; }
	Vec3            GetTargetPos() { return m_vTargetPos; }
	float           GetRadius() { return m_fRadius; }
	float           GetSpeed() { return m_fSpeed; }

	bool IsFinish() { return m_bFinish; }

	void Start(ENERGYBALL_MODE _eMode, Vec3 _vTargetPos = Vec3(0.f, 0.f, 0.f), float _fRotTime = 0.f)
	{
		m_eCurMode   = _eMode;
		m_vTargetPos = _vTargetPos;
		m_fRotTime   = _fRotTime;
		m_bFinish    = false;
	}

	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(EnergyBallScript)

	EnergyBallScript();
	EnergyBallScript(const EnergyBallScript& _origin);
	~EnergyBallScript();
};