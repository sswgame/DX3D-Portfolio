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


	// �ִϸ��̼� Ŭ�� ������ ������ ���� �ε�.
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

	// stateMgr���� ���� �������� ���� num�� �����´�.
	m_iCurAttackHandIdx = GetOwner()->GetScript<Hand_StateMgr>()->GetCurAttackHandNUM();

	// ��� ���� ���� ���´� Gen ���� �ΰ� �����Ѵ�.
	// ��� ���´� ������ ���� idx �� ���� �ո� update �ȴ�.
	GetOwner()->GetScript<Hand_StateMgr>()->SetNextState(L"GEN");
}

void BossJugHandScript::update()
{
	if (-1 == GetOwner()->GetScript<Hand_StateMgr>()->GetCurAttackHandNUM())
		return;

	CScript* pMgrScript = GetOwner()->GetScript<Hand_StateMgr>();

	// �ִϸ��̼��� �����ٴ� �˸�> bool ���� ���� ������ȯ
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
				// vanish ���� ������ ��� ������ ������ �ƴ϶�� �ٽ� Gen ���� ����
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
		// ���� �������� Hand�� idx�� owner �� index�� �ٸ����
		// �������� ����.
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
#include "pch.h"
#include "BossJugHandScript.h"

#include "CHand_StateMgr.h"
#include <random>

#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>

BossJugHandScript::BossJugHandScript()
	: CScript{(int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT}
	, m_pStateMgr(nullptr)
	, m_vDirection(Vec3(0.f, 0.f, -1.f))
	, m_vPrevDirection(Vec3(0.f, 0.f, 0.f))
	, m_pBossObj(nullptr)
	, m_fRunningTime(0)
	, m_fSpeed(0)
	, m_iOwnerHandIdx(-1)
	, m_iCurAttackHandIdx(0)
	, m_bAnimDone(true)
{
}

BossJugHandScript::BossJugHandScript(const BossJugHandScript& _origin)
	: CScript{(int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT}
	, m_pStateMgr(_origin.m_pStateMgr)
	, m_vDirection(_origin.m_vDirection)
	, m_vPrevDirection(_origin.m_vPrevDirection)
	, m_pBossObj(_origin.m_pBossObj)
	, m_fRunningTime(_origin.m_fRunningTime)
	, m_fSpeed(_origin.m_fSpeed)
	, m_iOwnerHandIdx(_origin.m_iOwnerHandIdx)
	, m_iCurAttackHandIdx(-1)
	, m_bAnimDone(_origin.m_bAnimDone)
{
}

BossJugHandScript::~BossJugHandScript()
{
	SAFE_DELETE(m_pStateMgr);
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
		assert(L"Hand Script Error" && L"Player is not exist");

	return pPlayer->Transform()->GetRelativePos();
}

void BossJugHandScript::InitMonsterStat()
{
	m_fSpeed = 100.f;
}

void BossJugHandScript::start()
{
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CHand_StateMgr;
		m_pStateMgr->InitState(GetOwner());
	}

	InitMonsterStat();

	m_iOwnerHandIdx = 3;

	// �ִϸ��̼� Ŭ�� ������ ������ ���� �ε�.
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

	m_pStateMgr->SetNextState(L"GEN");


	// Hand�� Ư���� ���� ���� ������Ʈ�� �˰� �ִ´�.
	// Hand �� 1���� ���ݻ��¿��� �ϱ� ������ �������� 
	// ���� ���ݻ����� Hand Num�� �����´�.
	if (nullptr == m_pBossObj)
	{
		vector<CGameObject*> pVecObjs = CSceneMgr::GetInst()->GetCurScene()->GetLayer(5)->GetObjects();

		for (size_t i = 0; i < pVecObjs.size(); i++)
		{
			if (nullptr != pVecObjs[i]->GetScriptByName(L"BossJugScript"))
			{
				m_pBossObj = pVecObjs[i];
			}
			else
				assert(L"Boss_Jug is not Exist");
		}

	}

	if (-1 == m_iOwnerHandIdx)
		assert(L"Please Set Hand Index.");

}

void BossJugHandScript::update()
{
	// ���� �ִϸ��̼��� ������̶�� Mgr�� update
	if (false == m_bAnimDone)
	{
		m_pStateMgr->Update();
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
