#include "pch.h"
#include "FieldMonsteScript.h"
#include "FieldM_StateMgr.h"
#include "PlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>

FieldMonsteScript::FieldMonsteScript()
	: CScript{ (int)SCRIPT_TYPE::FIELDMONSTESCRIPT }
	, m_pMonsterMgr(nullptr)
	, m_vColliderSize()
	, m_pOwnerMonster(nullptr)
	, m_pPlayer(nullptr)
	, m_fDetachRange(0.f)
	, m_fCurDistance(0.f)
	, m_fAttackRange(0.f)
	, m_iFullHP(0)
	, m_iCurHP(0)
	, m_eMonsterType(FieldMonsterType::NONE)
	, m_bCurAnimationDone(true)
	, m_fCoolTime(0.f)
	, m_bIsChasing(false)
{
	SetName(L"FieldMonsterScript");

	m_fDetachRange = 500.f;
	m_fAttackRange = 100.f;
}

FieldMonsteScript::FieldMonsteScript(const FieldMonsteScript& _origin)
	: CScript{ (int)SCRIPT_TYPE::FIELDMONSTESCRIPT }
	, m_pMonsterMgr(_origin.m_pMonsterMgr)
	, m_vColliderSize(_origin.m_vColliderSize)
	, m_pOwnerMonster(_origin.m_pOwnerMonster)
	, m_pPlayer(_origin.m_pPlayer)
	, m_fDetachRange(_origin.m_fDetachRange)
	, m_fCurDistance(_origin.m_fCurDistance)
	, m_fAttackRange(_origin.m_fAttackRange)
	, m_iFullHP(_origin.m_iFullHP)
	, m_iCurHP(_origin.m_iCurHP)
	, m_eMonsterType(_origin.m_eMonsterType)
	, m_bCurAnimationDone(true)
	, m_fCoolTime(0.f)
	, m_bIsChasing(false)
{
	SetName(L"FieldMonsterScript");
}

FieldMonsteScript::~FieldMonsteScript()
{
	SAFE_DELETE(m_pMonsterMgr);
}

bool FieldMonsteScript::DetectPlayer()
{
	if (nullptr == m_pPlayer)
	{
		vector<CGameObject*> pPlayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(3)->GetObjects();

		for (size_t i = 0; i < pPlayer.size(); i++)
		{
			if (nullptr != pPlayer[i]->GetScript<PlayerScript>())
			{
				m_pPlayer = pPlayer[i];
			}
		}
	}

	if (nullptr == m_pPlayer)
		return false;

	Vec3 vPlayerPos = m_pPlayer->Transform()->GetRelativePos();
	Vec3 vMonsterPOs = m_pOwnerMonster->Transform()->GetRelativePos();

	m_fCurDistance = sqrt((vPlayerPos.x - vMonsterPOs.x) * (vPlayerPos.x - vMonsterPOs.x) +
		(vPlayerPos.y - vMonsterPOs.y) * (vPlayerPos.y - vMonsterPOs.y) +
		(vPlayerPos.z - vMonsterPOs.z) * (vPlayerPos.z - vMonsterPOs.z));

	if (m_fCurDistance <= m_fDetachRange)
		return true;
	else
		return false;
}

void FieldMonsteScript::ChangeState()
{
	SetCoolTimeReset();
	//change state IDLE
	m_bCurAnimationDone = true;
}

void FieldMonsteScript::start()
{
	if (nullptr == m_pOwnerMonster)
	{
		m_pOwnerMonster = GetOwner();
	}

	if (nullptr == m_pMonsterMgr)
	{
		m_pMonsterMgr = new FieldM_StateMgr;
		m_pMonsterMgr->InitState(GetOwner());
	}

	// 애니메이션 클립 정보가 없을시 직접 로드.
	map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
	if (1 == mapAnim.size())
	{
		if (m_eMonsterType == FieldMonsterType::HOMONCULUS)
		{
			GetOwner()->Animator3D()->MakeAnimationFromTXT("HomonculusAnimInfo.txt");
		}
		else if (m_eMonsterType == FieldMonsterType::DEUXIEMIE)
		{
			GetOwner()->Animator3D()->MakeAnimationFromTXT("DeuxiemeAnimInfo.txt");
		}
		else
			return;
	}

}

void FieldMonsteScript::update()
{
	if (m_bCurAnimationDone)
	{
		if (DetectPlayer())
		{
			if (false == m_bIsChasing)
			{
				// 방금 플레이어를 인식.
				m_pMonsterMgr->SetNextState(L"WALK");
				m_pMonsterMgr->SetRunTime(3.f);
				// dir 회전
				m_bIsChasing = true;
			}
			else
			{
				if (0.f < m_fCoolTime)
				{
					m_fCoolTime -= DT;
				}
				else
				{
					if (L"WALK" == m_pMonsterMgr->GetCurState()
						|| L"IDLE" == m_pMonsterMgr->GetCurState())
					{
						if (m_fCurDistance < m_fAttackRange)
						{
							// attack 모드로 전환.
							m_pMonsterMgr->SetNextState(L"ATTACK");
							m_pMonsterMgr->SetRunTime(-1.f);
							m_fCoolTime = 2.f;
						}
						else
						{	// Set RunTime
							m_pMonsterMgr->SetNextState(L"WALK");
							m_pMonsterMgr->SetRunTime(3.f);
						}
					}
					else
					{
						m_pMonsterMgr->SetNextState(L"ATTACK");
						m_pMonsterMgr->SetRunTime(-1.f);
						m_fCoolTime = 2.f;
					}
				}
			}
		}
		else
		{
			m_bIsChasing = false;
			//  idle 하거나 walk 하거나
			// Set RunTime
			if (L"IDLE" == m_pMonsterMgr->GetCurState())
				m_pMonsterMgr->SetNextState(L"WALK");
			else
				m_pMonsterMgr->SetNextState(L"IDLE");
			m_pMonsterMgr->SetRunTime(4.f);
		}
		m_bCurAnimationDone = false;

	}
	else
	{
		// 다만 attack range에 들어오면 바로 공격으로 한다.
		if (DetectPlayer())
		{
			if (L"ATTACK" != m_pMonsterMgr->GetCurState())
			{
				if (m_fCurDistance <= m_fAttackRange)
				{
					m_pMonsterMgr->SetNextState(L"ATTACK");
					m_pMonsterMgr->SetRunTime(-1.f);
					m_fCoolTime = 2.f;
					m_bCurAnimationDone = false;
				}
			}
		}

		return;
	}

	m_pMonsterMgr->update();
}

void FieldMonsteScript::lateupdate()
{
	m_pMonsterMgr->lateUpdate();
}

void FieldMonsteScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void FieldMonsteScript::OnCollision(CGameObject* _OtherObject)
{
	if (nullptr != _OtherObject->GetScript<PlayerScript>())
	{
		// 플레이어 한테 데미지 가져온다.
		//((PlayerScript*)_OtherObject)->GetDamage

		m_pMonsterMgr->SetNextState(L"HIT");
		m_pMonsterMgr->SetRunTime(-1.f);
	}
}

void FieldMonsteScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
