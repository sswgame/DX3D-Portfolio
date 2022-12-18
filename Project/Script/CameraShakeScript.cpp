#include "pch.h"
#include "CameraShakeScript.h"


CameraShakeScript::CameraShakeScript()
	: CScript((int)SCRIPT_TYPE::CAMERASHAKESCRIPT)
	, m_bShake(false)
	, m_iShaker(-1)
	, m_fEffectTime(0.f)
	, m_fTimer(0.f)
{
}

CameraShakeScript::~CameraShakeScript()
{
}

void CameraShakeScript::update()
{
	m_fTimer += DT;

	if (m_fTimer >= m_fEffectTime)
	{
		m_fTimer = 0.f;
		Transform()->SetRelativePos(m_vOriginPos);
	}
	else
	{
		// Áøµ¿
		Vec3 vPos = m_vOriginPos;
		vPos.x += m_iShaker * 300.f * DT;
		vPos.y += m_iShaker * 300.f * DT;

		Transform()->SetRelativePos(vPos);
		m_iShaker *= -1;
	}
}

void CameraShakeScript::lateupdate()
{
}

void CameraShakeScript::StartShake(bool _bShake, float _fTime)
{
	m_bShake      = true;
	m_fEffectTime = _fTime;
	m_vOriginPos  = Transform()->GetRelativePos();
}
