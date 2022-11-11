#include "pch.h"
#include "CPlayerMoveState.h"

#include <Engine/Ccamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator3D.h>


#include "PlayerScript.h"
#include "RigidBodyScript.h"
#include "CStateMgr.h"

#include "CScriptMgr.h"
#include "CPlayerStat.h"




CPlayerMoveState::CPlayerMoveState()
	:CState(L"MOVE")
	, m_pTranslateMgr(nullptr)
	, m_bChangeLerpTime(false)
{

}

CPlayerMoveState::CPlayerMoveState(const CPlayerMoveState& _origin)
	:CState(_origin)

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


	if (m_pTranslateMgr == nullptr)
	{
		CGameObject* pCamera = pPlayerScript->GetCamera();
		m_pTranslateMgr = new CTranslateMgr;
		m_pTranslateMgr->Init(GetOwner(), pCamera, pScript);

	}

	// �ִϸ��̼� ���
	PlayMoveAnim(L"run", true);

	// StateMGr �� ������°� ������� �˸���. 
	m_pStateMgr = pPlayerScript->GetStateMgr();
	m_pStateMgr->ChangeCurStateType(L"MOVE");

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
