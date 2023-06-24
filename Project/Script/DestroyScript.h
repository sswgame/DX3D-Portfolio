#pragma once
#include <Engine/CScript.h>

/*
		�� ��ũ��Ʈ�� �߰��ϸ�
		������ �ð��� �����ų�
		Ư�� ����� �ο��ϸ� ������Ʈ�� ������ϴ�.

*/

enum class DESTROY_MODE
{
	DEAFULT,	// Ư�� Ÿ�̹� ��� ��� 
	TIMER,		// ������ �ð� 
	ACTIVE,		// �ƿ� ���� ���� �ʰ� DEACTIVATE

};

class DestroyScript :
	public CScript
{
private:
	float				m_fDeadLine_Timer;		// ���� �ð� 
	float				m_fTimer;				// ���� ���� �ð� 
	bool				m_bDestory;				// ���� ���� 

	DESTROY_MODE		m_eDestroyMode;			// ������� ��� 
	float				m_fTimerRatio;


public:
	void DestroyObject();
	void SetDeadLineTime(float _fTimer) { m_fDeadLine_Timer = _fTimer; }
	void Destroy(bool _b) { m_bDestory = _b; }

	void SetDestroyMode(DESTROY_MODE _eMode) { m_eDestroyMode = _eMode; }

	void ResetTimer() { m_fTimer = 0.f; }

public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(DestroyScript)

public:
	DestroyScript(const DestroyScript& _origin);
	DestroyScript();
	virtual ~DestroyScript();
};

