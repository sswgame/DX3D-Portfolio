#include "pch.h"
#include "CEditorCamScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

CEditorCamScript::CEditorCamScript()
	: CScript(-1)
	, m_fSpeed(200.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	float fSpeed = m_fSpeed;

	if (KEY_PRESSED(KEY::LSHFT))
	{
		fSpeed *= 5.f;
	}

	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		if (KEY_PRESSED(KEY::W))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos += DT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos -= DT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos -= DT * vRight * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos += DT * vRight * fSpeed;
		}
	}

	else
	{
		if (KEY_PRESSED(KEY::W))
		{
			vPos.y += DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += DT * fSpeed;
		}
	}


	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * XM_PI;
		vRot.x -= DT * vMouseDir.y * XM_PI;

		Transform()->SetRelativeRotation(vRot);
	}

	Transform()->SetRelativePos(vPos);
}
