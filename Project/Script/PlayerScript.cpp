#include "pch.h"
#include "PlayerScript.h"

//#include "CState.h" // .. �� ���º� ������� CState.h �� �����߽��ϴ�. 

#include "CStateMgr.h"

#include "CObjKeyMgr.h"

#include "PlayerCamScript.h"

#include <Engine/CCollider3D.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CFSM.h>


#include "SwordTrailScript.h"

PlayerScript::PlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pKeyMgr(nullptr)
	, m_pStateMgr(nullptr)
	, m_pStat(nullptr)
{
	SetName(L"PlayerScript");
}

PlayerScript::PlayerScript(const PlayerScript& _origin)
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pKeyMgr(nullptr)
	, m_pCamera(nullptr)
	, m_pStateMgr(nullptr)
	, m_pStat(nullptr)
{
	SetName(L"PlayerScript");
}


PlayerScript::~PlayerScript()
{
	SAFE_DELETE(m_pKeyMgr);
	SAFE_DELETE(m_pStateMgr);
	SAFE_DELETE(m_pStat);
}

void PlayerScript::start()
{
	// Ű ���� ���� �ʱ�ȭ
	if (nullptr == m_pKeyMgr)
	{
		m_pKeyMgr = new CObjKeyMgr;
		m_pKeyMgr->SetOwnerScript(this);
	}


	// ���� ���� ���� �ʱ�ȭ
	if (nullptr == m_pStateMgr)
	{
		m_pStateMgr = new CStateMgr;
		m_pStateMgr->InitState(GetOwner());
	}


	// ���� ���� ���� �ʱ�ȭ 
	if (nullptr == m_pStat)
	{
		m_pStat = new CPlayerStat;
		m_pStat->SetOwnerScript(this);
		m_pStat->start();
	}


	// ī�޶� �ʱ�ȭ - �⺻������ �ܺο��� �ʱ�ȭ�Ѵ�. 
	if (nullptr == m_pCamera)
	{
		// �ܺο��� �ʱ�ȭ�� ������ٸ�
		// CAMERA ���̾� / MainCamera ��� Obj �� ã�´�. ( �����Ǿ���� )
		CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CAMERA");
		if (pLayer != nullptr)
		{
			vector<CGameObject*> vecObjs = pLayer->GetObjects();
			for (int i = 0; i < vecObjs.size(); ++i)
			{
				if (vecObjs[i]->GetName() == L"MainCamera")
				{
					m_pCamera = vecObjs[i];
					m_pStateMgr->SetCamera(m_pCamera); // ī�޶� ���� 

					break;
				}
			}
		}
	}

	if (m_pStateMgr->GetCamera() == nullptr)
	{
		if (m_pCamera)
			m_pStateMgr->SetCamera(m_pCamera);
	}

	// ���� �浹ü �ʱ�ȭ
	if (m_pSwordCollider == nullptr)
	{
		m_pSwordCollider = GetChildObj(GetOwner(), L"Sword_Bone_Collider");

		if (!m_pSwordCollider)
		{
			m_pSwordCollider = new CGameObject;
			m_pSwordCollider->SetName(L"Sword_Bone_Collider");

			m_pSwordCollider->AddComponent(new CTransform);
			m_pSwordCollider->AddComponent(new CCollider3D);

			m_pSwordCollider->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

			CSceneMgr::GetInst()->GetCurScene()->AddObject(m_pSwordCollider, L"PLAYER");
			GetOwner()->AddChild(m_pSwordCollider);
		}
	}

	// ���� Ʈ���� �ʱ�ȭ  
	if (m_pSword_Trail == nullptr)
	{
		m_pSword_Trail = GetChildObj(GetOwner(), L"Sword_Trail");
		if (m_pSword_Trail)
		{
			m_pSword_Trail->Transform()->SetIgnoreParent(true);		// �θ��� ��� ����� ������� ���� 
			m_pSword_Trail->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
		}
	}

	// �� �ʱ�ȭ
	if (!m_pDirectionalLight)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"BG_OBJ");
		if (pLayer)
			m_pDirectionalLight = pLayer->FindRootObject(L"Directional Light");
	}
}

void PlayerScript::update()
{

	// 1. Ű ���� ������Ʈ 
	m_pKeyMgr->update();
	tKey_Zip tCurKeyInfo  = m_pKeyMgr->GetCurKeyInfo();
	tKey_Zip tPrevKeyInfo = m_pKeyMgr->GetPrevKeyInfo();

	// 2. �÷��̾� ���� ������Ʈ 
	m_pStateMgr->SetCurKeyInfo(tCurKeyInfo);
	m_pStateMgr->SetPrevKeyInfo(tPrevKeyInfo);
	m_pStateMgr->Update();

	// 3. �÷��̾� ���� ������Ʈ 
	m_pStat->update();

	// 4. ī�޶� ���� ������Ʈ 
	UpdateCamera();

	// 5. ���� �浹ü ������Ʈ 
	UpdateSwordCollider();

	// 6. �� ������Ʈ 
	UpdateDirectionalLight();

	// Die ���� üũ 
	if (m_pStat->GetStat().fHp <= 0.f)
	{
		if(m_pStateMgr->GetCurstate() != L"DIE")
			GetOwner()->FSM()->ChangeState(L"DIE");
	}


}

void PlayerScript::lateupdate()
{
	m_pKeyMgr->lateupdate();

	m_pStateMgr->LateUpdate();

	m_pStat->lateupdate();
}

void PlayerScript::UpdateCamera()
{
	if (m_pCamera == nullptr)
		return;

	PlayerCamScript* pCamScript = (PlayerCamScript*)m_pCamera->GetScriptByName(L"PlayerCamScript");
	if (pCamScript)
	{
		// ī�޶� ��ũ��Ʈ�� �÷��̾��� ��ġ ������ �ѱ�� .  
		pCamScript->SetTargetPos(GetOwner()->Transform()->GetRelativePos());
	}
}

void PlayerScript::UpdateSwordCollider()
{
	if (!m_pSwordCollider)
		return;


	CAnimator3D* pAnimator3D = GetOwner()->Animator3D();
	if (pAnimator3D)
	{
		CAnimation3D* pAnim = pAnimator3D->GetCurAnim();
		if (pAnim)
		{
			int    SocketIdx   = 0;
			Matrix SocketMat   = pAnimator3D->GetSocket(SocketIdx);
			Vec3   SocketRot   = DecomposeRotMat(SocketMat);
			Vec3   SocketTrans = Vec3(SocketMat._41, SocketMat._42, SocketMat._43);

			// ȸ�� ���� 
			m_pSwordCollider->Transform()->SetRelativeRotation(SocketRot);

			Vec3 vScale       = m_pSwordCollider->Collider3D()->GetOffsetScale();
			Vec3 vForward     = m_pSwordCollider->Transform()->GetWorldFrontDir();
			Vec3 vOffsetTrans = vForward * (vScale.z / 2.f);

			// ��ġ ���� 
			m_pSwordCollider->Transform()->SetRelativePos(SocketTrans); // ���� ��ġ
			m_pSwordCollider->Collider3D()->SetOffsetPos(vOffsetTrans); // ���Ƿ� ��ġ ����  
		}
	}
}

void PlayerScript::UpdateDirectionalLight()
{

	if (!m_pDirectionalLight)
		return;

	Vec3 vCamPos      = m_pCamera->Transform()->GetRelativePos();
	Vec3 vCamForward  = m_pCamera->Transform()->GetWorldFrontDir();
	Vec3 vCamBackward = -1 * vCamForward;

	Vec3 vLightPos = Vec3(vCamPos.x + vCamBackward.x * 500.f,
	                      vCamPos.y + 500.f,
	                      vCamPos.z + vCamBackward.z * 500.f);

	m_pDirectionalLight->Transform()->SetRelativePos(vLightPos);

}

CGameObject* PlayerScript::GetChildObj(CGameObject* _parent, wstring _name)
{
	vector<CGameObject*> vecChild = _parent->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (vecChild[i]->GetName() == _name)
		{
			return vecChild[i];
		}
		else
		{
			CGameObject* pObj = GetChildObj(vecChild[i], _name);
			if (pObj != nullptr)
				return pObj;
		}
	}

	return nullptr;
}

void PlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	// ���� �ٲ�°� 
	// �и� ���� �ٲٴ°� 
	if (!_OtherObject)
		return;

	// �浹ü ���̾� �ε��� 
	int           iLayerIdx       = _OtherObject->GetLayerIndex();
	const wstring _OtherLayerName = CSceneMgr::GetInst()->GetCurScene()->GetLayerNameFromIdx(iLayerIdx);


	// [ Monster Attack Check ]
	// 1. Non _ Parrying Attack Check
	GAME::LAYER::MONSTER_NON_PARRING_ATTACK;


	// 2. Parrying Attack Check
	GAME::LAYER::MONSTER_PARRING_ATTACK;
	// ���� : �÷��̾ �ٶ󺸴� �������� �̵������� �ٲ۴�.
	Vec3 vChangedDir = GetOwner()->Transform()->GetWorldFrontDir();

}

void PlayerScript::OnCollision(CGameObject* _OtherObject)
{


}

void PlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{


}


void PlayerScript::SaveToScene(FILE* _pFile)
{
}

void PlayerScript::LoadFromScene(FILE* _pFile)
{
}


void PlayerScript::Stat_Up(STAT_TYPE _eType, float _fPercent)
{
	tSTAT MaxStat = m_pStat->GetMaxStat();

	switch (_eType)
	{
	case STAT_TYPE::HP:
		{
			float fHP = MaxStat.fHp * _fPercent;
			m_pStat->AddHp(fHP);
		}
		
		break;
	case STAT_TYPE::STAMINA:
		{
			float fStamina = MaxStat.fStamina * _fPercent;
			m_pStat->AddStamina(fStamina);
		}
		
		break;
	case STAT_TYPE::ETHER:
		{
			float fEther = MaxStat.fEther * _fPercent;
			m_pStat->AddEther(fEther);
		}
		
		break;
	case STAT_TYPE::ATTACK:
		{
			float fAttack = MaxStat.fEther * _fPercent;
			m_pStat->AddAttack(fAttack);
		}
		
		break;
	case STAT_TYPE::ARMOR:
		{
			float fArmor = MaxStat.fArmor * _fPercent;
			m_pStat->AddArmor(fArmor);
		}
		
		break;
	case STAT_TYPE::SPEED:
		{
			float fSpeed = MaxStat.fSpeed * _fPercent;
			m_pStat->AddSpeed(fSpeed);
		}
		
		break;
	}
}

void PlayerScript::Stat_Down(STAT_TYPE _eType, float _fPercent)
{
	tSTAT MaxStat = m_pStat->GetMaxStat();

	switch (_eType)
	{
	case STAT_TYPE::HP:
		{
		if (m_pStateMgr->GetCurstate() == L"SPRINT" ||
			m_pStateMgr->GetCurstate() == L"DIE")
				break;

			float fHP = MaxStat.fHp * _fPercent;
			m_pStat->SubHp(fHP);
			GetOwner()->FSM()->ChangeState(L"HIT");

		}
		
		break;
	case STAT_TYPE::STAMINA:
		{
			float fStamina = MaxStat.fStamina * _fPercent;
			m_pStat->SubStamina(fStamina);
		}
		
		break;
	case STAT_TYPE::ETHER:
		{
			float fEther = MaxStat.fEther * _fPercent;
			m_pStat->SubEther(fEther);
		}
		
		break;
	case STAT_TYPE::ATTACK:
		{
			float fAttack = MaxStat.fEther * _fPercent;
			m_pStat->SubAttack(fAttack);
		}
		
		break;
	case STAT_TYPE::ARMOR:
		{
			float fArmor = MaxStat.fArmor * _fPercent;
			m_pStat->SubArmor(fArmor);
		}
		
		break;
	case STAT_TYPE::SPEED:
		{
			float fSpeed = MaxStat.fSpeed * _fPercent;
			m_pStat->SubSpeed(fSpeed);
		}
		
		break;
	}
}
