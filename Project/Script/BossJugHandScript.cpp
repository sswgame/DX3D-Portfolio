#include "pch.h"
#include "BossJugHandScript.h"
#include <random>

// < ENGINE >
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CGameObject.h>

// < SCRIPT >
#include "PlayerScript.h"
#include "CPlayerStat.h"
#include "BossJugScript.h"
#include "CObjectManager.h"
#include "PaperBurnScript.h"
#include <Engine/CSerializer.h>
BossJugHandScript::BossJugHandScript()
	: CScript{ (int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT }
	, m_vDirection(Vec3(0.f, 0.f, -1.f))
	, m_vPrevDirection(Vec3(0.f, 0.f, 0.f))
	, m_sCurState(L"")
	, m_fRunningTime(0.f)
	, m_fSpeed(300.f)
	, m_iCurAttackHandIdx(-1)
	, m_iOwnerHandIdx(-1)
	, m_bAllAttackIsDone(false)
	, m_bGenStateDone(false)
	, m_bAttackStateDone(false)
	, m_bVanishStateDone(false)
	, m_bAttackRepeat(false)
	, m_pPlayer(nullptr)
	, m_bPaperburnOn(false)
{
}

BossJugHandScript::BossJugHandScript(const BossJugHandScript& _origin)
	: CScript{ (int)SCRIPT_TYPE::BOSSJUGHANDSCRIPT }
	, m_vDirection(_origin.m_vDirection)
	, m_vPrevDirection(_origin.m_vPrevDirection)
	, m_fRunningTime(_origin.m_fRunningTime)
	, m_fSpeed(_origin.m_fSpeed)
	, m_iOwnerHandIdx(_origin.m_iOwnerHandIdx)
	, m_iCurAttackHandIdx(_origin.m_iCurAttackHandIdx)
	, m_bAllAttackIsDone(_origin.m_bAllAttackIsDone)
	, m_bGenStateDone(_origin.m_bGenStateDone)
	, m_bAttackStateDone(_origin.m_bAttackStateDone)
	, m_bVanishStateDone(_origin.m_bVanishStateDone)
	, m_bAttackRepeat(_origin.m_bAttackRepeat)
{
}

BossJugHandScript::~BossJugHandScript()
{
}

Vec3 BossJugHandScript::GetPlayerPosition()
{
	// player is not exits
	//return Vec3(0, 0, 0);

	if (nullptr == m_pPlayer)
	{
		return Vec3(0.f, 0.f, 0.f);
	}

	return m_pPlayer->Transform()->GetRelativePos();
}

void BossJugHandScript::InitMonsterStat()
{
	m_fSpeed = 100.f;
}

void BossJugHandScript::start()
{
	CGameObject* pHandMgr = GetOwner()->GetParent();
	m_sCurState = pHandMgr->GetScript<HandStateMgrScript>()->GetCurState();

	string curstate;
	if (L"GEN" == m_sCurState)
	{
		curstate = "GEN";
	}
	else if (L"ATTACK" == m_sCurState)
	{
		curstate = "ATTACK";
	}
	else if (L"VANISH" == m_sCurState)
	{
		curstate = "VANISH";
	}

	AddScriptParam("ATTACK NUM", SCRIPTPARAM_TYPE::INT, &m_iCurAttackHandIdx);
	AddScriptParam("OWNER NUM", SCRIPTPARAM_TYPE::INT, &m_iOwnerHandIdx);
	AddScriptParam("HAND STATE", SCRIPTPARAM_TYPE::TEXT, &curstate);

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

	// ��� ���� ���� ���´� Gen ���� �ΰ� �����Ѵ�.
	// ��� ���´� ������ ���� idx �� ���� �ո� update �ȴ�.
	pHandMgr->GetScript<HandStateMgrScript>()->SetNextState(L"GEN");

}

void BossJugHandScript::update()
{
	CGameObject* pHandMgr = GetOwner()->GetParent();
	CScript* pMgrScript = pHandMgr->GetScript<HandStateMgrScript>();

	m_iCurAttackHandIdx = pHandMgr->GetScript<HandStateMgrScript>()->GetCurAttackHandNUM();

	// attackIndex �� -1�Ͻ� �ƹ��͵� ���� ����
	if (-1 == m_iCurAttackHandIdx)
		return;

	if (nullptr == m_pPlayer)
	{
		m_pPlayer = CObjectManager::GetInst()->GetPlayer();
	}

	// ������ ���� ��ȣ�� ��ġ���� ������ update ���� ����.
	if (m_iOwnerHandIdx != m_iCurAttackHandIdx)
	{
		if (true == GetOwner()->IsActive())
		{
			GetOwner()->Deactivate();

		}
		return;
	}
	else
	{
		if (false == GetOwner()->IsActive())
		{
			GetOwner()->Activate();
		}
	}


	// ���� routine
	// 
	// ����Ŭ�� �������� ����.
	// 1.	����Ŭ ����.	
	// 
	// 2.	���� ��� �������� ����.
	//		m_bGenStateDone / m_bAttackStateDone / m_bVanishStateDone 
	//		�� �����϶� stateDone �� True ��� ���� ���·� ��ȯ
	// 
	// 3.	���� ���� �������϶��� �ƹ� �ϵ� ���� �ʴ´�.
	// 
	// 4.	vanish ���� m_bAllAttackIsDone�� Ȱ��ȭ �Ǹ� ��ũ��Ʈ ����.
	//		�ƴ϶�� GEN ���� ���� ����.
	//		vanish�� ����Ǹ� ��� ���� reset


	wstring sCurStateName = ((HandStateMgrScript*)pMgrScript)->GetCurState();

	if (L"GEN" == sCurStateName)
	{
		if (m_bPaperburnOn == false)
		{
			m_bPaperburnOn = true;
		}

		if (m_bGenStateDone)
		{
			pHandMgr->GetScript<HandStateMgrScript>()->SetNextState(L"ATTACK");
		}
	}
	else if (L"ATTACK" == sCurStateName)
	{
		m_bPaperburnOn = false;
		if (m_bAttackStateDone)
		{
			pHandMgr->GetScript<HandStateMgrScript>()->SetNextState(L"VANISH");

		}
		else
		{
			if (m_bVanishStateDone)
				m_bVanishStateDone = false;
		}
	}
	else if (L"VANISH" == sCurStateName)
	{
		if (m_bVanishStateDone)
		{
			m_bGenStateDone = false;
			m_bAttackStateDone = false;
			m_bVanishStateDone = false;

			if (true == m_bAllAttackIsDone)
			{
				if (false == m_bAttackRepeat)
				{
					pHandMgr->GetScript<HandStateMgrScript>()->ResetCurAttackHandNumber();
			
				}
				pHandMgr->GetScript<HandStateMgrScript>()->SetNextState(L"GEN");
			}
			else
				pHandMgr->GetScript<HandStateMgrScript>()->SetNextState(L"GEN");
		}
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
	// Hand01 �� �������� ���� �� �ִ�.
	if (1 != m_iOwnerHandIdx)
		return;

	CScript* pPlayerScript = _OtherObject->GetScript<PlayerScript>();
	CGameObject* pJug = GetOwner()->GetParent()->GetParent();
	CScript* pBossJugScript = pJug->GetScript<BossJugScript>();

	if (nullptr == pPlayerScript)
		return;

	if (nullptr == pBossJugScript)
		return;

}

void BossJugHandScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
void BossJugHandScript::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_vDirection) << YAML::Value << m_vDirection;
	emitter << YAML::Key << NAME_OF(m_vPrevDirection) << YAML::Value << m_vPrevDirection;
	//emitter << YAML::Key << NAME_OF(m_sCurState) << YAML::Value << m_sCurState;
	emitter << YAML::Key << NAME_OF(m_fRunningTime) << YAML::Value << m_fRunningTime;
	emitter << YAML::Key << NAME_OF(m_fSpeed) << YAML::Value << m_fSpeed;
	emitter << YAML::Key << NAME_OF(m_iOwnerHandIdx) << YAML::Value << m_iOwnerHandIdx;
	emitter << YAML::Key << NAME_OF(m_iCurAttackHandIdx) << YAML::Value << m_iOwnerHandIdx;
	emitter << YAML::Key << NAME_OF(m_bAllAttackIsDone) << YAML::Value << m_bAllAttackIsDone;
	emitter << YAML::Key << NAME_OF(m_bGenStateDone) << YAML::Value << m_bGenStateDone;
	emitter << YAML::Key << NAME_OF(m_bAttackStateDone) << YAML::Value << m_bAttackStateDone;
	emitter << YAML::Key << NAME_OF(m_bVanishStateDone) << YAML::Value << m_bVanishStateDone;
	emitter << YAML::Key << NAME_OF(m_bAttackRepeat) << YAML::Value << m_bAttackRepeat;

	CScript::Serialize(emitter);
}

void BossJugHandScript::Deserialize(const YAML::Node& node)
{
	m_vDirection = node[NAME_OF(m_vDirection)].as<Vec3>();
	m_vPrevDirection = node[NAME_OF(m_vPrevDirection)].as<Vec3>();
	//m_sCurState = node[NAME_OF(m_sCurState)].as<wstring>();
	m_fRunningTime = node[NAME_OF(m_fRunningTime)].as<float>();
	m_fSpeed = node[NAME_OF(m_fSpeed)].as<float>();
	m_iOwnerHandIdx = node[NAME_OF(m_iOwnerHandIdx)].as<int>();
	m_iCurAttackHandIdx = node[NAME_OF(m_iCurAttackHandIdx)].as<int>();
	m_bAllAttackIsDone = node[NAME_OF(m_bAllAttackIsDone)].as<bool>();
	m_bGenStateDone = node[NAME_OF(m_bGenStateDone)].as<bool>();
	m_bAttackStateDone = node[NAME_OF(m_bAttackStateDone)].as<bool>();
	m_bVanishStateDone = node[NAME_OF(m_bVanishStateDone)].as<bool>();
	m_bAttackRepeat = node[NAME_OF(m_bAttackRepeat)].as<bool>();

	CScript::Deserialize(node);
}
