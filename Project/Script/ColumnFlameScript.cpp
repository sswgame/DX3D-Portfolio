#include "pch.h"
#include "ColumnFlameScript.h"

//[ ENGINE ]
#include <Engine/CGameObject.h>
#include <Engine/CCollider3D.h>

ColumnFlameScript::ColumnFlameScript()
	: CScript((int)SCRIPT_TYPE::COLUMNFLAMESCRIPT)
{
	AddScriptParam("FLAME SPEED", SCRIPTPARAM_TYPE::FLOAT, &m_fFlameSpeed);
	AddScriptParam("ROTATION SPEED", SCRIPTPARAM_TYPE::FLOAT, &m_fRotSpeed);
	AddScriptParam("COLOR", SCRIPTPARAM_TYPE::VEC4, &m_vColor);
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
		CTexture*  pTex  = CResMgr::GetInst()->Load<CTexture>(L"texture\\boss\\T_SimpleFlames.png",
		                                                      L"texture\\boss\\T_SimpleFlames.png").Get();
		m_pLaser_1->Transform()->SetRelativeScale(60.f, 600.f, 1.f);

		pMtrl->SetTexParam(TEX_PARAM::TEX_0, pTex);

		m_pLaser_1->MeshRender()->SetMesh(pMesh);
		m_pLaser_1->MeshRender()->SetSharedMaterial(pMtrl, 0);
		GetOwner()->AddChild(m_pLaser_1);
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

	CMaterial* pMtrl = m_pLaser_1->MeshRender()->GetMaterial(0).Get();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &yPos);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);

	pMtrl = m_pLaser_2->MeshRender()->GetDynamicMaterial(0).Get();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &yPos);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
}

void ColumnFlameScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript::OnCollisionEnter(_OtherObject);
}

void ColumnFlameScript::OnCollision(CGameObject* _OtherObject)
{
	CScript::OnCollision(_OtherObject);
}

void ColumnFlameScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript::OnCollisionExit(_OtherObject);
}
