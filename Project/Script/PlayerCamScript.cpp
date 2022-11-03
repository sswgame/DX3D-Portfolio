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
	m_eCamMode = CAMERA_MODE::THIRD_PERSON; // 3��Ī Default

	SetDistance(500.f); // ĳ���Ϳ� ī�޶���� �Ÿ� ( ���̴� �ش�ȵ� )
	SetDistanceMinMax(10.f, 600.f);


}

void PlayerCamScript::update()
{


	switch (m_eCamMode)
	{
	case CAMERA_MODE::FIRST_PERSON: // 1��Ī 
	{
		UpdateFirstPersonMode();

	}
	break;
	case CAMERA_MODE::THIRD_PERSON: // 3��Ī 
	{
		UpdateThirdPersonMode();

	}
	break;
	case CAMERA_MODE::FREE:			// ����
	{

	}
	break;

	}

}

void PlayerCamScript::lateupdate()
{


	switch (m_eCamMode)
	{
	case CAMERA_MODE::FIRST_PERSON: // 1��Ī 
	{
		LateUpdateFirstPersonMode();
	}
	break;
	case CAMERA_MODE::THIRD_PERSON: // 3��Ī 
	{
		LeteUpdateThirdPersonMode();
	}
	break;
	case CAMERA_MODE::FREE:			// ���� 
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
	// 0. �ܺο��� ī�޶� ���� �߽� ��ġ(TargetPos) �� �����ؾ��Ѵ�.
	//m_pCam->SetTarget(Traget Pos);

	// 1. ī�޶� ȸ���Ѵ�. 
	Vec3 vRot = m_pCam->Transform()->GetRelativeRotation();
	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

	if (KEY_PRESSED(KEY::RBTN))
	{
		vRot.y += DT * vMouseDir.x * XM_PI;
		vRot.x -= DT * vMouseDir.y * XM_PI;

		// 0 ~ 360 �� ���� 
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


	// 2. ī�޶��� ��ġ�� �����Ѵ�.
	Vec3 vCamPos = UpdateCameraRelativePos();				// ī�޶� �÷��̾ ���󰡰� �Ѵ�. 
	m_pCam->Transform()->SetRelativePos(vCamPos);
	//m_pCam->Transform()->SetRelativePos(m_vTargetPos);	// ĳ���Ϳ� ī�޶��� ��ġ�� ��ġ 


	/*
	   3. �ٲ� ��ġ�� �������� ī�޶� ȸ������ �����Ѵ�. 

			*  ȸ������ �̶� ���� ������ ȸ������ �޶�����  Forward Axis �� �޶����� ������ 
				ī�޶� ������ġ�� �̵���Ű�µ� ������ ���ܼ��̴�. 
	*/
	m_pCam->Transform()->SetRelativeRotation(vRot);		// ī�޶� ȸ�� 

	// 4. ī�޶� �ڷ� �̵� ( 3��Ī ) 
	Translate(GetForwardAxis(), -m_fDistance);			// ī�޶� ���� �ٶ󺸴� �������� Distance ��ŭ [�ڷ�] �̵�

	// 2. ī�޶� ���� ���� 
	Vec3 vPos = m_pCam->Transform()->GetRelativePos();
	vPos.y = m_pCam->Transform()->GetRelativePos().y + m_fCamHeight; // ī�޶� ���̸� �ø���. (��� - ĳ���� ����) 
	m_pCam->Transform()->SetRelativePos(vPos);

	// 3. ī�޶�� ĳ���Ϳ��� �Ÿ��� ���� 
	// m_fDistance �� ���̸� ĳ���Ϳ� ���������. 
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

	// SimpleMath ���� �� ���
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

	//Translate(GetForwardAxis(), m_fDistance);			// ī�޶� ���� ��ġ�� �̵� 
	
	// 1. ī�޶��� ���� ��ġ�� �̵��Ѵ�. 
	vCamCurPos += GetForwardAxis() * m_fDistance;
	vCamCurPos.y -= m_fCamHeight; 
	
	if (vCamCurPos == m_vTargetPos)
		return vCamCurPos;;

	
	// 2. ī�޶� �̵��� ����� �Ÿ��� ���Ѵ�. ( �ǵ� : �÷��̾���� �Ÿ����� �̿��� ī�޶� �÷��̾ ���󰡰� �����. )
	Vec3 vDist		= m_vTargetPos - vCamCurPos; 
	Vec3 vLength	= vDist; 
	vLength			= Vec3(abs(vLength.x), abs(vLength.y), abs(vLength.z)); // �̵� �Ÿ� ����
	Vec3 vVector	= vDist.Normalize();									// �̵� ���� 

	// ī�޶� �÷��̾� ��ġ�� ���󰣴�. 
	// ��.... Length �� ª�������� �ð������� ������ ª�����ϱ�... ����� ������ �ʹ� ������ �ٰ�����.
	// ���� �Ÿ� ���̰� ������ ĳ�������� ���� �ӵ��� ��������..
	// �ָ� ����, ������ �����ӵ��� �÷������� ī�޶� ����������.
	// ��� �ұ�././

	// 3. �Ÿ� ����ŭ ī�޶� ��ġ�� �����Ѵ�. ( ��, �Ÿ����� �ʹ� ������ �ٰ����� �ӵ��� �׸�ŭ �۾����Ƿ� �������ش�. )
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