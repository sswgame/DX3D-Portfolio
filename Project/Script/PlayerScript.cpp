#include "pch.h"
#include "PlayerScript.h"

//#include "CState.h" // .. �� ���º� ������� CState.h �� �����߽��ϴ�. 

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

	// Ű ���� ���� �ʱ�ȭ
	if (nullptr == m_pKeyMgr)
	{
		m_pKeyMgr = new CObjKeyMgr;
		m_pKeyMgr->SetOwnerScript(this);
	}


	// ���� ���� ���� �ʱ�ȭ
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CStateMgr;
		m_pStateMgr->InitState(GetOwner());
	}


	// ���� ���� ���� �ʱ�ȭ 
	if (nullptr == m_pStat)
	{
		m_pStat = new CPlayerStat;
		m_pStat->SetOwnerScript(this);
		m_pStat->start();
	}


	// ī�޶� �ʱ�ȭ - �⺻������ �ܺο��� �ʱ�ȭ�Ѵ�. 
	if (nullptr == m_pCamera)
	{
		// �ܺο��� �ʱ�ȭ�� ������ٸ�
		// Default ���̾� / MainCamera ��� Obj �� ã�´�. ( �����Ǿ���� )
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
	// 0. ī�޶� ���� ������Ʈ 
	UpdateCamera();

	// 1. Ű ���� ������Ʈ 
	m_pKeyMgr->update();
	tKey_Zip tCurKeyInfo = m_pKeyMgr->GetCurKeyInfo();
	tKey_Zip tPrevKeyInfo = m_pKeyMgr->GetPrevKeyInfo();

	// 2. �÷��̾� ���� ������Ʈ 
	m_pStateMgr->SetCurKeyInfo(tCurKeyInfo);
	m_pStateMgr->SetPrevKeyInfo(tPrevKeyInfo);
	m_pStateMgr->Update();

	// 3. �÷��̾� ���� ������Ʈ 
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
		// ī�޶� ��ũ��Ʈ�� �÷��̾��� ��ġ ������ �ѱ�� .  
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
