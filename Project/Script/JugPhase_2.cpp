#include "pch.h"
#include "JugPhase_2.h"

// [ ENGINE PART]
#include <Engine/CFSM.h>
#include <Engine/CState.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

// [ SCRIPT PART ]
#include "BossJugCombatMgrScript.h"
#include "BossJugScript.h"
#include "ColumnFlameScript.h"

#define FLAME_COUNT 6

JugPhase_2::JugPhase_2()
	: CState(L"JUG_PHASE_2")
	, m_pCombatMgr(nullptr)
	, m_pBossFSM(nullptr)
	, m_pBossAnimator(nullptr)
	, m_fIdleTime(3.f)
	, m_iPrevAttackPattern(0)
	, m_iAttackPattern(0)
	, m_bAttackProceeding(false)
	, m_bRot(true)
	, m_fAttackTime(3.f)
{
}

JugPhase_2::JugPhase_2(const JugPhase_2& _origin)
	: CState(_origin.m_sStateType)
	, m_pCombatMgr(nullptr)
	, m_pBossFSM(nullptr)
	, m_pBossAnimator(nullptr)
	, m_fIdleTime(3.f)
	, m_iPrevAttackPattern(0)
	, m_iAttackPattern(0)
	, m_bAttackProceeding(false)
	, m_bRot(true)
	, m_fAttackTime(_origin.m_fAttackTime)
{
}

JugPhase_2::~JugPhase_2()
{
	vector<CGameObject*>::iterator iter = m_vecColumnFlames.begin();
	for (; iter != m_vecColumnFlames.end();)
	{
		iter = m_vecColumnFlames.erase(iter);
	}
}

void JugPhase_2::Init()
{
	m_pCombatMgr = GetOwner()->GetScript<BossJugCombatMgrScript>();
	assert(m_pCombatMgr);

	if (m_vecColumnFlames.empty())
	{
		for (int i = 0; i < FLAME_COUNT; i++)
		{
			Ptr<CPrefab> pColumn = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\ColumnLaser.pref");
			pColumn->SetName(L"Flame_" + std::to_wstring(i));
			m_vecColumnFlames.push_back(pColumn->Instantiate());
		}
	}
}

void JugPhase_2::Enter()
{
	m_pBossFSM = m_pCombatMgr->GetJug()->FSM();
	assert(m_pBossFSM);

	m_pBossAnimator = m_pCombatMgr->GetJug()->Animator3D();
	assert(m_pBossAnimator);

	// 현재 보스 상태를 JUG_SPAWNHAMMER로 바꾼다.
	m_pBossFSM->ChangeState(L"JUG_SPAWNHAMMER");
}

void JugPhase_2::Update()
{
	CState::Update();

	CState* pCurState = m_pBossFSM->GetCurState();

	// 만약 현재 상태가 SPAWNHAMMER이고 애니메이션이 종료된 상태라면 다시 IDLE 상태로 돌아온다
	if (L"JUG_SPAWNHAMMER" == pCurState->GetStateType())
	{
		if (ANIMATION_STATE::FINISH == m_pBossAnimator->GetCurAnim()->GetState())
			m_pBossFSM->ChangeState(L"JUG_HAMMER_IDLE");

		else
		{
			if (507 == m_pBossAnimator->GetCurAnim()->GetCurFrameIdx())
				m_pCombatMgr->GetHammer()->Activate();
		}
	}

	// 플래이어 방향으로 회전
	if (m_bRot && m_bAttackProceeding)
	{
		RotTowardPlayer();
	}

	switch (m_iAttackPattern)
	{
	case 0:
		if (GAME::BOSS::JUG_HAMMER_IDLE == m_pBossFSM->GetCurState()->GetStateType())
			ChangePattern();
		break;
	case 1:
		Attack_1();
		break;
	case 2:
		Attack_2();
		break;
	case 3:
		Attack_3();
		break;
	case 4:
		Attack_4();
		break;
	}
}

void JugPhase_2::Exit()
{
}

void JugPhase_2::LateUpdate()
{
	CState::LateUpdate();
}


void JugPhase_2::RotTowardPlayer()
{
	Vec3 vBossPos   = m_pCombatMgr->GetJug()->Transform()->GetWorldPos();
	Vec3 vPlayerPos = m_pCombatMgr->GetPlayer()->Transform()->GetWorldPos();

	Vec3 vBossFront = m_pCombatMgr->GetJug()->Transform()->GetWorldFrontDir();
	vBossFront *= -1;

	// 보스의 정면 벡터와 보스-플레이어 벡터 사이의 각을 구한다.
	Vec3 v1(vBossFront.x, 0.f, vBossFront.z);
	Vec3 v2(vPlayerPos.x - vBossPos.x, 0.f, vPlayerPos.z - vBossPos.z);

	v1.Normalize();
	v2.Normalize();

	float dot   = XMConvertToDegrees(v1.Dot(v2)); // 0 ~ 180
	Vec3  cross = v1.Cross(v2); // -1 ~ 1

	float fSpeed = 0;

	// 회전 방향을 찾는다
	if (0.f < cross.y)
		fSpeed = 20.f;
	else if (0.f > cross.y)
		fSpeed = -20.f;

	// 각이 5도 이상이면 플레이어 방향으로 지속적으로 회전하고 5도 이하이면 플레이를 바로 바라본다.
	if (fabsf(dot) > 5.f)
	{
		Vec3 vBossRot = m_pCombatMgr->GetJug()->Transform()->GetRelativeRotation();
		vBossRot.ToDegree();
		vBossRot.y += DT * fSpeed;
		vBossRot.ToRadian();
		m_pCombatMgr->GetJug()->Transform()->SetRelativeRotation(vBossRot);
	}
	else
	{
		Vec3 vBossRot = m_pCombatMgr->GetJug()->Transform()->GetRelativeRotation();
		vBossRot.ToDegree();
		vBossRot.y = dot * (cross.y ? -1.f : 1.f);
		vBossRot.ToRadian();
		m_pCombatMgr->GetJug()->Transform()->SetRelativeRotation(vBossRot);
	}
}

void JugPhase_2::ChangePattern()
{
	if (GetTimer() < m_fIdleTime)
		return;

	ResetTimer();

	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	// 1 부터 4 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	std::uniform_int_distribution<int> dis(1, 4);

	m_iAttackPattern = dis(gen);

	if (m_iAttackPattern == m_iPrevAttackPattern)
	{
		++m_iAttackPattern;

		if (m_iAttackPattern > 4)
			m_iAttackPattern = 1;

		m_iPrevAttackPattern = 0;
	}
}

void JugPhase_2::Attack_1()
{
	if (!m_bAttackProceeding)
	{
		m_bAttackProceeding = true;
		m_pBossFSM->ChangeState(GAME::BOSS::JUG_ATTACK_0);
	}
	else
	{
		if (GetTimer() > m_fAttackTime)
		{
			m_pBossFSM->ChangeState(GAME::BOSS::JUG_HAMMER_IDLE);
			m_bAttackProceeding  = false;
			m_iPrevAttackPattern = m_iAttackPattern;
			m_iAttackPattern     = 0;
			ResetTimer();
		}
	}
}

void JugPhase_2::Attack_2()
{
	if (!m_bAttackProceeding)
	{
		m_bAttackProceeding = true;
		m_pBossFSM->ChangeState(GAME::BOSS::JUG_ATTACK_1);
	}
	else
	{
		if (GetTimer() > m_fAttackTime)
		{
			m_pBossFSM->ChangeState(GAME::BOSS::JUG_HAMMER_IDLE);
			m_bAttackProceeding  = false;
			m_iPrevAttackPattern = m_iAttackPattern;
			m_iAttackPattern     = 0;
			ResetTimer();
		}
	}
}

void JugPhase_2::Attack_3()
{
	if (!m_bAttackProceeding)
	{
		m_bAttackProceeding = true;
		m_pBossFSM->ChangeState(GAME::BOSS::JUG_ATTACK_0);
	}
	else
	{
		if (GetTimer() > m_fAttackTime)
		{
			m_pBossFSM->ChangeState(GAME::BOSS::JUG_HAMMER_IDLE);
			m_bAttackProceeding  = false;
			m_iPrevAttackPattern = m_iAttackPattern;
			m_iAttackPattern     = 0;
			ResetTimer();
		}
	}
}

void JugPhase_2::Attack_4()
{
	if (!m_bAttackProceeding)
	{
		m_bAttackProceeding = true;
		m_pBossFSM->ChangeState(GAME::BOSS::JUG_ATTACK_0);
	}
	else
	{
		if (GetTimer() > m_fAttackTime)
		{
			m_pBossFSM->ChangeState(GAME::BOSS::JUG_HAMMER_IDLE);
			m_bAttackProceeding  = false;
			m_iPrevAttackPattern = m_iAttackPattern;
			m_iAttackPattern     = 0;
			ResetTimer();
		}
	}
}
