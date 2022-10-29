#include "pch.h"
#include "CPlayerMoveState.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include "PlayerScript.h"
#include "CScriptMgr.h"



CPlayerMoveState::CPlayerMoveState()
	:CState(L"MOVE")
	, m_fMoveForwardDT(0.f)
	, m_fMoveSideDT(0.f)

{
}

CPlayerMoveState::CPlayerMoveState(const CPlayerMoveState& _origin)
	:CState(_origin)

{
}

CPlayerMoveState::~CPlayerMoveState()
{
}


void CPlayerMoveState::Enter()
{
	CState::ResetTimer();
	m_pTransform		= CState::GetOwner()->Transform();





}

void CPlayerMoveState::Exit()
{

}

void CPlayerMoveState::Update()
{
	CState::Update();
	CGameObject* pGameObj = CState::GetOwner();
	PlayerScript* pPlayerScript = nullptr;
	if (pGameObj != nullptr)
		pPlayerScript = (PlayerScript*)CScriptMgr::GetScript(L"PlayerScript");

	if (pPlayerScript != nullptr)
		 m_tCurKeyInfo = pPlayerScript->GetObjKeyMgr()->GetCurKeyInfo();



	m_fMoveForwardDT = 0.f;
	m_fMoveSideDT = 0.f;

	if (m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::FORWARD
		&& m_tCurKeyInfo.iKeyFlags & PLAYER_KEY_OPTION::PRESSED)
	{
		m_fMoveForwardDT += DT;
	}


	Walk(m_fMoveForwardDT);
	Strafe(m_fMoveSideDT);

}

void CPlayerMoveState::LateUpdate()
{
	CState::LateUpdate();

}

void CPlayerMoveState::Strafe(float distance)
{
}

void CPlayerMoveState::Walk(float distance)
{
	XMVECTOR rightVec = GetRightAxis();
	XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
	Vector3  front = frontVec;
	XMStoreFloat3(&front, frontVec);

	Translate(front, distance);
}

void CPlayerMoveState::MoveForward(float distance)
{
}

void CPlayerMoveState::Pitch(float radian)
{
}

void CPlayerMoveState::Yaw(float radian)
{    
	//시야의 위/아래 회전(카메라의 오른쪽 축 주위)과 회전 각도가 너무 커지는 것을 방지하기 위해 회전 각도를 제한합니다

}

void CPlayerMoveState::LookAt(const Vec3& target, const Vec3& up)
{
	Vec3 vCurPos = m_pTransform->GetRelativePos();

	const XMMATRIX matView = XMMatrixLookAtLH(vCurPos, target, up);
	const XMMATRIX matViewInverse = XMMatrixInverse(nullptr, matView);
	Matrix         rotMatrix;
	XMStoreFloat4x4(&rotMatrix, matViewInverse);
	
	Vec3 vNewRot = GetEulerAnglesFromRotationMatrix(rotMatrix);
	m_pTransform->SetRelativeRotation(vNewRot);

}

void CPlayerMoveState::LookTo(const Vec3& direction, const Vec3& up)
{
	Vec3 vCurPos = m_pTransform->GetRelativePos();

	const XMMATRIX matView = XMMatrixLookToLH(vCurPos, direction, up);
	const XMMATRIX matViewInverse = XMMatrixInverse(nullptr, matView);
	Matrix         rotMatrix{};
	XMStoreFloat4x4(&rotMatrix, matViewInverse);

	Vec3 vNewRot = GetEulerAnglesFromRotationMatrix(rotMatrix);
	m_pTransform->SetRelativeRotation(vNewRot);
}

void CPlayerMoveState::SetScale(const Vec3& scale)
{
	m_pTransform->SetRelativeScale(scale);

}

void CPlayerMoveState::SetScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(Vec3(x, y, z));
}

void CPlayerMoveState::SetRotation(const Vec3& eulerAnglesInRadian)
{
	m_pTransform->SetRelativeRotation(eulerAnglesInRadian);
}

void CPlayerMoveState::SetRotation(float x, float y, float z)
{
	m_pTransform->SetRelativeRotation(Vec3(x, y, z));

}

void CPlayerMoveState::Rotate(const Vec3& eulerAnglesInRadian)
{
	//const XMVECTOR newRotationVec = XMVectorAdd(XMLoadFloat3(&m_Rotation), XMLoadFloat3(&eulerAnglesInRadian));
	//XMStoreFloat3(&m_Rotation, newRotationVec);

	Vec3 NewRotationVec = m_pTransform->GetRelativeRotation() + eulerAnglesInRadian;
	m_pTransform->SetRelativeRotation(NewRotationVec);

}

void CPlayerMoveState::RotateAxis(const Vec3& axis, float radian)
{
	Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const Matrix matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector((Vector3)vRotation) * DirectX::XMMatrixRotationAxis(axis, radian);
	Vec3 vNewRotation = GetEulerAnglesFromRotationMatrix(matRotation);
	
	m_pTransform->SetRelativeRotation(vNewRotation);

}

void CPlayerMoveState::RotateAround(const Vec3& point, const Vec3& axis, float radian)
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

void CPlayerMoveState::SetPosition(const Vec3& position)
{
	m_pTransform->SetRelativePos(position);
}

void CPlayerMoveState::SetPosition(float x, float y, float z)
{
	m_pTransform->SetRelativePos(Vec3(x, y, z));

}



void CPlayerMoveState::Translate(const Vec3& direction, float magnitude)
{
	Vec3 directionVec = XMVector3Normalize(direction);

	// SimpleMath 없을 때 방법
	// XMVECTOR newPosition  = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec,
	// XMLoadFloat3(&m_Position)); XMStoreFloat3(&m_Position, newPosition);
	Vec3 vCurPos = m_pTransform->GetRelativePos();
	Vec3 vNewPos = directionVec * magnitude + vCurPos;
	
	m_pTransform->SetRelativePos(vNewPos);
}


Vec3 CPlayerMoveState::GetScale() const
{
	return m_pTransform->GetRelativeScale();
}

Vec3 CPlayerMoveState::GetRotation() const
{
	return 	m_pTransform->GetRelativeRotation();
}

Vec3 CPlayerMoveState::GetPosition() const
{
	return 	m_pTransform->GetRelativePos();

}

Vec3 CPlayerMoveState::GetRightAxis() const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();
	 
	const Matrix  matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(vRotation);
	const Vector3 right = { matRotation._11, matRotation._12, matRotation._13 };

	return right;
}

Vec3 CPlayerMoveState::GetUpAxis() const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const Matrix  matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(vRotation);
	const Vector3 up = { matRotation._21, matRotation._22, matRotation._23 };

	return up;
}

Vec3 CPlayerMoveState::GetForwardAxis() const
{
	const Vec3 vRotation = m_pTransform->GetRelativeRotation();

	const XMMATRIX matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&vRotation));
	Vector3        forward{};
	XMStoreFloat3(&forward, matRotation.r[2]);
	return forward;
}

Matrix CPlayerMoveState::GetLocalToWorldMatrix() const
{
	
	const Matrix matScale = DirectX::XMMatrixScalingFromVector(m_pTransform->GetRelativeScale());
	const Matrix matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(m_pTransform->GetRelativeRotation());
	const Matrix matTranslation = DirectX::XMMatrixTranslationFromVector(m_pTransform->GetRelativePos());

	const Matrix World = matScale * matRotation * matTranslation;
	
	return World;
}

Matrix CPlayerMoveState::GetWorldToLocalMatrix() const
{
	const XMMATRIX matWorldInverse = DirectX::XMMatrixInverse(nullptr, GetLocalToWorldMatrix());
	return matWorldInverse;
}

Vec3 CPlayerMoveState::GetEulerAnglesFromRotationMatrix(const Matrix& rotationMatrix)
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


void CPlayerMoveState::SetCurKeyInfo(tKey_Zip _keyInfo)
{
	m_tCurKeyInfo = _keyInfo;
}
