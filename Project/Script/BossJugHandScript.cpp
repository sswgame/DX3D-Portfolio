#include "pch.h"
#include "BossJugHandScript.h"

#include "Hand_StateMgr.h"
#include <random>

#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CGameObject.h>

BossJugHandScript::BossJugHandScript()
	: CScript{ (int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT }
	, m_iOwnerHandIdx(-1)
	, m_vDirection(Vec3(0.f, 0.f, -1.f))
	, m_vPrevDirection(Vec3(0.f, 0.f, 0.f))
	, m_bAnimDone(true)
	, m_fRunningTime(0.f)
	, m_fSpeed(0.f)
	, m_iCurAttackHandIdx(-1)
{
}

BossJugHandScript::BossJugHandScript(const BossJugHandScript* _origin)
	: CScript{ (int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT }
	, m_iOwnerHandIdx(_origin->m_iOwnerHandIdx)
	, m_vDirection(_origin->m_vDirection)
	, m_vPrevDirection(_origin->m_vPrevDirection)
	, m_bAnimDone(_origin->m_bAnimDone)
	, m_fRunningTime(_origin->m_fRunningTime)
	, m_fSpeed(_origin->m_fSpeed)
	, m_iCurAttackHandIdx(_origin->m_iCurAttackHandIdx)
{
}

BossJugHandScript::~BossJugHandScript()
{
}

Vec3 BossJugHandScript::GetPlayerPosition()
{
	vector<CGameObject*> pVecObjs = CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetObjects();
	CGameObject* pPlayer = nullptr;

	for (size_t i = 0; i < pVecObjs.size(); i++)
	{
		if (nullptr != pVecObjs[i]->GetScriptByName(L"PlayerScript"))
		{
			pPlayer = pVecObjs[i];
		}
	}

	if (nullptr == pPlayer)
		assert(L"Hand Script Error", L"Player is not exist");

	return pPlayer->Transform()->GetRelativePos();
}

void BossJugHandScript::InitMonsterStat()
{
	m_fSpeed = 100.f;
}

void BossJugHandScript::start()
{
	wstring wOwnerName = GetOwner()->GetName();

	if (L"Hand01" == wOwnerName)
	{
		m_iOwnerHandIdx = 1;
	}
	else if (L"Hand02" == wOwnerName)
	{
		m_iOwnerHandIdx = 2;
	}
	else if (L"Hand03" == wOwnerName)
	{
		m_iOwnerHandIdx = 3;
	}


	// 애니메이션 클립 정보가 없을시 직접 로드.
	map<wstring, CAnimation3D*> mapAnim = GetOwner()->Animator3D()->GetAllAnim();
	if (1 == mapAnim.size())
	{
		if (1 == m_iOwnerHandIdx)
		{
			GetOwner()->Animator3D()->MakeAnimationFromTXT("JugHand01AnimInfo.txt");
		}
		else if (2 == m_iOwnerHandIdx)
		{
			GetOwner()->Animator3D()->MakeAnimationFromTXT("JugHand02AnimInfo.txt");
		}
		else
		{
			GetOwner()->Animator3D()->MakeAnimationFromTXT("JugHand03AnimInfo.txt");
		}
	}

	// stateMgr에서 현재 공격중인 손의 num을 가져온다.
	m_iCurAttackHandIdx = GetOwner()->GetScript<Hand_StateMgr>()->GetCurAttackHandNUM();

	// 모든 손의 현재 상태는 Gen 으로 두고 시작한다.
	// 모든 상태는 같지만 공격 idx 와 같은 손만 update 된다.
	GetOwner()->GetScript<Hand_StateMgr>()->SetNextState(L"GEN");
}

void BossJugHandScript::update()
{
	if (-1 == GetOwner()->GetScript<Hand_StateMgr>()->GetCurAttackHandNUM())
		return;

	CScript* pMgrScript = GetOwner()->GetScript<Hand_StateMgr>();

	// 애니메이션이 끝났다는 알림> bool 값이 오면 상태전환
	if (m_iOwnerHandIdx == m_iCurAttackHandIdx)
	{
		if (m_bAnimDone)
		{
			wstring sCurStateName = ((Hand_StateMgr*)pMgrScript)->GetCurState();

			if (L"GEN" == sCurStateName)
			{
				((Hand_StateMgr*)pMgrScript)->SetNextState(L"ATTACK");
			}
			else if (L"ATTACK" == sCurStateName)
			{
				((Hand_StateMgr*)pMgrScript)->SetNextState(L"VANISH");

			}
			else if (L"VANISH" == sCurStateName)
			{
				// vanish 까지 왔지만 모든 공격이 끝난게 아니라면 다시 Gen 부터 공격
				if (false == GetOwner()->GetScript<Hand_StateMgr>()->GetAllAttackDone())
				{
					((Hand_StateMgr*)pMgrScript)->SetNextState(L"GEN");
				}
				else
					((Hand_StateMgr*)pMgrScript)->SetNextState(L"IDLE");

			}

		}
	}
	else
	{
		// 현재 진행중인 Hand의 idx가 owner 의 index와 다를경우
		// 진행하지 않음.
		return;
	}
}

void BossJugHandScript::lateupdate()
{
}

void BossJugHandScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void BossJugHandScript::OnCollision(CGameObject* _OtherObject)
{
}

void BossJugHandScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void BossJugHandScript::SaveToScene(FILE* _pFile)
{
}

void BossJugHandScript::LoadFromScene(FILE* _pFile)
{
}
