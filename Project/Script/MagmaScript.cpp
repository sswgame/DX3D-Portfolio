#include "pch.h"
#include "MagmaScript.h"

// < ENGINE >>
#include <Engine/CMesh.h>
#include <Engine/CCollider3D.h>
#include <Engine/CTransform.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CSerializer.h>
MagmaScript::MagmaScript()
	: CScript{ (int)SCRIPT_TYPE::MAGMASCRIPT }
	, m_fLifeTime(0.f)
	, m_fAddTime(0.f)
	, m_fExplodeScale(0.f)
{
	m_fLifeTime = 6.f;
	m_fExplodeScale = 500.f;
}

MagmaScript::~MagmaScript()
{
}

void MagmaScript::Update()
{
	m_fAddTime += DT;

	if (m_fLifeTime <= m_fAddTime)
	{
		// ===============
		//	magma explode
		// ===============

		//// magma collider
		//CGameObject* pExplodeCollider = new CGameObject;
		//pExplodeCollider->AddComponent(new CTransform);
		//pExplodeCollider->AddComponent(new CCollider3D);
		//pExplodeCollider->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		//pExplodeCollider->Transform()->SetRelativeScale(Vec3(m_fExplodeScale, m_fExplodeScale, m_fExplodeScale));
		//pExplodeCollider->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		//CSceneMgr::GetInst()->SpawnObject(pExplodeCollider, 6);

		//// magma effect
		//CGameObject* pExplodeParticle = new CGameObject;
		//const CParticleSystem* parti = CResMgr::GetInst()->FindRes<CParticleSystem>(L"Magma_Effect").Get();
		//pExplodeParticle->AddComponent(new CParticleSystem(*parti));
		//pExplodeParticle->AddComponent(new CTransform);
		//pExplodeParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		//CSceneMgr::GetInst()->SpawnObject(pExplodeParticle, 1);

		//GetOwner()->Destroy();

	}
	else if (4.f < m_fAddTime && m_fAddTime < 6.f)
	{
		// =====================
		//	magma explode ready
		// =====================
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

void MagmaScript::LateUpdate()
{
}

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
