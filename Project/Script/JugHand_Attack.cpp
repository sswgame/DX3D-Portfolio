#include "pch.h"
#include "JugHand_Attack.h"

#include "BossJugHandScript.h"

// Engine
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>

JugHand_Attack::JugHand_Attack()
	: CState{ L"ATTACK" }
	, m_pAnimation(nullptr)
	, m_bFirstAttackDone(false)
	, m_bSecondAttackDone(false)
	, m_bThirdAttackDone(false)
	, m_fLerfTime(0.f)
{
}

JugHand_Attack::~JugHand_Attack()
{
}

void JugHand_Attack::Hand01Attack()
{
	// [ Hand01 Attack routine ]
	// 1. ���� ����
	// 2. �˾��Ǹ� player �� ���� (~ 116 Frame)
	// 3. ������ ���������� player ���� x (117 Frame ~)
	// 4. �������� �� �� �ڸ����� ����� �����
	// 5. ���߿��� �˾��� �ٷ� 116 Frame ���� �����.

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vPlayerPos = ((BossJugHandScript*)pScript)->GetPlayerPosition();
	float fSpeed = ((BossJugHandScript*)pScript)->GetSpeed();
	float iDisappearaceTime = 2.f;
	float iAppearanceTime = 1.f;

	Vec3 vDir = vPlayerPos - vPos;
	vDir = vDir.Normalize();

	if (false == m_bFirstAttackDone)
	{
		if (m_pAnimation->GetCurFrameIdx() < 117)
		{
			vPos.x += vDir.x * DT * fSpeed;
			vPos.z += vDir.z * DT * fSpeed;
		}
		else
		{
			if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
			{
				SetAttackFinsh(1);
			}
			else
			{
				// ===============
				// �Ʒ��� ���� ���
				// ===============
			}
		}
	}
	else if (false == m_bSecondAttackDone)
	{
		if (117 == m_pAnimation->GetCurFrameIdx())
		{
			m_pAnimation->SetCurFrameIdx(117);
			vPos.x = vPlayerPos.x;
			vPos.z = vPlayerPos.z;
		}
		else if (m_pAnimation->GetCurFrameIdx() == m_pAnimation->GetEndFrameIdx())
		{
			SetAttackFinsh(2);
		}
		else
		{
			// ===============
			// �Ʒ��� ���� ���
			// ===============
		}
	}
	else if (false == m_bThirdAttackDone)
	{
		if (117 == m_pAnimation->GetCurFrameIdx())
		{
			m_pAnimation->SetCurFrameIdx(117);
			vPos.x = vPlayerPos.x;
			vPos.z = vPlayerPos.z;
		}
		else if (m_pAnimation->GetCurFrameIdx() == m_pAnimation->GetEndFrameIdx())
		{
			SetAttackFinsh(3);
			AllAttackDone();
		}
		else
		{
			// ===============
			// �Ʒ��� ���� ���
			// ===============
		}
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);

}

void JugHand_Attack::Hand02Attack()
{
	// [ Hand02 Attack routine ]
	// 1. �߾ӿ��� 238 �� ���� �ִϸ��̼� ����
	// 2. vanish
	// 3. ������ ��ġ���� gen
	// 3. 238 ���� �ִϸ��̼� ����
	// 3. �ִϸ��̼� ������ �����

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	float iDisappearaceTime = 2.f;
	float iAppearanceTime = 1.f;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();

	if (237 == m_pAnimation->GetCurFrameIdx())
	{
		SetAttackFinsh(1);
	}
	else
	{
		// ==============
		// Create Monster
		// ==============
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);

}


void JugHand_Attack::Hand03Attack()
{
	// [ Hand03 Attack routine ]
	// 1. runningTime ��ŭ ���� ����
	// 2. �ִϸ��̼� ������ ����ü �߻� + Dir ȸ��
	// 3. �ִϸ��̼� ������ �����

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();

	if (true == m_bThirdAttackDone)
	{
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
		return;
	}

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vPlayerPos = ((BossJugHandScript*)pScript)->GetPlayerPosition();
	Vec3 vMonsterCurDir = ((BossJugHandScript*)pScript)->GetCurDir();

	Vec3 vNewDir = vPlayerPos - vPos;
	vNewDir = vNewDir.Normalize();

	float fTheta = vMonsterCurDir.x * vNewDir.x + vMonsterCurDir.z * vNewDir.z;
	fTheta = acos(fTheta);

	if (vMonsterCurDir.x < vNewDir.x)
		fTheta = -fTheta;

	if (false == m_bThirdAttackDone)
	{
		if (m_pAnimation->GetCurFrameIdx() == m_pAnimation->GetEndFrameIdx())
		{
			if (false == m_bFirstAttackDone)
			{
				m_bFirstAttackDone = true;
			}
			else if (false == m_bSecondAttackDone)
			{
				m_bSecondAttackDone = true;
			}
			else if (false == m_bThirdAttackDone)
			{
				m_bThirdAttackDone = true;
				((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
			}
		}
		else if (120 == m_pAnimation->GetCurFrameIdx())
		{
			// ==================
			// create attack ball
			// ==================
		}
		else
		{
			vRotate.y += fTheta;
		}
	}


	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);
}

void JugHand_Attack::AllAttackDone()
{
	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();

	((BossJugHandScript*)pScript)->Set1stAttackDone(false);
	((BossJugHandScript*)pScript)->Set2ndAttackDone(false);
	((BossJugHandScript*)pScript)->Set3rdAttackDone(false);
}

void JugHand_Attack::SetAttackFinsh(int _attackNum)
{
	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();

	switch (_attackNum)
	{
	case 1:
	{
		m_bFirstAttackDone = true;
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
		((BossJugHandScript*)pScript)->Set1stAttackDone(m_bFirstAttackDone);
		((BossJugHandScript*)pScript)->SaveCurFrm(m_pAnimation->GetCurFrameIdx());

		break;
	}
	case 2:
	{
		m_bSecondAttackDone = true;
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
		((BossJugHandScript*)pScript)->Set2ndAttackDone(m_bSecondAttackDone);
		((BossJugHandScript*)pScript)->SaveCurFrm(m_pAnimation->GetCurFrameIdx());

		break;
	}
	case 3:
	{
		m_bThirdAttackDone = true;
		((BossJugHandScript*)pScript)->SetCurAnimationPlayDone(true);
		((BossJugHandScript*)pScript)->Set2ndAttackDone(m_bThirdAttackDone);
		((BossJugHandScript*)pScript)->SaveCurFrm(m_pAnimation->GetCurFrameIdx());

		break;
	}
	}
}

void JugHand_Attack::Enter()
{
	CState::ResetTimer();

	// ((BossJugHandScript*)pScript)->StartAttack();

	m_fLerfTime = 2.f;

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	int iIndex = ((BossJugHandScript*)pScript)->GetHandIndexNumber();
	wstring sAnimName = L"";

	if (nullptr == m_pAnimation)
	{
		map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();

		if (1 == iIndex)
		{
			sAnimName = L"Hand01_Smashing";
		}
		else if (2 == iIndex)
		{
			sAnimName = L"Hand02_Pattern2";
		}
		else
		{
			sAnimName = L"Hand03_Pattern3";
		}

		if (mapAnim.find(sAnimName) == mapAnim.end())
			assert(nullptr, L"�ִϸ��̼� Ŭ���� ã�� �� �����ϴ�. \n Hand State Script error");

		m_pAnimation = mapAnim.find(sAnimName)->second;
	}

	if (3 == iIndex)
	{
		GetOwner()->Animator3D()->Play(sAnimName, true);
		m_pAnimation->SetLerpTime(0.f);
	}
	else if (1 == iIndex)
	{
		GetOwner()->Animator3D()->Play(sAnimName, false);

		if (true == m_bFirstAttackDone)
		{
			m_pAnimation->SetCurFrameIdx(117);
		}
	}
	else if (2 == iIndex)
	{
		GetOwner()->Animator3D()->Play(sAnimName, false);
		if (true == m_bFirstAttackDone)
		{
			m_pAnimation->SetCurFrameIdx(238);
		}
	}


	//m_bFirstAttackDone = ((BossJugHandScript*)pScript)->Get1stAttackDone();
	//m_bSecondAttackDone = ((BossJugHandScript*)pScript)->Get2ndAttackDone();
	//m_bThirdAttackDone = ((BossJugHandScript*)pScript)->Get3rdAttackDone();


}

void JugHand_Attack::Update()
{
	CState::Update();

	float iDisappearaceTime = 2.f;
	float iAppearanceTime = 1.f;

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	int iIndex = ((BossJugHandScript*)pScript)->GetHandIndexNumber();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vPlayerPos = ((BossJugHandScript*)pScript)->GetPlayerPosition();


	if (1 == iIndex)
	{
		Hand01Attack();
	}
	else if (2 == iIndex)
	{
		Hand02Attack();
	}
	else
	{
		Hand03Attack();
	}


	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);
}

void JugHand_Attack::Exit()
{
	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();

	if (nullptr == pScript)
		return;

	((BossJugHandScript*)pScript)->SetRunningTime(0);

	m_bFirstAttackDone = false;
	m_bSecondAttackDone = false;
	m_bThirdAttackDone = false;

}
