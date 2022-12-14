#include "pch.h"
#include "EffectScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CMesh.h>

EffectScript::EffectScript()
	: CScript{ (int)SCRIPT_TYPE::EFFECTSCRIPT }
	, m_pEffectTexture(nullptr)
	, m_fLifeTime(0.f)
	, m_eEffectType(EffectType::NONE)
{
	m_fLifeTime = 2.f;
}

EffectScript::EffectScript(const EffectScript& _origin)
	: CScript{ (int)SCRIPT_TYPE::EFFECTSCRIPT }
	, m_pEffectTexture(_origin.m_pEffectTexture)
	, m_fLifeTime(_origin.m_fLifeTime)
	, m_eEffectType(_origin.m_eEffectType)
{
}

EffectScript::~EffectScript()
{
}

void EffectScript::start()
{
	m_eEffectType = EffectType::BOSS_HANP_DROP_EFFECT;

	if (m_eEffectType == EffectType::BOSS_HANP_DROP_EFFECT)
	{
		GetOwner()->AddComponent(new CMeshRender);
		GetOwner()->AddComponent(new CTransform);

		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		GetOwner()->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));
		GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));


		CMesh* pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
		m_pEffectTexture = CResMgr::GetInst()->Load<CTexture>(L"texture\\particle\\CircleImpact.png"
			, L"texture\\particle\\CircleImpact.png").Get();
		GetOwner()->MeshRender()->SetMesh(pMesh);
		const Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl");
		GetOwner()->MeshRender()->SetSharedMaterial(pMaterial, 0);
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_pEffectTexture);

	}
	else if (m_eEffectType == EffectType::NONE)
	{

	}
}

void EffectScript::update()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRotation = GetOwner()->Transform()->GetRelativeRotation();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

	if (m_eEffectType == EffectType::BOSS_HANP_DROP_EFFECT)
	{
		m_fLifeTime -= DT;

		if (m_fLifeTime <= 0.f)
		{
			GetOwner()->Destroy();
		}
		else
		{
			vScale.x *= 1.001f;
			vScale.y *= 1.001f;
		}
	}
	else if (m_eEffectType == EffectType::NONE)
	{
		return;
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeRotation(vRotation);
	GetOwner()->Transform()->SetRelativeScale(vScale);

}

void EffectScript::lateupdate()
{
}

void EffectScript::Serialize(YAML::Emitter& emitter)
{
}

void EffectScript::Deserialize(const YAML::Node& node)
{
}
