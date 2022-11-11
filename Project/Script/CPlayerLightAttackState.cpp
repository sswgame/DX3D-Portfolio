#include "pch.h"
#include "CPlayerLightAttackState.h"
#include "PlayerScript.h"
#include "CStateMgr.h"


#include <Engine/CFSM.h>
#include <Engine/Ccamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator3D.h>


CPlayerLightAttackState::CPlayerLightAttackState()
	: CState(L"LIGHT_ATTACK")
{
}

CPlayerLightAttackState::CPlayerLightAttackState(const CPlayerLightAttackState& _origin)
	:CState(_origin)

{
}

CPlayerLightAttackState::~CPlayerLightAttackState()
{
}


void CPlayerLightAttackState::Enter()
{
	CState::ResetTimer();
	m_eLA_State = LIGHT_ATTACK_STATE::COMBO_1;
	m_tAttTimer.Clear();

	if (m_pPlayerScript == nullptr)
	{
		m_pPlayerScript = (PlayerScript*)GetOwner()->GetScriptByName(L"PlayerScript");
		m_pStateMgr = m_pPlayerScript->GetStateMgr();

		// StateMGr 에 현재상태가 변경됨을 알린다. 
		m_pStateMgr = m_pPlayerScript->GetStateMgr();
		m_pStateMgr->ChangeCurStateType(L"LIGHT_ATTACK");

	}

	PlayLightAttackAnim(L"attack_light_1", false);
	m_bComboAtt_Start = true;
	m_bCombo1_2_Success = false;
	m_bCombo2_3_Success = false;



}

void CPlayerLightAttackState::Exit()
{
	m_tAttTimer.Clear();
	m_bCombo1_2_Success = false;
	m_bCombo2_3_Success = false;
}

void CPlayerLightAttackState::Update()
{
	CState::Update();

	// 키 업데이트  
	m_tCurKeyInfo = m_pStateMgr->GetCurKeyInfo();
	m_tPrevKeyInfo = m_pStateMgr->GetPrevKeyInfo();

	// 약공격 상태 업데이트 
	UpdateLightAttackState();

	// 약공격 애니메이션 업데이트
	UpdateLightAttaclAnim();



}

void CPlayerLightAttackState::LateUpdate()
{
	CState::LateUpdate();


}

void CPlayerLightAttackState::UpdateLightAttackState()
{
	switch (m_eLA_State)
	{
	case LIGHT_ATTACK_STATE::COMBO_1:
	{
		m_tAttTimer.fCombo1_Timer += DT;

		// 1.f 초 안에 약 공격키를 또 눌렀다면  
		if (m_tAttTimer.fCombo1_Timer <= 1.f)
		{
			if (m_bComboAtt_Start)
				m_bComboAtt_Start = false;
			else
			{
				// 콤보 성공 
				if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LIGHT_ATTACK &&
					m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
					m_bCombo1_2_Success = true;
			}

		}

		if (m_bCombo1_2_Success && m_tAttTimer.fCombo1_Timer >= 1.f)
		{
			// 애니메이션 재생 후 콤보3 상태로 변환
			PlayLightAttackAnim(L"attack_light_3", false);
			m_eLA_State = LIGHT_ATTACK_STATE::COMBO_2;
		}

		// 콤보 실패 
		if (!m_bCombo1_2_Success && m_tAttTimer.fCombo1_Timer >= 1.f)
		{
			m_eLA_State = LIGHT_ATTACK_STATE::COMBO_END;

		}

	}
	break;
	case LIGHT_ATTACK_STATE::COMBO_2:
	{
		m_tAttTimer.fCombo2_Timer += DT;

		// 1.f 초 안에 약 공격키를 또 눌렀다면  
		if (m_tAttTimer.fCombo2_Timer <= 1.f)
		{
			// 콤보 성공 
			if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LIGHT_ATTACK &&
				m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
				m_bCombo2_3_Success = true;
		}

		if (m_bCombo2_3_Success && m_tAttTimer.fCombo2_Timer >= 1.f)
		{
			// 애니메이션 재생 후 콤보3 상태로 변환
			PlayLightAttackAnim(L"attack_light_4", false);
			m_eLA_State = LIGHT_ATTACK_STATE::COMBO_3;
		}

		// 콤보 실패 
		if (!m_bCombo2_3_Success && m_tAttTimer.fCombo2_Timer >= 1.f)
		{
			m_eLA_State = LIGHT_ATTACK_STATE::COMBO_END;

		}
	}
	break;
	case LIGHT_ATTACK_STATE::COMBO_3:
	{
		m_tAttTimer.fCombo3_Timer += DT;

		// 약공격 콤보 전부 끝 -> IDLE 상태로 돌아감 
		if (m_pCurAnim != nullptr && m_pCurAnim->IsFinish() == true)
		{
			GetOwner()->FSM()->ChangeState(L"IDLE");
		}
	}
	break;
	case LIGHT_ATTACK_STATE::COMBO_END:
	{
		// 콤보 끝 -> IDLE 상태로 돌아감 
		if (m_pCurAnim != nullptr && m_pCurAnim->IsFinish() == true)
		{
			GetOwner()->FSM()->ChangeState(L"IDLE");
		}
	}
	break;

	}
}

void CPlayerLightAttackState::UpdateLightAttaclAnim()
{
}

void CPlayerLightAttackState::PlayLightAttackAnim(wstring _sName, bool _bRepeat, float _fSpeed)
{
	CAnimator3D* pAnimator3D = GetOwner()->Animator3D();
	if (pAnimator3D != nullptr)
	{
		CAnimation3D* pAnim = pAnimator3D->FindAnim(_sName);
		m_pCurAnim = pAnim;
		if (pAnim != nullptr)
		{
			pAnimator3D->Play(_sName, _bRepeat);
			pAnimator3D->SetSpeed(_fSpeed);
		}
	}
}
