#include "pch.h"
#include "CPlayerMoveState.h"

#include <Engine/Ccamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator3D.h>
#include "TrailScript.h"


#include "PlayerScript.h"
#include "CStateMgr.h"

#include "CScriptMgr.h"
#include "CPlayerStat.h"
#include "CObjKeyMgr.h"

#include "PlayerCamScript.h"
#include "PaperBurnScript.h"




CPlayerMoveState::CPlayerMoveState()
	:CState(L"MOVE")
	, m_pTranslateMgr(nullptr)
	, m_pCamScript(nullptr)
	, m_bChangeLerpTime(false)
{

}

CPlayerMoveState::CPlayerMoveState(const CPlayerMoveState& _origin)
	:CState(_origin)
	, m_pTranslateMgr(nullptr)
	, m_pCamScript(nullptr)
	, m_bChangeLerpTime(false)
{
}

CPlayerMoveState::~CPlayerMoveState()
{
	SAFE_DELETE(m_pTranslateMgr);

}


void CPlayerMoveState::Enter()
{
	CState::ResetTimer();

	CScript* pScript = CState::GetOwner()->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
	PlayerScript* pPlayerScript = (PlayerScript*)pScript;
	m_bChangeLerpTime = false;

	pPlayerScript->GetSwordBoneCollider()->Deactivate();

	if (m_pTranslateMgr == nullptr)
	{
		CGameObject* pCamera = pPlayerScript->GetCamera();
		m_pTranslateMgr = new CTranslateMgr;
		m_pTranslateMgr->Init(GetOwner(), pCamera, pScript);

	}

	m_eMode = MOVE_MODE::RUN;

	// �ִϸ��̼� ���
	PlayMoveAnim(L"run", true);

	// StateMGr �� ������°� ������� �˸���. 
	m_pStateMgr = pPlayerScript->GetStateMgr();
	m_pStateMgr->ChangeCurStateType(L"MOVE");


	TrailScript* pTrail = (TrailScript*)GetOwner()->GetScriptByName(L"TrailScript");
	if (pTrail)
	{
		pTrail->Off();
	}

	PaperBurnScript* pBurnScript = GetOwner()->GetScript<PaperBurnScript>();
	if (pBurnScript)
	{
		pBurnScript->Off();
		pBurnScript->SetDir(1);

	}


}

void CPlayerMoveState::Exit()
{
	// �ִϸ��̼��� ���� �ð��� ���� �����ð����� �ǵ�����.
	if (m_bChangeLerpTime == true)
	{
		CAnimation3D* pAnim = GetOwner()->Animator3D()->GetCurAnim();
		pAnim->SetLerpTime(m_fStandardLerpTime);
		m_bChangeLerpTime = false;
	}
}



void CPlayerMoveState::Update()
{
	CState::Update();

	// MOVE_MODE ������Ʈ 
	UpdateMoveMode();

	switch (m_eMode)
	{
	case MOVE_MODE::RUN:
	{
		UpdateRunMode();
	}
	break;
	case MOVE_MODE::WALK:
	{
		UpdateWalkMode();

	}
	break;
	}


	// �̵� ������Ʈ 
	m_pTranslateMgr->Update();

	// �ִϸ��̼� ���� �ð� ���� 
	UpdateRunAnimLerpTime();
}

void CPlayerMoveState::LateUpdate()
{
	CState::LateUpdate();

	m_pTranslateMgr->LateUpdate();

}

void CPlayerMoveState::UpdateMoveMode()
{
	if (m_pCamScript)
	{
		CAMERA_OPTION eCamOption = m_pCamScript->GetCameraOption();
		switch (eCamOption)
		{
		case CAMERA_OPTION::DEFAULT:
			m_eMode = MOVE_MODE::RUN;
			m_pTranslateMgr->SetEqualize_Player_and_Camera(false);
			m_pStat->SetSpeed(150.f);


			break;
		case CAMERA_OPTION::CAMERA_LOCK:
			m_eMode = MOVE_MODE::WALK;
			m_pTranslateMgr->SetEqualize_Player_and_Camera(true);
			m_pStat->SetSpeed(100.f);

			break;
		}
	}
	else
	{
		PlayerScript* pPlayer = (PlayerScript*)CState::GetOwner()->GetScriptByName(L"PlayerScript");
		m_pStat = pPlayer->GetPlayerStat();
		if (pPlayer)
		{

			CGameObject* pCam = pPlayer->GetCamera();
			m_pCamScript = (PlayerCamScript*)pCam->GetScriptByName(L"PlayerCamScript");

		}
	}

}

void CPlayerMoveState::UpdateRunMode()
{
	CAnimation3D* pCurAnim = CState::GetOwner()->Animator3D()->GetCurAnim();
	if (pCurAnim)
	{
		if (pCurAnim->GetName() != L"run")
		{
			// �ִϸ��̼� ���
			PlayMoveAnim(L"run", true);
		}
	}
}

void CPlayerMoveState::UpdateWalkMode()
{
	tKey_Zip tCurKeyInfo = m_pStateMgr->GetCurKeyInfo();
	CAnimation3D* pCurAnim = CState::GetOwner()->Animator3D()->GetCurAnim();
	wstring sAnimName = L"";

	// �ϳ��� ������ �� 
	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD)
	{
		sAnimName = L"walk_forward";
	}

	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD)
	{
		sAnimName = L"walk_back";
	}

	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT)
	{
		sAnimName = L"walk_right";
	}

	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT)
	{
		sAnimName = L"walk_left";
	}

	// ���ÿ� ������ ��  
	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT &&
		tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT)
		sAnimName = L"walk_forward";

	if (tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD &&
		tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD)
		sAnimName = L"walk_forward";



	// �ִϸ��̼� ������Ʈ 
	if (pCurAnim->GetName() != sAnimName)
		PlayMoveAnim(sAnimName, true);

}




void CPlayerMoveState::SetCurKeyInfo(tKey_Zip _keyInfo)
{
	m_tCurKeyInfo = _keyInfo;
}

void CPlayerMoveState::PlayMoveAnim(wstring _sName, bool _bRepeat)
{
	CAnimator3D* pAnimator3D = GetOwner()->Animator3D();
	if (pAnimator3D != nullptr)
	{
		if (pAnimator3D->FindAnim(_sName) != nullptr)
			pAnimator3D->Play(_sName, _bRepeat);
	}
}

void CPlayerMoveState::UpdateRunAnimLerpTime()
{
	// "Run" Animation�� �������� ����� �� �����ð��� [����]�Ѵ�.  
	if (m_bChangeLerpTime == true)
		return;

	CAnimator3D* pAnimator3D = GetOwner()->Animator3D();
	if (pAnimator3D != nullptr)
	{
		CAnimation3D* pAnim = pAnimator3D->GetCurAnim();
		// �ش� �ִϸ��̼��� 2 ��° �÷��� �� �� 
		if (pAnim != nullptr && pAnim->GetPlayCnt() == 2)
		{
			// ���� �����ð� ���� 
			m_bChangeLerpTime = true;
			m_fStandardLerpTime = pAnim->GetLerpTime();

			// ���� �ð� ���� 
			float fChangeLerpTime = 0.01f;
			pAnimator3D->SetLerpTimeOnAnim(pAnim->GetName(), fChangeLerpTime);
		}

	}
}
