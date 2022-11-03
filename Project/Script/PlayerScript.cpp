#include "pch.h"
#include "PlayerScript.h"

//#include "CState.h" // .. 각 상태별 헤더들은 CState.h 에 정의했습니다. 

#include "CStateMgr.h"
#include "CPlayerStat.h"
#include "CObjKeyMgr.h"

#include "PlayerCamScript.h"


PlayerScript::PlayerScript() :
	CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pKeyMgr(nullptr)
	, m_pStateMgr(nullptr)
	, m_pStat(nullptr)
{
	SetName(L"PlayerScript");

}

PlayerScript::~PlayerScript() 
{
	SAFE_DELETE(m_pKeyMgr);
	SAFE_DELETE(m_pStateMgr);
	SAFE_DELETE(m_pStat);
}

void PlayerScript::start()
{

	// 키 정보 관리 초기화
	if (nullptr == m_pKeyMgr)
	{
		m_pKeyMgr = new CObjKeyMgr;
		m_pKeyMgr->SetOwnerScript(this);
	}


	// 상태 정보 관리 초기화
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CStateMgr;
		m_pStateMgr->InitState(GetOwner());
	}


	// 스탯 정보 관리 초기화 
	if (nullptr == m_pStat)
	{
		m_pStat = new CPlayerStat;
		m_pStat->SetOwnerScript(this);
		m_pStat->start();
	}


	// 카메라 초기화 - 기본적으로 외부에서 초기화한다. 
	if (nullptr == m_pCamera)
	{
		// 외부에서 초기화를 안해줬다면
		// Default 레이어 / MainCamera 라는 Obj 를 찾는다. ( 지정되어야함 )
		CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Default");
		if (pLayer != nullptr) 
		{
			vector<CGameObject*> vecObjs = pLayer->GetObjects();
			for (int i = 0; i < vecObjs.size(); ++i)
			{
				if (vecObjs[i]->GetName() == L"MainCamera")
				{
					m_pCamera = vecObjs[i];
					break;
				}
			}
		}
	
	}

}

void PlayerScript::update()
{	
	// 0. 카메라 상태 업데이트 
	UpdateCamera();

	// 1. 키 상태 업데이트 
	m_pKeyMgr->update();
	tKey_Zip tCurKeyInfo = m_pKeyMgr->GetCurKeyInfo();
	tKey_Zip tPrevKeyInfo = m_pKeyMgr->GetPrevKeyInfo();

	// 2. 플레이어 상태 업데이트 
	m_pStateMgr->SetCurKeyInfo(tCurKeyInfo);
	m_pStateMgr->SetPrevKeyInfo(tPrevKeyInfo);
	m_pStateMgr->Update();

	// 3. 플레이어 스탯 업데이트 
	m_pStat->update();
}

void PlayerScript::lateupdate() 
{

	m_pKeyMgr->lateupdate();

	m_pStateMgr->LateUpdate();

	m_pStat->lateupdate();

}

void PlayerScript::UpdateCamera()
{
	if (m_pCamera == nullptr)
		return;

	static PlayerCamScript* pCamScript = (PlayerCamScript*)m_pCamera->GetScriptByName(L"PlayerCamScript");
	if (pCamScript != nullptr)
	{
		// 카메라 스크립트에 플레이어의 위치 정보를 넘긴다 .  
		pCamScript->SetTargetPos(GetOwner()->Transform()->GetRelativePos());

	}
}

void PlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void PlayerScript::OnCollision(CGameObject* _OtherObject)
{
}

void PlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{
}


void PlayerScript::SaveToScene(FILE* _pFile)
{
}

void PlayerScript::LoadFromScene(FILE* _pFile)
{
}
