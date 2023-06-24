#pragma once
#include <Engine/CEntity.h>

/*
* [ ���׸� ���� ]
		���׸� ����  �� Shattered: Tale of the Forgotten King�� �Ҹ�ǰ�Դϴ�.
		�Ҹ� ��������
			* HP ȸ���� ���� ȸ�� �Ӽ�
			* Ư�� �������� ���� ȿ�� ����
			* �Ͻ��� �Ǵ� �������� ���� �� ����� ����
		���� �پ��� ȿ���� �ο��մϴ�.

*/

enum class STAT_TYPE
{
	HP,
	STAMINA,
	ETHER,
	ATTACK,
	ARMOR,
	SPEED,
	END,
};

struct tSTAT
{
	float	fHp;		// ü��
	float	fStamina;	// Ȱ�� 
	float	fEther;		// ���׸� 

	float	fAttack;	// ���ݷ�
	float	fArmor;		// ����

	float   fSpeed;		// �ӵ�
};

class CPlayerStat :
    public CEntity
{
private:
	CScript* m_pOwnerScript;

	tSTAT	 m_tMaxStat;		// �ִ� ����
	tSTAT	 m_tCurStat;		// ���� ����
		
public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();


	float GetDamage();


public:
	// [ S U B ]
	void SubHp(float _fHp);
	void SubStamina(float _fStamina);
	void SubEther(float _fEther);

	void SubAttack(float _fAttack);
	void SubArmor(float _fStamina);

	void SubSpeed(float _fSpeed);


	// [ A D D ]
	void AddHp(float _fHp);
	void AddStamina(float _fStamina);
	void AddEther(float _fEther);

	void AddAttack(float _fAttack);
	void AddArmor(float _fArmor);

	void AddSpeed(float _fSpeed);


public:
	// [ SET PART ]
	void InitCurStat(float _fHp = 0.f, float _fStamina = 0.f, float _fEther = 0.f,
		float _fAttack = 0.f, float _fArmor = 0.f, float _fSpeed = 0.f);
	void InitMaxStat(float _fHp = 0.f, float _fStamina = 0.f, float _fEther = 0.f,
		float _fAttack = 0.f, float _fArmor = 0.f, float _fSpeed = 0.f);
	
	void SetHP(float _fHp)				{ m_tCurStat.fHp = _fHp; }
	void SetStamina(float _fStamina)	{ m_tCurStat.fStamina = _fStamina; }
	void SetEther(float _fEther)		{ m_tCurStat.fEther = _fEther; }
	void SetAttack(float _fAttack)		{ m_tCurStat.fAttack = _fAttack; }
	void SetArmor(float _fArmor)		{ m_tCurStat.fArmor = _fArmor; }
	void SetSpeed(float _fSpeed)		{ m_tCurStat.fSpeed = _fSpeed; }

	void SetOwnerScript(CScript* _pScript) { m_pOwnerScript = _pScript; }



	// [ GET PART ]
	tSTAT GetStat() { return m_tCurStat; }
	tSTAT GetMaxStat() { return m_tMaxStat; }

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerStat)
public:
	CPlayerStat();
	CPlayerStat(const CPlayerStat& _origin);
	virtual ~CPlayerStat();
};

