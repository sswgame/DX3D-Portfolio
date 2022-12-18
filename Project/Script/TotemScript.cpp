#include "pch.h"
#include "TotemScript.h"

#include "CStateMgr.h"
#include "PlayerScript.h"
#include "BossJugCombatMgrScript.h"
#include "BossJugScript.h"

#include <Engine/CCollider3D.h>

TotemScript::TotemScript()
	: CScript{ (int)SCRIPT_TYPE::TOTEMSCRIPT }
	, m_fTotemFullHP(0.f)
	, m_fTotemCurHP(0.f)
	, m_bHitOn(false)
	, m_bPrevHitOn(false)
	, m_fPaperburnTime(2.f)
{
}

TotemScript::~TotemScript()
{
}

void TotemScript::HitEffect()
{
	static float Timer = 0.f;
	static int   iShaker = 1;
	static Vec3  vOriginalPos = Transform()->GetWorldPos();
	int          iTrue = 1;
	int          iFalse = -1;

	Timer += DT;

	if (Timer >= m_fEffectTime)
	{
		Timer = 0.f;
		m_bHit = false;

		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &iFalse);
		Transform()->SetRelativePos(vOriginalPos);

	}
	else
	{
		// 붉은색 효과
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &iTrue);

		// 진동
		Vec3 vPos = vOriginalPos;
		vPos.x += iShaker * 3.f;
		vPos.y += iShaker * 3.f;

		Transform()->SetRelativePos(vPos);
		iShaker *= -1;
	}
}

void TotemScript::start()
{
	m_fTotemFullHP = 300.f;
	m_fTotemCurHP = 300.f;
	
	CTexture* pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\T_JuguArchi05Dark.png").Get();
	int iHitOn = (int)m_bHitOn;

	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &iHitOn);
}

void TotemScript::update()
{
	int iHitOn = (int)m_bHitOn;
	if (m_bPrevHitOn != m_bHitOn)
	{
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &iHitOn);
		m_bPrevHitOn = m_bHitOn;
	}

}

void TotemScript::lateupdate()
{
	if (m_fTotemFullHP <= 0.f)
	{
		m_fPaperburnTime -= DT;

		if (m_fPaperburnTime <= 0.f)
		{
			GetOwner()->Destroy();
			return;
		}

		CMaterial* pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PaperBurnMtrl.mtrl").Get();
		CTexture* pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\T_JuguArchi05Dark.png").Get();

		float burnStrength = (2.f - m_fPaperburnTime) / 2.f;
		int iHitOn = (int)false;
		GetOwner()->MeshRender()->SetSharedMaterial(pMtrl, 0);
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &iHitOn);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &burnStrength);

		GetOwner()->MeshRender()->Collider3D()->SetOffsetScale(Vec3(0, 0, 0));

	}
}

void TotemScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript::OnCollisionEnter(_OtherObject);

	if (_OtherObject->GetName() == L"Sword_Bone_Collider")
	{
		m_bHitOn = true;
		m_fTotemCurHP -= 100.f;

		PlayerScript* pPlayerScript = m_pCombatMgr->GetPlayer()->GetScript<PlayerScript>();
		if (nullptr != pPlayerScript)
		{
			wstring playerState = pPlayerScript->GetStateMgr()->GetCurstate();
			if (L"LIGHT_ATTACK" == playerState
				|| L"HEAVY_ATTACK" == playerState)
			{
				BossJugScript* pJugScript = m_pCombatMgr->GetJug()->GetScript<BossJugScript>();
				pJugScript->GetHit(0.06f);

				m_bHit = true;
			}
		}
	}
	else if (_OtherObject->GetName() == L"player")
	{
		// 플레이어 
		//float playerStat = _OtherObject->GetScript<PlayerScript>()->GetPlayerStat()->GetStat().fSpeed;
		//float playerStat = _OtherObject->GetScript<PlayerScript>()->GetPlayerStat()->GetStat().fSpeed;
	}
}

void TotemScript::OnCollision(CGameObject* _OtherObject)
{
	CScript::OnCollision(_OtherObject);
}

void TotemScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript::OnCollisionExit(_OtherObject);

		if (_OtherObject->GetName() == L"Sword_Bone_Collider")
		{
			m_bHitOn = false;
		}
		else if (_OtherObject->GetName() == L"player")
		{

		}
}

