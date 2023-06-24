#include "pch.h"
#include "CTranslateMgr.h"

#include <Engine/Ccamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CRigidBody.h>


#include "PlayerScript.h"
#include "CStateMgr.h"

#include "CScriptMgr.h"
#include "CPlayerStat.h"


/*
			��������..

			1. ȸ���ؾ��� ���� ũ�⸦ �����Ѵ�.
				- ���� ī�޶� �ٶ󺸴� ���� �� �� ĳ���Ͱ� �ٶ󺸴� ���� ��
				- ���� ������ �����Ѵ�.
				- �̶� ī�޶� �ٶ󺸴� ���� ���� ������ �Ǳ⶧����
				- ī�޶� �������� �ٲ𶧸��� ĳ���Ϳ��� ������ �޶�����.
				( ĳ�������� ������ �ִµ� ī�޶� ���� �����̸� ( �ݴ뵵 �������� )
				  ī�޶��� ���ؿ� ���� �ش� ������ ��������� �ٲ�ٴ� �� )

			2. ĳ���͸� ȸ���Ѵ�.
				- RotatePlayerFront(Vec3 Axis)
				- ĳ���Ͱ� �ٶ󺸴� ���� RotatePlayerFront �Ű�����(Axis)�� �� �� �������� ȸ���Ѵ�.
				- �̶� ������ ���� m_fMoveAccRot( ���ƾ��� ���� ���� )��
				  �� ���� ũ�⸦ �̿��� �ڿ������� ȸ���ϵ��� �Ѵ�.
*/



CTranslateMgr::CTranslateMgr()
	: m_fDefaultAngle(180.f)
	, m_vObjForwardAxis(Vec3(0.f, 0.f, 1.f))
	, m_fRotSpeed(7.f)
	, m_fMoveAccRot(0.f)
	, m_fRotRadian(0.f)
	, m_fRotAngle(0.f)
	, m_pOwner(nullptr)
	, m_pCamera(nullptr)
	, m_pScript(nullptr)
	, m_pTransform(nullptr)
	, m_bEqualizePlayerAxis_CamForward(false)
{

}

CTranslateMgr::CTranslateMgr(const CTranslateMgr& _origin)
	: m_fDefaultAngle(_origin.m_fDefaultAngle)
	, m_vObjForwardAxis(_origin.m_vObjForwardAxis)
	, m_fRotSpeed(_origin.m_fRotSpeed)
	, m_fMoveAccRot(_origin.m_fMoveAccRot)
	, m_fRotRadian(_origin.m_fRotRadian)
	, m_fRotAngle(_origin.m_fRotAngle)
	, m_bEqualizePlayerAxis_CamForward(_origin.m_bEqualizePlayerAxis_CamForward)

{

}

CTranslateMgr::~CTranslateMgr()
{

}



void CTranslateMgr::Init(CGameObject* _pOwner, CGameObject* _pCamera, CScript* _pPlayerScript)
{

	m_pOwner = _pOwner;
	m_pCamera = _pCamera;
	m_pScript = _pPlayerScript;
	m_pTransform = _pOwner->Transform();

}

void CTranslateMgr::Update()
{
	if (m_pOwner == nullptr)
		return;


	// 1.  180�� ȸ�����ѳ����� ���� -180 ~ 0 ~ 180 �� ���ǰ� �����Ѵ�.  [ -180 ~ 180 ]
	Vec3 vRot = m_pTransform->GetRelativeRotation();
	vRot.y -= XM_PI;
	m_pTransform->SetRelativeRotation(vRot);


	// 2. ���� ī�޶� ���� �����Ѵ�.
	UpdateCameraAxis();

	// 3. �÷��̾ �����δ�. 
	UpdateMovePlayer();

	// 4. ī�޶�� �÷��̾� ������ ������ �����Ѵ�.
	UpdateCameraPlayerAngle();

	// 5. �÷��̾ ȸ����Ų��.
	RotateBody();


	// 6. �� ĳ���Ͱ� �⺻������ ���� ������ �ٶ󺸰� �ֱ� ������ 180�� ������.    [ 0 ~ 360 ]
	vRot = m_pTransform->GetRelativeRotation();
	vRot.y += XM_PI;
	m_pTransform->SetRelativeRotation(vRot);

}



void CTranslateMgr::LateUpdate()
{

}


void CTranslateMgr::RotateBody()
{
	m_bKeyUpdateFinish = false;

	// Ű�� ���� ���� 
	CheckForwardKey();
	CheckBackwardKey();
	CheckRightKey();
	CheckLeftKey();


}

void CTranslateMgr::RotatePlayerFront(Vec3 vDir)
{
	if (m_fMoveAccRot == 0.f)
		return;

	Vec3 vStandard_Dir = vDir.Normalize();		// ������ �Ǵ� ���� 

	// �÷��̾� ������ ī�޶� �� ����� ������ ����  
	if (m_bEqualizePlayerAxis_CamForward)
	{
		m_fMoveAccRot = GetRadianBetweenVector(m_vCamAxis.vFrontAxis, m_vObjForwardAxis);
		if (m_fMoveAccRot <= 0.f)
			m_fMoveAccRot *= -1;
		vStandard_Dir = m_vCamAxis.vFrontAxis;
	}


	// 1. �÷��̾��� ���� �� ���� �� Dir ���� ���� ������ ���Ѵ�. 
	m_vObjForwardAxis = GetForwardAxis();
	float fRad_Rest = GetRadianBetweenVector(vStandard_Dir, m_vObjForwardAxis);		// �÷��̾ ���ƾ��� ���� ���� 

	// �÷��̾ ������. 
	int iDir_Rot = (fRad_Rest <= 0.f) ? -1 : 1;
	Vec3 vRot = m_pTransform->GetRelativeRotation();

	float fRotateDT = DT * m_fRotSpeed * iDir_Rot;
	bool bFinishRotate = CheckRotateFinish(fRotateDT);

	if (bFinishRotate == false)
	{
		vRot.y += fRotateDT;

		if (vRot.y <= -XM_PI)
			vRot.y = XM_PI;
		else if (vRot.y >= XM_PI)
			vRot.y = -XM_PI + 0.00001f;
	}

	m_pTransform->SetRelativeRotation(vRot);
	return;

}

bool CTranslateMgr::CheckRotateFinish(float _fRot_DT)
{
	float fDT = (_fRot_DT > 0.f) ? -1 * _fRot_DT : 1 * _fRot_DT;
	m_fMoveAccRot += fDT;
	if (m_fMoveAccRot <= 0.f)
	{
		m_fMoveAccRot = 0.f;
		return true;
	}
	else
		return false;


}

void CTranslateMgr::UpdateMovePlayer()
{

	PlayerScript* pPlayerScript = (PlayerScript*)m_pScript;
	m_tCurKeyInfo = pPlayerScript->GetObjKeyMgr()->GetCurKeyInfo();

	float fMoveForwardDT = 0.f;
	float fMoveSideDT = 0.f;

	// TAP
	//RigidBodyScript* pRigid = (RigidBodyScript*)m_pOwner->GetScriptByName(L"RigidBodyScript");
	CRigidBody* pRigid = m_pOwner->RigidBody();

	// ��
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::FORWARD
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
	{
		if (pRigid)
		{
			Vec3 vVelocity = m_vCamAxis.vFrontAxis * 10.f;
			pRigid->AddVelocity(vVelocity);
		}
	}

	// ��
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::BACKWARD
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
	{
		if (pRigid)
		{
			Vec3 vVelocity = m_vCamAxis.vBackAxis * 10.f;
			pRigid->AddVelocity(vVelocity);
		}
	}

	// ������ 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::RIGHT
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
	{
		if (pRigid)
		{
			Vec3 vVelocity = m_vCamAxis.vRightAxis * 10.f;
			pRigid->AddVelocity(vVelocity);
		}
	}

	// ���� 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LEFT
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP)
	{
		if (pRigid)
		{
			Vec3 vVelocity = m_vCamAxis.vLeftAxis * 10.f;
			pRigid->AddVelocity(vVelocity);
		}
	}

	// PRESSED 
	// ��
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED)
	{
		fMoveForwardDT += DT * pPlayerScript->GetPlayerStat()->GetStat().fSpeed;
	}

	// ��
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED)
	{
		fMoveForwardDT -= DT * pPlayerScript->GetPlayerStat()->GetStat().fSpeed;
	}

	// ������ 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED)
	{
		fMoveSideDT += DT * pPlayerScript->GetPlayerStat()->GetStat().fSpeed;
	}

	// ���� 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT
		&& m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED)
	{
		fMoveSideDT -= DT * pPlayerScript->GetPlayerStat()->GetStat().fSpeed;
	}

	m_vPrevPos = m_pOwner->Transform()->GetRelativePos();

	TranslateByRigidBody(m_vCamAxis.vFrontAxis, fMoveForwardDT * 10.f);
	TranslateByRigidBody(m_vCamAxis.vRightAxis, fMoveSideDT * 10.f);

	Walk(fMoveForwardDT * 2.f); // �� �� 
	Strafe(fMoveSideDT * 2.f);  // �� �� 

	m_vCurPos = m_pOwner->Transform()->GetRelativePos();

}


void CTranslateMgr::TranslateByRigidBody(const Vec3& direction, float magnitude)
{
	//RigidBodyScript* pRigid = (RigidBodyScript*)m_pOwner->GetScriptByName(L"RigidBodyScript");
	CRigidBody* pRigid = m_pOwner->RigidBody();
	if (pRigid != nullptr)
	{
		Vec3 directionVec = XMVector3Normalize(direction);
		Vec3 vForce = directionVec * magnitude;
		pRigid->AddForce(vForce);

	}
}

void CTranslateMgr::UpdateCameraAxis()
{
	PlayerScript* pPlayerScript = (PlayerScript*)m_pScript;
	//CGameObject* pPlayerCamera = pPlayerScript->GetCamera();

	// 1. Camera Right Axis 
	const Vec3		vCameraRot = m_pCamera->Transform()->GetRelativeRotation();
	const Matrix	matCameraRot = DirectX::XMMatrixRotationRollPitchYawFromVector(vCameraRot);
	Vec3			vCameraRight = { matCameraRot._11, matCameraRot._12, matCameraRot._13 };

	// ī�޶� ���� �������� �ʾҴٸ�
	if (vCameraRight.Normalize() == m_vCamAxis.vRightAxis)
		return;

	// Dx11 - �޼� ��ǥ�� / ������ ���ؼ� �� �� ���͸� ���Ѵ�.  
	m_vCamAxis.vFrontAxis = XMVector3Normalize(XMVector3Cross(vCameraRight, g_XMIdentityR1));
	m_vCamAxis.vRightAxis = XMVector3Normalize(vCameraRight);
	m_vCamAxis.vBackAxis = XMVector3Normalize(XMVector3Cross(vCameraRight, (g_XMIdentityR1 * -1.f)));
	m_vCamAxis.vLeftAxis = XMVector3Normalize(XMVector3Cross(m_vCamAxis.vFrontAxis, g_XMIdentityR1));

}
void CTranslateMgr::UpdateCameraPlayerAngle()
{
	// [ -180 ~ 180 ] - �ð� ������ ���

	// ī�޶�		���� ���� ���Ϳ�
	// �÷��̾	���� ���� ���� ������ ���� �����Ѵ�. 
	m_vObjForwardAxis = GetForwardAxis();

	// �� ���� ������ �������� 0 ~ 180 �� ���� ���� ���´�. 
	float fCos = m_vCamAxis.vFrontAxis.Dot(m_vObjForwardAxis);

	if (fCos <= -1.f)
		fCos = -1.f;
	if (fCos >= 1.f)
		fCos = 1.f;

	m_fRotRadian = acos(fCos);
	m_fRotAngle = m_fRotRadian * (180.f / XM_PI);
	if (m_fRotAngle == 180.f)
		return;

	// ���� -> ( + ) : �� ���� ������ / ( - ) : �� ���� ���� 
	Vec3 vCorss = XMVector3Cross(m_vObjForwardAxis, m_vCamAxis.vFrontAxis);
	// �� ���� ������ ���� 180 ���� �Ѵ� ��� -> ���� ( ������ )
	if (vCorss.y > 0.f)
	{
		// ���� ��
		//m_fRotRadian = acos(m_vCamAxis.vBackAxis.Dot(m_vObjForwardAxis)) + XM_PI;
		m_fRotRadian = acos(m_vCamAxis.vBackAxis.Dot(m_vObjForwardAxis)) * -1;
		m_fRotAngle = m_fRotRadian * (180.f / XM_PI);
	}

	if (m_fRotAngle == -180.f)
	{
		m_fRotRadian = XM_PI;
		m_fRotAngle = m_fRotRadian * (180.f / XM_PI);
	}
	return;

}

int CTranslateMgr::GetQuadrantNum(float _fRad)
{
	float fAngle = _fRad * (180.f / XM_PI);

	if (0.f <= fAngle && fAngle < 90.f)		return 1;
	if (90.f <= fAngle && fAngle < 180.f)		return 2;
	if (-90.f <= fAngle && fAngle < -180.f)		return 3;
	if (0.f <= fAngle && fAngle < -90.f)		return 4;

	return 0;


}
void CTranslateMgr::Strafe(float distance)
{
	// �÷��̾�� ī�޶� �ٶ󺸴� ������ ��������
	// �յ��¿� �� �����ȴ�.

	// �÷��̾� ī�޶� �����´�.
	PlayerScript* pPlayerScript = (PlayerScript*)m_pScript;
	CGameObject* pPlayerCamera = pPlayerScript->GetCamera();


	if (pPlayerCamera != nullptr)
	{
		Translate(m_vCamAxis.vRightAxis, distance);

	}
	else
	{
		Translate(GetRightAxis(), distance);

	}
}

Vec3 CTranslateMgr::Walk(float distance)
{
	// �÷��̾�� ī�޶� �ٶ󺸴� ������ ��������
	// �յ��¿� �� �����ȴ�.

	// �÷��̾� ī�޶� �����´�.
	PlayerScript* pPlayerScript = (PlayerScript*)m_pScript;
	CGameObject* pPlayerCamera = pPlayerScript->GetCamera();

	if (pPlayerCamera != nullptr)
	{
		// �÷��̾�� ī�޶� �ٶ󺸴� �չ����� �������� �����δ�. 
		Translate(m_vCamAxis.vFrontAxis, distance);
		return m_vCamAxis.vFrontAxis;

	}
	else
	{
		Vec3 rightVec = GetRightAxis();
		Vec3 frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
		Vec3  front = frontVec;
		XMStoreFloat3(&front, frontVec);

		Translate(front, distance);
		return front;

	}

}







float CTranslateMgr::GetAngleBetweenVector(Vec3 _vStandard, Vec3 _vTarget)
{
	if (_vStandard == _vTarget)
		return 0.f;
	if (_vStandard == -_vTarget)
		return XM_PI;

	// [ -180 ~ 180 Degree ]

		// ī�޶�		���� ���� ���Ϳ�
		// �÷��̾	���� ���� ���� ������ ���� �����Ѵ�. 
	Vec3 _vA = _vStandard;
	Vec3 _vB = _vTarget;

	_vA.Normalize();
	_vB.Normalize();

	// �� ���� ������ �������� 0 ~ 180 �� ���� ���� ���´�. -> �������� �̿��� CW/CCW �̵��������� Ȯ���Ѵ�. 
	float fCos = _vA.Dot(_vB);
	if (fCos >= 1.f) fCos = 1.f;
	if (fCos <= -1.f) fCos = -1.f;

	float fRotRadian = acos(fCos);
	float fRotAngle = fRotRadian * (180.f / XM_PI);

	Vec3 vCorss = XMVector3Cross(_vA, _vB);
	// _vTarget �������� ���� �������� ���ƾ� �ϴ� ��� ( ���� ) 
	if (vCorss.y > 0.f)
	{
		//fRotRadian = acos(_vC.Dot(_vB)) + XM_PI;
		fRotRadian *= -1.f;
		fRotAngle *= -1.f;
	}
	return fRotAngle;
}

float CTranslateMgr::GetRadianBetweenVector(Vec3 _vStandard, Vec3 _vTarget)
{
	if (_vStandard == _vTarget)
		return 0.f;
	if (_vStandard == -_vTarget)
		return XM_PI;

	// [ -180 ~ 180 Degree ]

		// ī�޶�		���� ���� ���Ϳ�
		// �÷��̾	���� ���� ���� ������ ���� �����Ѵ�. 
	Vec3 _vA = _vStandard;
	Vec3 _vB = _vTarget;

	_vA.Normalize();
	_vB.Normalize();

	// �� ���� ������ �������� 0 ~ 180 �� ���� ���� ���´�. -> �������� �̿��� CW/CCW �̵��������� Ȯ���Ѵ�. 
	float fCos = _vA.Dot(_vB);
	if (fCos >= 1.f) fCos = 1.f;
	if (fCos <= -1.f) fCos = -1.f;

	float fRotRadian = acos(fCos);
	float fRotAngle = fRotRadian * (180.f / XM_PI);

	Vec3 vCorss = XMVector3Cross(_vA, _vB);
	// _vTarget �������� ���� �������� ���ƾ� �ϴ� ��� ( ���� ) 
	if (vCorss.y > 0.f)
	{
		//fRotRadian = acos(_vC.Dot(_vB)) + XM_PI;
		fRotRadian *= -1.f;
		fRotAngle *= -1.f;
	}

	return fRotRadian;
}

Vec3 CTranslateMgr::GetDiagnosisBetweenVector(Vec3 _vA, Vec3 _vB)
{
	// �� ������ �߰�(����) ���͸� ����Ѵ�. ( �밢�� )
	Vec3 vDiag = Vec3(
		(_vA.x + _vB.x) * 0.5f,
		(_vA.y + _vB.y) * 0.5f,
		(_vA.z + _vB.z) * 0.5f
	);

	vDiag.Normalize();
	return vDiag;
}




void CTranslateMgr::SetScale(const Vec3& scale)
{
	m_pTransform->SetRelativeScale(scale);

}

void CTranslateMgr::SetScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(Vec3(x, y, z));
}

void CTranslateMgr::SetRotation(const Vec3& eulerAnglesInRadian)
{
	m_pTransform->SetRelativeRotation(eulerAnglesInRadian);
}

void CTranslateMgr::SetRotation(float x, float y, float z)
{
	m_pTransform->SetRelativeRotation(Vec3(x, y, z));

}

void CTranslateMgr::Rotate(const Vec3& eulerAnglesInRadian)
{
	//const XMVECTOR newRotationVec = XMVectorAdd(XMLoadFloat3(&m_Rotation), XMLoadFloat3(&eulerAnglesInRadian));
	//XMStoreFloat3(&m_Rotation, newRotationVec);

	Vec3 NewRotationVec = m_pTransform->GetRelativeRotation() + eulerAnglesInRadian;
	m_pTransform->SetRelativeRotation(NewRotationVec);

}

void CTranslateMgr::RotateAxis(const Vec3& axis, float radian)
{
	Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const Matrix matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector((Vector3)vRotation) * DirectX::XMMatrixRotationAxis(axis, radian);
	Vec3 vNewRotation = GetEulerAnglesFromRotationMatrix(matRotation);

	m_pTransform->SetRelativeRotation(vNewRotation);

}

void CTranslateMgr::RotateAround(const Vec3& point, const Vec3& axis, float radian)
{
	Vec3 vRotation = m_pTransform->GetRelativeRotation();

	// Rotate with point as the origin
	XMMATRIX RT
		= XMMatrixRotationRollPitchYawFromVector(vRotation) * XMMatrixTranslationFromVector(vRotation - point);
	RT *= XMMatrixRotationAxis(axis, radian);
	RT *= XMMatrixTranslationFromVector(point);
	Matrix rotMatrix{};
	XMStoreFloat4x4(&rotMatrix, RT);

	Vec3 NewRot = GetEulerAnglesFromRotationMatrix(rotMatrix);
	Vec3 NewPos = RT.r[3];

	m_pTransform->SetRelativeRotation(NewRot);
	m_pTransform->SetRelativePos(NewPos);

}

void CTranslateMgr::SetPosition(const Vec3& position)
{
	m_pTransform->SetRelativePos(position);
}

void CTranslateMgr::SetPosition(float x, float y, float z)
{
	m_pTransform->SetRelativePos(Vec3(x, y, z));

}

void CTranslateMgr::Translate(const Vec3& direction, float magnitude)
{
	Vec3 directionVec = XMVector3Normalize(direction);

	// SimpleMath ���� �� ���
	// XMVECTOR newPosition  = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec,
	// XMLoadFloat3(&m_Position)); XMStoreFloat3(&m_Position, newPosition);
	Vec3 vCurPos = m_pTransform->GetRelativePos();
	Vec3 vNewPos = directionVec * magnitude + vCurPos;

	m_pTransform->SetRelativePos(vNewPos);
}


Vec3 CTranslateMgr::GetScale() const
{
	return m_pTransform->GetRelativeScale();
}

Vec3 CTranslateMgr::GetRotation() const
{
	return 	m_pTransform->GetRelativeRotation();
}

Vec3 CTranslateMgr::GetPosition() const
{
	return 	m_pTransform->GetRelativePos();

}

Vec3 CTranslateMgr::GetRightAxis() const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const Matrix  matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(vRotation);
	Vec3 right = { matRotation._11, matRotation._12, matRotation._13 };

	right.Normalize();

	return right;
}

Vec3 CTranslateMgr::GetUpAxis() const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const Matrix  matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(vRotation);
	Vector3 up = { matRotation._21, matRotation._22, matRotation._23 };
	up.Normalize();

	return up;
}

Vec3 CTranslateMgr::GetForwardAxis(Vec3 vOffSetRot) const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();
	Vec3 vRot = vRotation + vOffSetRot;


	const XMMATRIX matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&vRot));
	Vector3        forward{};
	XMStoreFloat3(&forward, matRotation.r[2]);
	forward.Normalize();

	return forward;
}

Matrix CTranslateMgr::GetLocalToWorldMatrix() const
{

	const Matrix matScale = DirectX::XMMatrixScalingFromVector(m_pTransform->GetRelativeScale());
	const Matrix matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(m_pTransform->GetRelativeRotation());
	const Matrix matTranslation = DirectX::XMMatrixTranslationFromVector(m_pTransform->GetRelativePos());

	const Matrix World = matScale * matRotation * matTranslation;

	return World;
}

Matrix CTranslateMgr::GetWorldToLocalMatrix() const
{
	const XMMATRIX matWorldInverse = DirectX::XMMatrixInverse(nullptr, GetLocalToWorldMatrix());
	return matWorldInverse;
}



Vec3 CTranslateMgr::GetEulerAnglesFromRotationMatrix(const Matrix& rotationMatrix)
{
	// Find Euler angles by rotating matrix
	// when restoring Euler angles, it may be possible due to precision problems with floating-point
	// numbers m32 Inexplicably greater than 1, resulting in an undefined root part.
	float c = sqrtf(1.0f - rotationMatrix._32 * rotationMatrix._32);
	// Prevent R[2][1] more than 1
	if (std::isnan(c))
	{
		c = 0.0f;
	}

	Vector3 rotation;

	rotation.x = atan2f(-rotationMatrix._32, c);
	rotation.y = atan2f(rotationMatrix._31, rotationMatrix._33);
	rotation.z = atan2f(rotationMatrix._12, rotationMatrix._22);

	return rotation;
}


void CTranslateMgr::SetCurKeyInfo(tKey_Zip _keyInfo)
{
	m_tCurKeyInfo = _keyInfo;
}


int CTranslateMgr::GetRotateDirection(int _iQuadNum, float fAngle_Final)
{
	int iDir_Rot = 1;

	// �� �� ������ ���� ��� ����?
	switch (_iQuadNum)
	{
	case 1:  // 0< ~ <=90
	{
		if (fAngle_Final == 0.f)
			iDir_Rot = -1;
		else if (fAngle_Final == 90.f)
			iDir_Rot = 1;
		else if (fAngle_Final == 180.f)
			iDir_Rot = 1;
		else if (fAngle_Final == -90.f)
			iDir_Rot = -1;

	}
	break;
	case 2: // 90< ~ <=180
	{
		if (fAngle_Final == 0.f)
			iDir_Rot = -1;
		else if (fAngle_Final == 90.f)
			iDir_Rot = -1;
		else if (fAngle_Final == 180.f)
			iDir_Rot = 1;
		else if (fAngle_Final == -90.f)
			iDir_Rot = 1;
	}
	break;
	case 3: // 180< ~ <=270
	{
		if (fAngle_Final == 0.f)
			iDir_Rot = 1;
		else if (fAngle_Final == 90.f)
			iDir_Rot = -1;
		else if (fAngle_Final == 180.f)
			iDir_Rot = -1;
		else if (fAngle_Final == -90.f)
			iDir_Rot = 1;
	}
	break;
	case 4: // 270< ~ <= 360 (0)
	{
		if (fAngle_Final == 0.f)
			iDir_Rot = 1;
		else if (fAngle_Final == 90.f)
			iDir_Rot = 1;
		else if (fAngle_Final == 180.f)
			iDir_Rot = -1;
		else if (fAngle_Final == -90.f)
			iDir_Rot = -1;
	}
	break;
	}
	return iDir_Rot;

}


void CTranslateMgr::CheckForwardKey()
{
	if (true == m_bKeyUpdateFinish)
		return;

	// FORWARD / TAP
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::FORWARD)
	{

	}


	// FORWARD / PRESSED 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD)
	{

		if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT)
		{

			// Front / Right ���� �밢�� ���͸� ���Ѵ�. 
			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vRightAxis);
			// ȸ���ؾ��� ���� ũ�⸦ ���Ѵ�.
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;

			// ȸ���Ѵ�. 
			RotatePlayerFront(vDiagAxis);

		}
		else if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vLeftAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(vDiagAxis);

		}
		else
		{
			m_fMoveAccRot = GetRadianBetweenVector(m_vCamAxis.vFrontAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(m_vCamAxis.vFrontAxis);

		}

		m_bKeyUpdateFinish = true;

	}

	// FORWARD / AWAY
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::AWAY &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::FORWARD)
	{



	}
}

void CTranslateMgr::CheckBackwardKey()
{
	if (true == m_bKeyUpdateFinish)
		return;

	// BACKWARD / TAP
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::BACKWARD)
	{

	}


	// BACKWARD / PRESSED 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD)
	{

		// RIGHT / PRESSED 
		if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT)
		{

			// Front / Right ���� �밢�� ���͸� ���Ѵ�. 
			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vBackAxis, m_vCamAxis.vRightAxis);

			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;

			RotatePlayerFront(vDiagAxis);

		}
		// LEFT / PRESSED 
		else if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vBackAxis, m_vCamAxis.vLeftAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(vDiagAxis);

		}
		else
		{
			m_fMoveAccRot = GetRadianBetweenVector(m_vCamAxis.vBackAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(m_vCamAxis.vBackAxis);

		}

		m_bKeyUpdateFinish = true;

	}

	// BACKWARD / AWAY
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::AWAY &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::BACKWARD)
	{

	}

}


void CTranslateMgr::CheckRightKey()
{
	if (true == m_bKeyUpdateFinish)
		return;


	// RIGHT / TAP
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::RIGHT)
	{

	}


	// RIGHT / PRESSED 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::RIGHT)
	{

		// FORWARD / PRESSED 
		if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vRightAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(vDiagAxis);

		}
		// BACKWARD / PRESSED 
		else if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vLeftAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;

			RotatePlayerFront(vDiagAxis);

		}
		else
		{
			m_fMoveAccRot = GetRadianBetweenVector(m_vCamAxis.vRightAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;

			RotatePlayerFront(m_vCamAxis.vRightAxis);

		}

		m_bKeyUpdateFinish = true;

	}

	// RIGHT / AWAY
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::AWAY &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::RIGHT)
	{

	}


}

void CTranslateMgr::CheckLeftKey()
{
	if (true == m_bKeyUpdateFinish)
		return;

	// LEFT / TAP
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::TAP &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Tap & PLAYER_KEY_OPTION::LEFT)
	{

	}


	// LEFT / PRESSED 
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::LEFT)
	{

		// FORWARD / PRESSED 
		if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::FORWARD)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vRightAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(vDiagAxis);

		}
		// BACKWARD / PRESSED 
		else if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::PRESSED &&
			m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Pressed & PLAYER_KEY_OPTION::BACKWARD)
		{

			Vec3 vDiagAxis = GetDiagnosisBetweenVector(m_vCamAxis.vFrontAxis, m_vCamAxis.vLeftAxis);
			m_fMoveAccRot = GetRadianBetweenVector(vDiagAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(vDiagAxis);

		}
		else
		{
			m_fMoveAccRot = GetRadianBetweenVector(m_vCamAxis.vLeftAxis, m_vObjForwardAxis);
			if (m_fMoveAccRot <= 0.f)
				m_fMoveAccRot *= -1;
			RotatePlayerFront(m_vCamAxis.vLeftAxis);

		}

		m_bKeyUpdateFinish = true;

	}

	// RIGHT / AWAY
	if (m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::AWAY &&
		m_tCurKeyInfo.tKeyFlags_Zip.iKeyFlags_Away & PLAYER_KEY_OPTION::LEFT)
	{

	}
}

Vec3 CTranslateMgr::GetMoveDir()
{
	Vec3 vMoveDir = Vec3(0.f, 0.f, 0.f);

	vMoveDir = m_vCurPos - m_vPrevPos;
	vMoveDir.Normalize();

	return vMoveDir;
}
