#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"

#include <Engine/CFSM.h>

CPlayerScript::CPlayerScript()
	:
	CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
  , m_pMissilePrefab(nullptr)
  , m_fSpeed(10.f)
  , m_fBurnStrength(0.f)
  , m_bBurn(false)
{
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
}

CPlayerScript::~CPlayerScript() {}

void CPlayerScript::start()
{
	m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
}

void CPlayerScript::update()
{
	static bool first = false;
	if (false == first)
	{
		first = true;
		FSM()->CreateState(L"FIRST",
		                   BIND_FN(&CPlayerScript::UpdateFunc),
		                   BIND_FN(&CPlayerScript::StartFunc),
		                   BIND_FN(&CPlayerScript::EndFunc));
		FSM()->CreateState(L"SECOND",
		                   BIND_FN(&CPlayerScript::Test));
		FSM()->ChangeState(L"FIRST");
	}
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_fSpeed;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_fSpeed;

	if (KEY_PRESSED(KEY::UP))
		vPos.y += DT * m_fSpeed;

	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * m_fSpeed;

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_2PI;
		Transform()->SetRelativeRotation(vRot);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		//GetOqwner()->Destroy();
		//GetOwner()->GetChild().at(0)->Destroy();

		//GetOwner()->GetChild().at(0)->Destroy();
		//GetOwner()->Destroy();

		//GetOwner()->GetChild().at(0)->Destroy();
		//GetOwner()->Destroy();

		if (nullptr != m_pMissilePrefab)
		{
			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();

			Vec3 vMissilePos = Transform()->GetRelativePos();
			vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
		}
	}

	if (KEY_TAP(KEY::B))
	{
		m_bBurn              = true;
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		Vec4           vColor(1.f, 0.75f, 0.5f, 0.f);
		pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	}

	Burnning();
}

void CPlayerScript::lateupdate() {}


void CPlayerScript::Burnning()
{
	if (!m_bBurn)
		return;

	m_fBurnStrength += DT * (1.f / 3.f);

	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fBurnStrength);
}


void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Missile")
	{
		_OtherObject->Destroy();
	}
}

void CPlayerScript::UpdateFunc(const tStateInfo& info)
{
	if (KEY_TAP(KEY::C))
	{
		MessageBox(nullptr, L"매 프레임마다 실행되는 함수입니다. 지금은 바로 상태를 전환해서 1번만 실행합니다", L"FIRST UPDATE",MB_OK);
		FSM()->ChangeState(L"SECOND");
	}
}

void CPlayerScript::StartFunc(const tStateInfo& info)
{
	MessageBox(nullptr, L"제일 저음 상태가 시작될 때 실행되는 함수입니다", L"FIRST START",MB_OK);
}

void CPlayerScript::EndFunc(const tStateInfo& info)
{
	MessageBox(nullptr, L"상태 종료 전에 실행되는 함수입니다", L"FIRST END",MB_OK);
}

void CPlayerScript::Test(const tStateInfo& info)
{
	static float temp{};
	temp += DT;
	if (temp > 3.f)
	{
		temp = 0.f;
		MessageBox(nullptr, L"3초마다 실행합니다", L"SECOND UPDATE",MB_OK);
	}
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
}
