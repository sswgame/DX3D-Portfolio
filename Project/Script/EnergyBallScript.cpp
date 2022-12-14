#include "pch.h"
#include "EnergyBallScript.h"

#include <Engine/CTransform.h>

EnergyBallScript::EnergyBallScript()
	: CScript((int)SCRIPT_TYPE::ENERGYBALLSCRIPT)
	, m_eCurMode(ENERGYBALL_MODE::NONE)
	, m_fRadius(60.f)
	, m_fSpeed(5.f)
	, m_fRotTime(0)
	, m_bFinish(false)
{
}

EnergyBallScript::EnergyBallScript(const EnergyBallScript& _origin)
	: CScript((int)SCRIPT_TYPE::ENERGYBALLSCRIPT)
	, m_eCurMode(_origin.m_eCurMode)
	, m_fRadius(_origin.m_fRadius)
	, m_fSpeed(_origin.m_fSpeed)
	, m_fRotTime(_origin.m_fRotTime)
	, m_bFinish(false)
	
{
}

EnergyBallScript::~EnergyBallScript()
{
}

void EnergyBallScript::SetTargetPos(Vec3 _vPos)
{
	Vec3 vPos    = GetOwner()->Transform()->GetRelativePos();
	m_vTargetPos = _vPos;
	m_vDir       = m_vTargetPos - vPos;
	m_vDir.Normalize();
}

void EnergyBallScript::start()
{
}

void EnergyBallScript::update()
{
	if (m_bFinish)
		return;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	switch (m_eCurMode)
	{
	case ENERGYBALL_MODE::NONE:
		{
		}
		break;

	case ENERGYBALL_MODE::ROTATION:
		{
			static bool  MoveTargetPos = false;
			static float angle         = 0;
			static float timer         = 0;


			timer += DT;

			// 타이머가 다 되면 회전을 멈춘다.
			if (timer > m_fRotTime)
			{
				m_bFinish  = true;
				m_eCurMode = ENERGYBALL_MODE::NONE;
			}

			// 현재 위치를 타겟 위치로 바꾼다.
			if (!MoveTargetPos)
			{
				GetOwner()->Transform()->SetRelativePos(m_vTargetPos);
				MoveTargetPos = true;
			}

			// 회전한다
			angle += m_fSpeed * DT;
			vPos.x = m_fRadius * cos(angle);
			vPos.z = m_fRadius * sin(angle);

			GetOwner()->Transform()->SetRelativePos(vPos);
			
		}
		break;

	case ENERGYBALL_MODE::MISSILE:
		{
			if (abs(m_vTargetPos.x - vPos.x < 0.01f)
			    && abs(m_vTargetPos.y - vPos.y < 0.01f)
			    && abs(m_vTargetPos.z - vPos.z < 0.01f))
			{
				// TODO:: 여기에 폭발 함수 추가하심 됩니다.

				m_bFinish  = true;
				m_eCurMode = ENERGYBALL_MODE::NONE;
			}
			else
			{
				vPos += m_vDir * DT * m_fSpeed;
				GetOwner()->Transform()->SetRelativePos(vPos);
			}
		}
		break;
	}
}

void EnergyBallScript::lateupdate()
{
	CScript::lateupdate();
}

void EnergyBallScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript::OnCollisionEnter(_OtherObject);
}

void EnergyBallScript::OnCollision(CGameObject* _OtherObject)
{
	CScript::OnCollision(_OtherObject);
}

void EnergyBallScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript::OnCollisionExit(_OtherObject);
}

void EnergyBallScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void EnergyBallScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}
