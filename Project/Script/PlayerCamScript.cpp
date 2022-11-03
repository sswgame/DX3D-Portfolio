#include "pch.h"
#include "PlayerCamScript.h"
#include <algorithm>
#include <math.h>

#include <Engine/CKeyMgr.h>

PlayerCamScript::PlayerCamScript()
	: CScript((int)SCRIPT_TYPE::PLAYERCAMSCRIPT)
	, m_vTargetPos(Vec3(0.f, 0.f, 0.f))
	, m_fDistance(0.f)
	, m_fMinDist(0.f)
	, m_fMaxDist(0.f)
	, m_fCamHeight(180.f)
{
	SetName(L"PlayerCamScript");

}

PlayerCamScript::~PlayerCamScript()
{
}


void PlayerCamScript::start()
{
	if (m_pCam == nullptr)
		m_pCam = GetOwner();

	m_vPos = m_vTargetPos;
	m_eCamMode = CAMERA_MODE::THIRD_PERSON; // 3인칭 Default

	SetDistance(500.f); // 캐릭터와 카메라와의 거리 ( 높이는 해당안됨 )
	SetDistanceMinMax(10.f, 600.f);


}

void PlayerCamScript::update()
{


	switch (m_eCamMode)
	{
	case CAMERA_MODE::FIRST_PERSON: // 1인칭 
	{
		UpdateFirstPersonMode();

	}
	break;
	case CAMERA_MODE::THIRD_PERSON: // 3인칭 
	{
		UpdateThirdPersonMode();

	}
	break;
	case CAMERA_MODE::FREE:			// 자유
	{

	}
	break;

	}

}

void PlayerCamScript::lateupdate()
{


	switch (m_eCamMode)
	{
	case CAMERA_MODE::FIRST_PERSON: // 1인칭 
	{
		LateUpdateFirstPersonMode();
	}
	break;
	case CAMERA_MODE::THIRD_PERSON: // 3인칭 
	{
		LeteUpdateThirdPersonMode();
	}
	break;
	case CAMERA_MODE::FREE:			// 자유 
	{

	}
	break;

	}

}


void PlayerCamScript::UpdateFirstPersonMode()
{

}

void PlayerCamScript::UpdateThirdPersonMode()
{
	// 0. 외부에서 카메라가 찍을 중심 위치(TargetPos) 를 갱신해야한다.
	//m_pCam->SetTarget(Traget Pos);

	// 1. 카메라를 회전한다. 
	Vec3 vRot = m_pCam->Transform()->GetRelativeRotation();
	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

	if (KEY_PRESSED(KEY::RBTN))
	{
		vRot.y += DT * vMouseDir.x * XM_PI;
		vRot.x -= DT * vMouseDir.y * XM_PI;

		// 0 ~ 360 도 고정 
		if (vRot.y >= XM_2PI)
		{
			float fDiff = vRot.y - XM_2PI;
			vRot.y = fDiff;
		}
		else if (vRot.y <= 0.f)
		{
			float fDiff = XM_2PI - vRot.y;
			vRot.y = fDiff;
		}

		if (vRot.x >= XM_2PI)
		{
			float fDiff = vRot.x - XM_2PI;
			vRot.x = fDiff;
		}
		else if (vRot.x <= 0.f)
		{
			float fDiff = XM_2PI - vRot.x;
			vRot.x = fDiff;
		}
		
	}


	// 2. 카메라의 위치를 조정한다.
	Vec3 vCamPos = UpdateCameraRelativePos();				// 카메라가 플레이어를 따라가게 한다. 
	m_pCam->Transform()->SetRelativePos(vCamPos);
	//m_pCam->Transform()->SetRelativePos(m_vTargetPos);	// 캐릭터와 카메라의 위치를 일치 


	/*
	   3. 바뀐 위치를 기준으로 카메라 회전각을 조정한다. 

			*  회전각을 이때 넣은 이유는 회전각이 달라지면  Forward Axis 도 달라지기 때문에 
				카메라를 원래위치로 이동시키는데 문제가 생겨서이다. 
	*/
	m_pCam->Transform()->SetRelativeRotation(vRot);		// 카메라 회전 

	// 4. 카메라를 뒤로 이동 ( 3인칭 ) 
	Translate(GetForwardAxis(), -m_fDistance);			// 카메라가 앞을 바라보는 기준으로 Distance 만큼 [뒤로] 이동

	// 2. 카메라 높이 설정 
	Vec3 vPos = m_pCam->Transform()->GetRelativePos();
	vPos.y = m_pCam->Transform()->GetRelativePos().y + m_fCamHeight; // 카메라 높이를 올린다. (상대 - 캐릭터 기준) 
	m_pCam->Transform()->SetRelativePos(vPos);

	// 3. 카메라와 캐릭터와의 거리를 갱신 
	// m_fDistance 를 줄이면 캐릭터와 가까워진다. 
	//pThirdPersonCamera->Approach(-io.MouseWheel * 1.0f);

}

void PlayerCamScript::LateUpdateFirstPersonMode()
{

}

void PlayerCamScript::LeteUpdateThirdPersonMode()
{

}


void PlayerCamScript::RotateX(float rad)
{
	Vector3 rotation = m_pCam->Transform()->GetRelativeRotation();
	// Limit rotation around the x-axis in radians between [0, pi/3]

	rotation.x += rad;
	if (rotation.x <= 0.f) rotation.x = 0.f;
	else if (rotation.x >= XM_PI / 3.f) rotation.x = XM_PI / 3.f;


	m_pCam->Transform()->SetRelativeRotation(rotation);
	m_pCam->Transform()->SetRelativePos(m_vTargetPos);
	Translate(GetForwardAxis(), -m_fDistance);

}

void PlayerCamScript::RotateY(float rad)
{
	Vector3 rotation = m_pCam->Transform()->GetRelativeRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);

	m_pCam->Transform()->SetRelativeRotation(rotation);
	m_pCam->Transform()->SetRelativePos(m_vTargetPos);
	Translate(GetForwardAxis(), -m_fDistance);
}

void PlayerCamScript::Approach(float dist)
{
	m_fDistance += dist;
	if (m_fDistance <= 0.f) m_fDistance = 0.f;
	else if (m_fDistance >= m_fMaxDist) m_fDistance = m_fMaxDist;


	m_pCam->Transform()->SetRelativePos(m_vTargetPos);
	Translate(GetForwardAxis(), -m_fDistance);
}

void PlayerCamScript::Translate(const Vec3& direction, float magnitude)
{
	Vec3 directionVec = XMVector3Normalize(direction);

	// SimpleMath 없을 때 방법
	// XMVECTOR newPosition  = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec,
	// XMLoadFloat3(&m_Position)); XMStoreFloat3(&m_Position, newPosition);
	Vec3 vCurPos = m_pCam->Transform()->GetRelativePos();
	Vec3 vNewPos = directionVec * magnitude + vCurPos;

	m_pCam->Transform()->SetRelativePos(vNewPos);

}

Vec3 PlayerCamScript::GetForwardAxis()
{
	const Vec3 vRotation = m_pCam->Transform()->GetRelativeRotation();

	const XMMATRIX matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&vRotation));
	Vector3        forward{};
	XMStoreFloat3(&forward, matRotation.r[2]);
	forward.Normalize();

	return forward;
}

Vec3 PlayerCamScript::GetTargetPosition() const
{
	return m_vTargetPos;
}

float PlayerCamScript::GetDistance() const
{
	return m_fDistance;
}

void PlayerCamScript::SetRotationX(float rad)
{
	Vec3 rotation = m_pCam->Transform()->GetRelativeRotation();
	rotation.x += rad;
	if (rotation.x <= 0.f) rotation.x = 0.f;
	else if (rotation.x >= XM_PI / 3.f) rotation.x = XM_PI / 3.f;

	m_pCam->Transform()->SetRelativeRotation(rotation);
	m_pCam->Transform()->SetRelativePos(m_vTargetPos);
	Translate(GetForwardAxis(), -m_fDistance);
}

void PlayerCamScript::SetRotationY(float rad)
{
}

void PlayerCamScript::SetTargetPos(const Vector3& target)
{
	if (m_vTargetPos != target)
		m_vPrevTargetPos = m_vTargetPos;

	m_vTargetPos = target;

}

void PlayerCamScript::SetDistance(float dist)
{
	m_fDistance = dist;

}

void PlayerCamScript::SetDistanceMinMax(float minDist, float maxDist)
{
	m_fMinDist = minDist;
	m_fMaxDist = maxDist;
}

Vec3 PlayerCamScript::UpdateCameraRelativePos()
{
	Vec3 vCamCurPos = m_pCam->Transform()->GetRelativePos();
	if (vCamCurPos == m_vTargetPos)
		return vCamCurPos;;

	//Translate(GetForwardAxis(), m_fDistance);			// 카메라 원래 위치로 이동 
	
	// 1. 카메라의 원래 위치로 이동한다. 
	vCamCurPos += GetForwardAxis() * m_fDistance;
	vCamCurPos.y -= m_fCamHeight; 
	
	if (vCamCurPos == m_vTargetPos)
		return vCamCurPos;;

	
	// 2. 카메라가 이동할 방향과 거리를 구한다. ( 의도 : 플레이어와의 거리차를 이용해 카메라가 플레이어를 따라가게 만든다. )
	Vec3 vDist		= m_vTargetPos - vCamCurPos; 
	Vec3 vLength	= vDist; 
	vLength			= Vec3(abs(vLength.x), abs(vLength.y), abs(vLength.z)); // 이동 거리 길이
	Vec3 vVector	= vDist.Normalize();									// 이동 방향 

	// 카메라가 플레이어 위치를 따라간다. 
	// 흠.... Length 가 짧아질수록 시간단축이 굉장히 짧아지니까... 가까워 질수록 너무 느리게 다가간다.
	// 일정 거리 차이가 있으면 캐릭터한테 가는 속도를 고정하자..
	// 멀면 빨리, 가까우면 일정속도로 플레어한테 카메라가 움직여야지.
	// 어떻게 할까././

	// 3. 거리 차만큼 카메라 위치를 조정한다. ( 단, 거리차가 너무 작으면 다가가는 속도도 그만큼 작아지므로 지정해준다. )
	float fXSpeed = 0.01f;
	if (vDist.x <= 10.f)
		fXSpeed = 2.f;
	
	float fYSpeed = 0.01f;
	if (vDist.y <= 10.f)
		fYSpeed = 2.f;

	float fZSpeed = 0.01f;
	if (vDist.z <= 10.f)
		fZSpeed = 2.f;

	vCamCurPos.x += DT * vLength.x * vVector.x * fXSpeed;
	vCamCurPos.y += DT * vLength.y * vVector.y * fYSpeed;
	vCamCurPos.z += DT * vLength.z * vVector.z * fZSpeed;

	if (vVector.x < 0.f)
	{
		if (vCamCurPos.x <= m_vTargetPos.x)
			vCamCurPos.x = m_vTargetPos.x;
	}
	else
	{
		if (vCamCurPos.x >= m_vTargetPos.x)
			vCamCurPos.x = m_vTargetPos.x;
	}

	if (vVector.y < 0.f)
	{
		if (vCamCurPos.y <= m_vTargetPos.y)
			vCamCurPos.y = m_vTargetPos.y;
	}
	else
	{
		if (vCamCurPos.y >= m_vTargetPos.y)
			vCamCurPos.y = m_vTargetPos.y;
	}

	if (vVector.z < 0.f)
	{
		if (vCamCurPos.z <= m_vTargetPos.z)
			vCamCurPos.z = m_vTargetPos.z;
	}
	else
	{
		if (vCamCurPos.z >= m_vTargetPos.z)
			vCamCurPos.z = m_vTargetPos.z;
	}


	return vCamCurPos;

}


void PlayerCamScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void PlayerCamScript::OnCollision(CGameObject* _OtherObject)
{
}

void PlayerCamScript::OnCollisionExit(CGameObject* _OtherObject)
{
}


void PlayerCamScript::SaveToScene(FILE* _pFile)
{
}

void PlayerCamScript::LoadFromScene(FILE* _pFile)
{
}