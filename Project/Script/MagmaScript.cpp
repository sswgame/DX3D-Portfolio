#include "pch.h"
#include "MagmaScript.h"

// < ENGINE >>
#include <Engine/CCollider3D.h>
#include <Engine/CTransform.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CSerializer.h>
#include <Engine/CRenderComponent.h>

MagmaScript::MagmaScript()
	: CScript{ (int)SCRIPT_TYPE::MAGMASCRIPT }
	, m_fLifeTime(0.f)
	, m_fAddTime(0.f)
	, m_fExplodeScale(0.f)
{
	m_fLifeTime = 6.f;
	m_fExplodeScale = 500.f;
}

MagmaScript::~MagmaScript() {}

void MagmaScript::Update()
{
	m_fAddTime += DT;

	if (m_fLifeTime <= m_fAddTime)
	{
		// ===============
		//	magma explode
		// ===============

		// magma collider
		GetOwner()->Collider3D()->CreateAttackCollider(1.f, m_fExplodeScale, GetOwner()->Transform()->GetRelativePos());


		// explode particle 추가
		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\explode.pref", L"prefab\\explode.pref").Get();
		CGameObject* pParticle = pPrefab->Instantiate();
		pParticle->SetName(L"magma explode effect");
		pParticle->ParticleSystem()->SetLifeTime(5.f);
		pParticle->ParticleSystem()->SetParticlePlayOneTime();
		pParticle->ParticleSystem()->SetMaterial(L"material\\explode.mtrl");
		CSceneMgr::GetInst()->SpawnObject(pParticle, 1);


		GetOwner()->Destroy();
	}
	else if (4.f < m_fAddTime && m_fAddTime < 6.f)
	{
		// =====================
		//	magma explode ready
		// =====================

		// 점점 하얗게 만들어주는 trigger On
		int i = 1;
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &i);

	}
	else if (2.f < m_fAddTime && m_fAddTime <= 4.f)
	{
		// ================
		//	magma scale up
		// ================

		Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

		float fMagmaScale = 0.f;
		fMagmaScale = (2.f - (4.f - m_fAddTime)) / 2.f;

		vScale.y = fMagmaScale;

		GetOwner()->Transform()->SetRelativeScale(vScale);
	}
	else
	{
		// ==================
		//	magma not pop up
		// ==================
	}
}

void MagmaScript::LateUpdate() {}

void MagmaScript::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_fLifeTime) << YAML::Value << m_fLifeTime;
	emitter << YAML::Key << NAME_OF(m_fAddTime) << YAML::Value << m_fAddTime;
	emitter << YAML::Key << NAME_OF(m_fExplodeScale) << YAML::Value << m_fExplodeScale;

	CScript::Serialize(emitter);
}

void MagmaScript::Deserialize(const YAML::Node& node)
{
	m_fLifeTime = node[NAME_OF(m_fLifeTime)].as<float>();
	m_fAddTime = node[NAME_OF(m_fAddTime)].as<float>();
	m_fLifeTime = node[NAME_OF(m_fLifeTime)].as<float>();

	CScript::Deserialize(node);
}
