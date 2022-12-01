#include "pch.h"
#include "JugHand_Attack.h"

#include "BossJugHandScript.h"

// Engine
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

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
	// 1. 상태 진행
	// 2. 팝업되면 player 를 추적 (~ 116 Frame)
	// 3. 추적후 내려찍을땐 player 추적 x (117 Frame ~)
	// 4. 내려찍은 후 그 자리에서 멈춘뒤 사라짐
	// 5. 공중에서 팝업후 바로 116 Frame 부터 재시작.

	CScript* pScript = pScript = GetOwner()->GetScript<BossJugHandScript>();
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vPlayerPos = ((BossJugHandScript*)pScript)->GetPlayerPosition();
	float fSpeed = ((BossJugHandScript*)pScript)->GetSpeed();

	bool repeat = ((BossJugHandScript*)pScript)->GetAttackRepeat();

	Vec3 vDir = vPlayerPos - vPos;
	vDir = vDir.Normalize();

	if (repeat) // 마지막 공격이 아니다 (첫번째 공격이다)
	{
		if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
		{
			((BossJugHandScript*)pScript)->SetAttackStateDone(true);
			m_pAnimation->SetAnimState(ANIMATION_STATE::STOP);
			m_bFirstAttackDone = true;
		}
		else if (2.f < CState::GetTimer())
		{
			if (ANIMATION_STATE::STOP == m_pAnimation->GetState())
				m_pAnimation->SetAnimState(ANIMATION_STATE::PLAY);

			// =======
			// 내려찍기
			// =======
		}
		else
		{
			vPos.x += vDir.x * DT * fSpeed;
			vPos.z += vDir.z * DT * fSpeed;
		}
	}
	else
	{
		if (ANIMATION_STATE::PLAY == m_pAnimation->GetState())
		{
			if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
			{
				((BossJugHandScript*)pScript)->SetAttackStateDone(true);
				GetOwner()->GetScript<BossJugHandScript>()->SetAllAttackDone(true);
				m_bFirstAttackDone = false;
				((BossJugHandScript*)pScript)->SetAttackRepeat(false);
			}
			else
			{
				// =======
				// 내려찍기
				// =======
			}
		}
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);


}

void JugHand_Attack::Hand02Attack()
{
	// [ Hand02 Attack routine ]
	// 1. 중앙에서 238 전 까지 애니메이션 진행
	// 2. vanish
	// 3. 랜덤한 위치에서 gen
	// 3. 238 부터 애니메이션 시작
	// 3. 애니메이션 끝나면 사라짐

	CScript* pScript = GetOwner()->GetScript<BossJugHandScript>();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();

	if (ANIMATION_STATE::PLAY == m_pAnimation->GetState())
	{
		if (false == m_bFirstAttackDone)
		{
			if (237 == m_pAnimation->GetCurFrameIdx())
			{
				// ==========================
				//		던지는 파티클 생성  
				// ==========================

				// ==========================
				//	       magma 생성  
				// ==========================
				//CGameObject* pMagma = new CGameObject;
				//wstring pMagmaName = L"";
				//pMagmaName = L"meshdata//magma0.mdat";
				//Ptr<CMeshData> pMagmaMeshData = CResMgr::GetInst()->Load<CMeshData>(pMagmaName.c_str(),
				//	pMagmaName.c_str());
				//pMagma = pMagmaMeshData->Instantiate();
				//pMagma->Transform()->SetRelativeScale(Vec3(1.f, 0.f, 1.f));
				//pMagma->SetName(L"Magma");
				////pMagma->AddComponent(new MagmaScript);
				//GetOwner()->AddChild(pMagma);

			}
			else if (m_pAnimation->GetCurFrameIdx() == m_pAnimation->GetEndFrameIdx())
			{
				((BossJugHandScript*)pScript)->SetAttackStateDone(true);
				GetOwner()->GetScript<BossJugHandScript>()->SetAllAttackDone(true);
				m_bFirstAttackDone = false;
				return;
			}
		}
	}

	//if (ANIMATION_STATE::FINISH == m_pAnimation->GetState())
	//{
	//	((BossJugHandScript*)pScript)->SetAttackStateDone(true);
	//	GetOwner()->GetScript<BossJugHandScript>()->SetAllAttackDone(true);
	//	m_bFirstAttackDone = false;
	//	return;
	//}

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);

}


void JugHand_Attack::Hand03Attack()
{
	CScript* pScript = GetOwner()->GetScript<BossJugHandScript>();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotate = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vPlayerPos = ((BossJugHandScript*)pScript)->GetPlayerPosition();
	Vec3 vMonsterCurDir = ((BossJugHandScript*)pScript)->GetCurDir();

	Vec2 vPlayertoMonsterDir = Vec2(vPos.x - vPlayerPos.x, vPos.z - vPlayerPos.z);
	vPlayertoMonsterDir.Normalize();

	Vec2 vNewMonsterVec = -vPlayertoMonsterDir;

	((BossJugHandScript*)pScript)->SetMonsterDir(Vec3(vNewMonsterVec.x, 0, vNewMonsterVec.y));

	float fTheta = vMonsterCurDir.x * vNewMonsterVec.x + vMonsterCurDir.z * vNewMonsterVec.y;
	fTheta = acos(fTheta);

	if (vPos.x < vPlayerPos.x)
		fTheta = fTheta + XM_PI / 2.f;
	else
		fTheta = -fTheta + XM_PI / 2.f;

	if (m_pAnimation->GetCurFrameIdx() == m_pAnimation->GetEndFrameIdx())
	{
		if (((BossJugHandScript*)pScript)->GetRunningTime() < CState::GetTimer())
		{
			((BossJugHandScript*)pScript)->SetAttackStateDone(true);
			GetOwner()->GetScript<BossJugHandScript>()->SetAllAttackDone(true);
			m_bFirstAttackDone = false;
			m_bSecondAttackDone = false;
			m_bThirdAttackDone = false;
			return;
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

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativePos(vRotate);
}

void JugHand_Attack::Enter()
{
	CState::ResetTimer();

	CScript* pScript = GetOwner()->GetScript<BossJugHandScript>();

	m_fLerfTime = 2.f;

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
			assert(nullptr && L"애니메이션 클립을 찾을 수 없습니다. \n Hand State Script error");

		m_pAnimation = mapAnim.find(sAnimName)->second;
	}

	sAnimName = m_pAnimation->GetName();

	if (3 == iIndex)
	{
		float fHand3RunTime = (float)(m_pAnimation->GetEndTime() - m_pAnimation->GetStartTime());
		fHand3RunTime *= 3.f;

		GetOwner()->Animator3D()->Play(sAnimName, true);
		m_pAnimation->SetLerpTime(0.f);
		((BossJugHandScript*)pScript)->SetRunningTime(fHand3RunTime);
	}
	else if (1 == iIndex)
	{
		m_pAnimation->SetLerpTime(0.f);

		float fRunTime = (float)(m_pAnimation->GetEndTime() - m_pAnimation->GetStartTime());

		if (true == m_bFirstAttackDone)
		{
			((BossJugHandScript*)pScript)->SetRunningTime(fRunTime);
			GetOwner()->Animator3D()->Play(sAnimName, false);
			((BossJugHandScript*)pScript)->SetAttackRepeat(false);
		}
		else
		{
			((BossJugHandScript*)pScript)->SetRunningTime(fRunTime + 2.f);
			m_pAnimation->SetAnimState(ANIMATION_STATE::STOP);
			((BossJugHandScript*)pScript)->SetAttackRepeat(true);
		}
	}
	else if (2 == iIndex)
	{
		GetOwner()->Animator3D()->Play(sAnimName, false);
		m_pAnimation->SetLerpTime(0.5f);
		if (true == m_bFirstAttackDone)
		{
			m_pAnimation->SetCurFrameIdx(238);
		}
	}
}

void JugHand_Attack::Update()
{
	CState::Update();

	int curAttackIndex = GetOwner()->GetParent()->GetScript<HandStateMgrScript>()->GetCurAttackHandNUM();
	int iIndex = GetOwner()->GetScript<BossJugHandScript>()->GetHandIndexNumber();


	if (curAttackIndex == iIndex)
	{
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
	}
}

void JugHand_Attack::Exit()
{
	m_pAnimation->Reset();
	CScript* pScript = GetOwner()->GetScript<BossJugHandScript>();

	if (nullptr == pScript)
		return;

	((BossJugHandScript*)pScript)->SetRunningTime(0);
}
