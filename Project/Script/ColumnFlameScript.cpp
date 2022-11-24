#include "pch.h"
#include "ColumnFlameScript.h"

//[ ENGINE ]
#include <Engine/CGameObject.h>
#include <Engine/CCollider3D.h>
#include <Engine/CDecal.h>


ColumnFlameScript::ColumnFlameScript()
	: CScript((int)SCRIPT_TYPE::COLUMNFLAMESCRIPT)
	, m_fDamage(10.f)
{
	AddScriptParam("FLAME SPEED", SCRIPTPARAM_TYPE::FLOAT, &m_fFlameSpeed);
	AddScriptParam("ROTATION SPEED", SCRIPTPARAM_TYPE::FLOAT, &m_fRotSpeed);
	AddScriptParam("COLOR", SCRIPTPARAM_TYPE::VEC4, &m_vColor);
	AddScriptParam("DAMAGE", SCRIPTPARAM_TYPE::FLOAT, &m_fDamage);
}

ColumnFlameScript::ColumnFlameScript(const ColumnFlameScript& _origin)
	: CScript((int)SCRIPT_TYPE::COLUMNFLAMESCRIPT)
{
}

ColumnFlameScript::~ColumnFlameScript()
{
}

void ColumnFlameScript::start()
{
	if (nullptr == m_pDecal)
	{
		m_pDecal = new CGameObject;
		m_pDecal->SetName(L"Decal");
		m_pDecal->AddComponent(new CTransform);
		m_pDecal->AddComponent(new CDecal);

		CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\boss\\T_FissureGradient.png",
		                                                    L"texture\\boss\\T_FissureGradient.png").Get();
		m_pDecal->Decal()->SetDecalType(DECAL_TYPE::CUBE);
		m_pDecal->Decal()->SetDecalTexture(pTex);
		m_pDecal->Decal()->SetDeferredLighting(false);

		m_pDecal->Transform()->SetRelativePos(0.f, -285.f, 0.f);
		m_pDecal->Transform()->SetRelativeScale(110.f, 10.f, 110.f);


		GetOwner()->AddChild(m_pDecal);
	}

	if (nullptr == m_pLaser_1)
	{
		///* TEST
		m_pLaser_1 = new CGameObject;
		m_pLaser_1->SetName(L"Attack0_Laser1");
		m_pLaser_1->AddComponent(new CTransform);
		m_pLaser_1->AddComponent(new CMeshRender);

		CMesh*     pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
		CMaterial* pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ColumnFlameMtrl.mtrl").Get();
		CTexture*  pTex  = CResMgr::GetInst()->Load<CTexture>(L"texture\\boss\\flame_2.png",
		                                                      L"texture\\boss\\flame_2.png").Get();
		m_pLaser_1->Transform()->SetRelativeScale(60.f, 600.f, 1.f);

		pMtrl->SetTexParam(TEX_PARAM::TEX_0, pTex);

		m_pLaser_1->MeshRender()->SetMesh(pMesh);
		m_pLaser_1->MeshRender()->SetSharedMaterial(pMtrl, 0);
		GetOwner()->AddChild(m_pLaser_1);

		if (nullptr == m_pDistortion_1)
		{
			m_pDistortion_1 = new CGameObject;
			m_pDistortion_1->SetName(L"Distortion");
			m_pDistortion_1->AddComponent(new CTransform);
			m_pDistortion_1->AddComponent(new CMeshRender);

			CMesh*     pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
			CMaterial* pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PostProcessMtrl.mtrl").Get();
			pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);

			m_pDistortion_1->MeshRender()->SetMesh(pMesh);
			m_pDistortion_1->MeshRender()->SetSharedMaterial(pMtrl, 0);
			m_pLaser_1->AddChild(m_pDistortion_1);
		}
	}

	if (nullptr == m_pLaser_2)
	{
		///* TEST
		m_pLaser_2 = m_pLaser_1->Clone();
		m_pLaser_2->SetName(L"Attack0_Laser2");

		Vec3 rot = m_pLaser_2->Transform()->GetRelativeRotation();
		rot.ToDegree();
		rot.y += 90;
		rot.ToRadian();
		m_pLaser_2->Transform()->SetRelativeRotation(rot);

		GetOwner()->AddChild(m_pLaser_2);
	}

	if (nullptr == GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D))
	{
		GetOwner()->AddComponent(new CCollider3D);

		GetOwner()->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(60.f, 600.f, 60.f));
	}
}

void ColumnFlameScript::update()
{
	Vec3 rot = m_pLaser_1->Transform()->GetRelativeRotation();
	rot.ToDegree();
	rot.y += m_fRotSpeed * DT;
	rot.ToRadian();
	m_pLaser_1->Transform()->SetRelativeRotation(rot);

	rot = m_pLaser_2->Transform()->GetRelativeRotation();
	rot.ToDegree();
	rot.y += m_fRotSpeed * DT;
	rot.ToRadian();
	m_pLaser_2->Transform()->SetRelativeRotation(rot);
}

void ColumnFlameScript::lateupdate()
{
	static float yPos = 0;
	yPos += m_fFlameSpeed * DT;

	m_pDecal->Decal()->SetColor(m_vColor);

	CMaterial* pMtrl = m_pLaser_1->MeshRender()->GetMaterial(0).Get();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &yPos);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);

	pMtrl = m_pLaser_2->MeshRender()->GetDynamicMaterial(0).Get();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &yPos);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));

	pMtrl = m_pDistortion_1->MeshRender()->GetMaterial(0).Get();
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex"));
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &yPos);
}

void ColumnFlameScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void ColumnFlameScript::OnCollision(CGameObject* _OtherObject)
{
	CScript::OnCollision(_OtherObject);
}

void ColumnFlameScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript::OnCollisionExit(_OtherObject);
}

void ColumnFlameScript::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_fFlameSpeed) << YAML::Value << m_fFlameSpeed;
	emitter << YAML::Key << NAME_OF(m_fRotSpeed) << YAML::Value << m_fRotSpeed;
	emitter << YAML::Key << NAME_OF(m_vColor) << YAML::Value << m_vColor;
	//emitter << YAML::Key << NAME_OF(m_fDamage) << YAML::Value << m_fDamage;
}

void ColumnFlameScript::Deserialize(const YAML::Node& node)
{
	m_fFlameSpeed = node[NAME_OF(m_fFlameSpeed)].as<float>();
	m_fRotSpeed   = node[NAME_OF(m_fRotSpeed)].as<float>();
	m_vColor      = node[NAME_OF(m_vColor)].as<Vec4>();
	//m_fDamage     = node[NAME_OF(m_fDamage)].as<float>();
}
