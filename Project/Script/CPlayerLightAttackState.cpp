#include "pch.h"
#include "CPlayerLightAttackState.h"
#include "PlayerScript.h"
#include "CStateMgr.h"

#include "SwordTrailScript.h"
#include "TrailScript.h"
#include "PaperBurnScript.h"



#include <Engine/CFSM.h>
#include <Engine/Ccamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCollider3D.h>


CPlayerLightAttackState::CPlayerLightAttackState()
	: CState(L"LIGHT_ATTACK") {}

CPlayerLightAttackState::CPlayerLightAttackState(const CPlayerLightAttackState& _origin)
	: CState(_origin) {}

CPlayerLightAttackState::~CPlayerLightAttackState() {}


void CPlayerLightAttackState::SetWeaponCollider(CGameObject* _pObj)
{
	if (_pObj == nullptr)
		return;

	m_pWeaponCollider = _pObj;
}

void CPlayerLightAttackState::SetWeaponTrail(CGameObject* _pObj) {}

void CPlayerLightAttackState::TrailOn()
{
	if (m_pWeaponCollider && m_pWeaponTrail)
	{
		Vec3 vScale       = m_pWeaponCollider->Collider3D()->GetOffsetScale();
		Vec3 vForward     = m_pWeaponCollider->Transform()->GetWorldFrontDir();
		Vec3 vOffsetTrans = vForward * (vScale.z / 2.f);

		Vec3 vWorldPos = m_pWeaponCollider->Transform()->GetWorldPos();
		Vec3 vWorldRot = DecomposeRotMat(m_pWeaponCollider->Transform()->GetWorldRotation());

		SwordTrailScript* pTrail = m_pWeaponTrail->GetScript<SwordTrailScript>();
		pTrail->Reset();
		if (pTrail && !pTrail->isStart())
		{
			pTrail->setPos(vWorldPos + vOffsetTrans);
			pTrail->SetRot(vWorldRot);
			pTrail->SetForward(vForward);
			pTrail->TrailStart();

			m_pWeaponTrail->Transform()->SetRelativePos(vWorldPos + vOffsetTrans);
			switch (m_eLA_State)
			{
			case LIGHT_ATTACK_STATE::COMBO_1:
				m_bSwordTrail_Play_1 = true;
				break;
			case LIGHT_ATTACK_STATE::COMBO_2:
				m_bSwordTrail_Play_2 = true;
				break;
			case LIGHT_ATTACK_STATE::COMBO_3:
				m_bSwordTrail_Play_3 = true;

				break;
			}
		}
		else
		{
			pTrail->setPos(vWorldPos + vOffsetTrans);
			pTrail->SetRot(vWorldRot);
			pTrail->SetForward(vForward);
		}

	}
}

void CPlayerLightAttackState::UpdateWeaponTrail()
{
	if (m_pWeaponTrail && m_pWeaponCollider)
	{
		Vec3 vScale       = m_pWeaponCollider->Collider3D()->GetOffsetScale();
		Vec3 vForward     = m_pWeaponCollider->Transform()->GetWorldFrontDir();
		Vec3 vOffsetTrans = vForward * (vScale.z / 2.f);

		Vec3 vWorldPos = m_pWeaponCollider->Transform()->GetWorldPos();
		Vec3 vWorldRot = DecomposeRotMat(m_pWeaponCollider->Transform()->GetWorldRotation());

		SwordTrailScript* pTrail = m_pWeaponTrail->GetScript<SwordTrailScript>();
		if (pTrail && pTrail->isStart())
		{
			pTrail->setPos(vWorldPos + vOffsetTrans);
			pTrail->SetRot(vWorldRot);
			pTrail->SetForward(vForward);
		}
	}
}

void CPlayerLightAttackState::UpdateWeaponTrailState()
{
	switch (m_eLA_State)
	{
	case LIGHT_ATTACK_STATE::COMBO_1:
		{
			if (m_tAttTimer.fCombo1_Timer >= 0.4f)
			{
				if (!m_bSwordTrail_Play_1)
				{
					TrailOn();
				}
			}
		}
		break;
	case LIGHT_ATTACK_STATE::COMBO_2:
		{
			if (m_tAttTimer.fCombo2_Timer >= 0.25f)
			{
				if (!m_bSwordTrail_Play_2)
				{
					TrailOn();
				}
			}
		}
		break;
	case LIGHT_ATTACK_STATE::COMBO_3:
		{
			if (m_tAttTimer.fCombo3_Timer >= 0.05f)
			{
				if (!m_bSwordTrail_Play_3)
				{
					TrailOn();
				}
			}
		}
		break;
	}
}

void CPlayerLightAttackState::Enter()
{
//#define LIGHT_ATTACK_IGNORE
#ifdef	LIGHT_ATTACK_IGNORE
	GetOwner()->FSM()->ChangeState(L"IDLE");

#endif // !LIGHT_ATTACK_IGNORE

	CState::ResetTimer();
	m_eLA_State = LIGHT_ATTACK_STATE::COMBO_1;
	m_tAttTimer.Clear();

	if (m_pPlayerScript == nullptr)
	{
		m_pPlayerScript = (PlayerScript*)GetOwner()->GetScriptByName(L"PlayerScript");
		m_pStateMgr     = m_pPlayerScript->GetStateMgr();

		// StateMGr 에 현재상태가 변경됨을 알린다. 
		m_pStateMgr = m_pPlayerScript->GetStateMgr();
		m_pStateMgr->ChangeCurStateType(L"LIGHT_ATTACK");
	}

	PlayLightAttackAnim(L"attack_light_1", false, 1.5f);
	m_bComboAtt_Start   = true;
	m_bCombo1_2_Success = false;
	m_bCombo2_3_Success = false;

	if (!m_pWeaponCollider)
	{
		CGameObject* pOwner = GetOwner();
		m_pWeaponCollider   = pOwner->FindChild(L"Sword_Bone_Collider");

	}
	else
	{
		m_pWeaponCollider->Activate();

	}

	if (!m_pWeaponTrail)
	{
		CGameObject* pOwner = GetOwner();
		m_pWeaponTrail      = pOwner->FindChild(L"Sword_Trail");
	}

	m_bSwordTrail_Play_1 = false;
	m_bSwordTrail_Play_2 = false;
	m_bSwordTrail_Play_3 = false;

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

void CPlayerLightAttackState::Exit()
{
	m_tAttTimer.Clear();
	m_bCombo1_2_Success = false;
	m_bCombo2_3_Success = false;


	if(m_pWeaponCollider)
		m_pWeaponCollider->Deactivate();

}

void CPlayerLightAttackState::Update()
{
	CState::Update();

	//// 키 업데이트  
	m_tCurKeyInfo  = m_pStateMgr->GetCurKeyInfo();
	m_tPrevKeyInfo = m_pStateMgr->GetPrevKeyInfo();

	// 약공격 상태 업데이트 
	UpdateLightAttackState();

	// 약공격 애니메이션 업데이트
	UpdateLightAttaclAnim();

	// 무기 잔상 효과 PLAY 타이밍 업데이트 
	UpdateWeaponTrailState();

	// 무기 잔상 효과 업데이트  
	UpdateWeaponTrail();
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
			if (m_tAttTimer.fCombo1_Timer <= 0.75f)
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

			if (m_bCombo1_2_Success && m_tAttTimer.fCombo1_Timer >= 0.75f)
			{
				// 애니메이션 재생 후 콤보3 상태로 변환
				PlayLightAttackAnim(L"attack_light_3", false, 1.5f);
				m_eLA_State = LIGHT_ATTACK_STATE::COMBO_2;
			}

			// 콤보 실패 
			if (!m_bCombo1_2_Success && m_tAttTimer.fCombo1_Timer >= 0.75f)
			{
				m_eLA_State = LIGHT_ATTACK_STATE::COMBO_END;
			}
		}
		break;
	case LIGHT_ATTACK_STATE::COMBO_2:
		{
			m_tAttTimer.fCombo2_Timer += DT;

			// 1.f 초 안에 약 공격키를 또 눌렀다면  
			if (m_tAttTimer.fCombo2_Timer <= 0.75f)
			{
				// 콤보 성공 
				if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LIGHT_ATTACK &&
				    m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
					m_bCombo2_3_Success = true;
			}

			if (m_bCombo2_3_Success && m_tAttTimer.fCombo2_Timer >= 0.75f)
			{
				// 애니메이션 재생 후 콤보3 상태로 변환
				PlayLightAttackAnim(L"attack_light_4", false, 1.f);
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

void CPlayerLightAttackState::UpdateLightAttaclAnim() {}

void CPlayerLightAttackState::PlayLightAttackAnim(wstring _sName, bool _bRepeat, float _fSpeed)
{
	CAnimator3D* pAnimator3D = GetOwner()->Animator3D();
	if (pAnimator3D != nullptr)
	{
		CAnimation3D* pAnim = pAnimator3D->FindAnim(_sName);
		m_pCurAnim          = pAnim;
		if (pAnim != nullptr)
		{
			pAnimator3D->Play(_sName, _bRepeat);
			pAnimator3D->SetSpeed(_fSpeed);
		}
	}
}
