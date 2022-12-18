#include "pch.h"
#include "EnergyBallScript.h"

#include <Engine/CTransform.h>
#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCollider3D.h>

#include "PlayerScript.h"

EnergyBallScript::EnergyBallScript()
	: CScript((int)SCRIPT_TYPE::ENERGYBALLSCRIPT)
	, m_eCurMode(ENERGYBALL_MODE::NONE)
	, m_eRotDir(ROT_DIR::HORIZONTAL)
	, m_fRadius(60.f)
	, m_fSpeed(5.f)
	, m_fTimeLimit(-1.f)
	, m_bFinish(false)
{
}

EnergyBallScript::EnergyBallScript(const EnergyBallScript& _origin)
	: CScript((int)SCRIPT_TYPE::ENERGYBALLSCRIPT)
	, m_eRotDir(_origin.m_eRotDir)
	, m_eCurMode(_origin.m_eCurMode)
	, m_fRadius(_origin.m_fRadius)
	, m_fSpeed(_origin.m_fSpeed)
	, m_fTimeLimit(_origin.m_fTimeLimit)
	, m_bFinish(false)

{
}

EnergyBallScript::~EnergyBallScript()
{
}

void EnergyBallScript::Explode()
{
	// Explode particle & collider
	Ptr<CPrefab> pPrefab   = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\explosion.pref");
	CGameObject* pParticle = pPrefab->Instantiate();
	pParticle->SetName(L"magma explode effect");
	pParticle->ParticleSystem()->SetLifeTime(5.f);
	pParticle->ParticleSystem()->SetMaterial(L"material\\explosion.mtrl");
	pParticle->Transform()->SetRelativePos(Transform()->GetWorldPos());
	CSceneMgr::GetInst()->SpawnObject(pParticle, 1);

	GetOwner()->Collider3D()->CreateAttackCollider(0.7f, 500.f, GetOwner()->Transform()->GetRelativePos());
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
	{
		m_bFinish = false;
		GetOwner()->Deactivate();
		return;
	}

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

			if (-1.f != m_fTimeLimit)
			{
				timer += DT;

				// 타이머가 다 되면 회전을 멈춘다.
				if (timer > m_fTimeLimit)
				{
					m_bFinish     = true;
					MoveTargetPos = false;
					angle         = 0;

					timer = 0.;
					break;
				}
			}

			// 현재 위치를 타겟 위치로 바꾼다.
			if (!MoveTargetPos)
			{
				GetOwner()->Transform()->SetRelativePos(m_vTargetPos);
				vPos          = m_vTargetPos;
				MoveTargetPos = true;
			}

			// 회전한다
			if (m_eRotDir == ROT_DIR::HORIZONTAL)
			{
				angle += m_fSpeed * DT;
				vPos.x = m_vTargetPos.x + m_fRadius * cos(angle);
				vPos.z = m_vTargetPos.z + m_fRadius * sin(angle);
			}
			else if (m_eRotDir == ROT_DIR::VERTICAL)
			{
				angle += m_fSpeed * DT;
				vPos.x = m_vTargetPos.x + m_fRadius * cos(angle);
				vPos.y = m_vTargetPos.y + m_fRadius * sin(angle);
			}

			GetOwner()->Transform()->SetRelativePos(vPos);
		}
		break;

	case ENERGYBALL_MODE::MISSILE:
		{
			static float timer = 0.f;

			if (-1.f != m_fTimeLimit)
			{
				timer += DT;

				// 타이머가 다 되면 회전을 멈춘다.
				if (timer > m_fTimeLimit)
				{
					m_bFinish = true;
					timer     = 0.f;
					Explode();
					break;
				}
			}
			if (abs(m_vTargetPos.x - vPos.x) < 0.05f
			    && abs(m_vTargetPos.y - vPos.y) < 0.05f
			    && abs(m_vTargetPos.z - vPos.z) < 0.05f)
			{
				Explode();
				m_bFinish = true;
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
	PlayerScript* pPlayerScript = _OtherObject->GetScript<PlayerScript>();
	if (nullptr != pPlayerScript)
		pPlayerScript->Stat_Down(STAT_TYPE::HP, 0.1f);

	Explode();
	m_bFinish = true;
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
