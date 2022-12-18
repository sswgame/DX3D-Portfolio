#include "pch.h"
#include "CPlayerDieState.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include "PaperBurnScript.h"


CPlayerDieState::CPlayerDieState()
	: CState(L"DIE")
{
}

CPlayerDieState::CPlayerDieState(const CPlayerDieState& _origin)
	:CState(_origin)

{
}

CPlayerDieState::~CPlayerDieState()
{
}


void CPlayerDieState::Enter()
{
	CState::ResetTimer();
	m_eState = DIE_STATE::DIE_START;
	PlayDieAnim(L"die", false, 0.8f);

}

void CPlayerDieState::Exit()
{

}

void CPlayerDieState::Update()
{
	CState::Update();

	switch (m_eState)
	{
	case DIE_STATE::DIE_START:
	{
		if (m_pCurAnim != nullptr && m_pCurAnim->IsFinish())
		{
			m_eState = DIE_STATE::DIE_END;
		}

	}
	break;
	case DIE_STATE::DIE_END:
	{
		if (m_pPaperBurn)
		{
			m_pPaperBurn->On();
		}

	}
	break;

	}
}

void CPlayerDieState::LateUpdate()
{
	CState::LateUpdate();

	switch (m_eState)
	{
	case DIE_STATE::DIE_START:
	{

	}
	break;
	case DIE_STATE::DIE_END:
	{

	}
	break;

	}

}

void CPlayerDieState::PlayDieAnim(wstring _sName, bool _bRepeat, float _fSpeed)
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
