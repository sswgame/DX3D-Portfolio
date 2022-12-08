#include "pch.h"
#include "FieldMonsteScript.h"
#include "FieldM_StateMgr.h"
#include "PlayerScript.h"

#include <random>

#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>
#include <Engine/CSerializer.h>


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
	, m_bIsChasing(false)
	, m_bChasingON(false)
{
	SetName(L"FieldMonsterScript");

	m_iFullHP = 100;
	m_fDetachRange = 500.f;
	m_fAttackRange = 300.f;
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
	, m_bIsChasing(false)
	, m_bChasingON(false)
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
	{
		MonsterRotation();
		return true;
	}
	else
		return false;
}

void FieldMonsteScript::MonsterRotation()
{
	if (L"WALK" != m_pMonsterMgr->GetCurState())
		return;

	Vec3 pCurMonsterPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pCurMonsterRotation = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 pNewMonsterRotation;

	Vec3 vPlayerPos = m_pPlayer->Transform()->GetRelativePos();

	Vec3 vBossFront = GetOwner()->Transform()->GetWorldFrontDir();
	vBossFront *= -1;

	// ������ ���� ���Ϳ� ����-�÷��̾� ���� ������ ���� ���Ѵ�.
	Vec3 v1(vBossFront.x, 0.f, vBossFront.z);
	Vec3 v2(vPlayerPos.x - pCurMonsterPos.x, 0.f, vPlayerPos.z - pCurMonsterPos.z);

	v1.Normalize();
	v2.Normalize();

	float dot = XMConvertToDegrees(v1.Dot(v2)); // 0 ~ 180
	Vec3  cross = v1.Cross(v2); // -1 ~ 1

	if (-5.f < dot && dot < 5.f)
		return;

	float fSpeed = 0;

	// ȸ�� ������ ã�´�
	if (0.f < cross.y)
		fSpeed = 50.f;
	else if (0.f > cross.y)
		fSpeed = -50.f;

	// ���� 5�� �̻��̸� �÷��̾� �������� ���������� ȸ���ϰ� 5�� �����̸� �÷��̸� �ٷ� �ٶ󺻴�.
	if (fabsf(dot) > 5.f)
	{
		Vec3 vBossRot = pCurMonsterRotation;
		vBossRot.ToDegree();
		vBossRot.y += DT * fSpeed;
		vBossRot.ToRadian();
		pNewMonsterRotation = vBossRot;
	}
	else
	{
		Vec3 vBossRot = pCurMonsterRotation;
		vBossRot.ToDegree();
		vBossRot.y = dot * (cross.y ? -1.f : 1.f);
		vBossRot.ToRadian();
		pNewMonsterRotation = vBossRot;
	}

	GetOwner()->Transform()->SetRelativeRotation(pNewMonsterRotation);
}

void FieldMonsteScript::PeaceStateRotation()
{
	// peace �����϶��� rotate
	Vec3 pMonsterRotation = GetOwner()->Transform()->GetRelativeRotation();

	std::random_device                 rdX;
	std::mt19937                       genX(rdX());
	std::uniform_int_distribution<int> disX(-60, 60);
	int angle = disX(genX);

	pMonsterRotation.y += angle;

	Vec3 vMonsterFront = GetOwner()->Transform()->GetWorldFrontDir();
	vMonsterFront *= -1;

	GetOwner()->Transform()->SetRelativeRotation(pMonsterRotation);
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

	// �ִϸ��̼� Ŭ�� ������ ������ ���� �ε�.
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

	m_iCurHP = m_iFullHP;
}

void FieldMonsteScript::update()
{
	if (m_bCurAnimationDone)
	{
		if (DetectPlayer())
		{
			if (false == m_bIsChasing)
			{
				// ��� �÷��̾ �ν�.
				m_pMonsterMgr->SetNextState(L"WALK");
				m_pMonsterMgr->SetRunTime(3.f);
				m_bCurAnimationDone = false;
				m_bIsChasing = true;
			}
			else
			{
				if (m_fCurDistance <= m_fAttackRange)
				{
					if (L"ATTACK" == m_pMonsterMgr->GetCurState())
					{
						// idle ���� ��ȯ.
						m_pMonsterMgr->SetNextState(L"IDLE");
						m_pMonsterMgr->SetRunTime(2.f);
						m_bCurAnimationDone = false;

					}
				}
				else
				{
					// ���� ���� ��������, ���� ������ �ƴ�. ��� �i�ư�
					m_pMonsterMgr->SetNextState(L"WALK");
					m_pMonsterMgr->SetRunTime(10.f);
					m_bCurAnimationDone = false;
					m_bChasingON = true;
				}
			}
		}
		else
		{
			m_bIsChasing = false;
			m_bChasingON = false;
			//  idle �ϰų� walk �ϰų�
			// Set RunTime
			if (L"IDLE" == m_pMonsterMgr->GetCurState())
			{
				PeaceStateRotation();
				m_pMonsterMgr->SetNextState(L"WALK");
			}
			else
				m_pMonsterMgr->SetNextState(L"IDLE");
			m_pMonsterMgr->SetRunTime(4.f);
		}
		m_bCurAnimationDone = false;

	}
	else
	{
		// �ٸ� attack range�� ������ �ٷ� �������� �Ѵ�.
		if (DetectPlayer())
		{
			if (L"WALK" == m_pMonsterMgr->GetCurState() && m_bChasingON)
			{
				if (m_fCurDistance <= m_fAttackRange)
				{
					m_pMonsterMgr->SetNextState(L"ATTACK");
					m_pMonsterMgr->SetRunTime(-1.f);
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
		// �÷��̾� ���� ������ �����´�.
		//((PlayerScript*)_OtherObject)->GetDamage

		m_pMonsterMgr->SetNextState(L"HIT");
		m_pMonsterMgr->SetRunTime(-1.f);
		m_bCurAnimationDone = false;
	}
}

void FieldMonsteScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void FieldMonsteScript::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_fDetachRange) << YAML::Value << m_fDetachRange;
	emitter << YAML::Key << NAME_OF(m_fAttackRange) << YAML::Value << m_fAttackRange;
	emitter << YAML::Key << NAME_OF(m_iFullHP) << YAML::Value << m_iFullHP;

	CScript::Serialize(emitter);
}

void FieldMonsteScript::Deserialize(const YAML::Node& node)
{
	m_fDetachRange = node[NAME_OF(m_fHP)].as<float>();
	m_fAttackRange = node[NAME_OF(m_fMaxHP)].as<float>();
	m_iFullHP = node[NAME_OF(m_fMaxHP)].as<int>();

	CScript::Deserialize(node);
}