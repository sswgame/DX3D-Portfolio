#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CFrustum::CFrustum()
	: m_ProjPos{}
	, m_WorldPos{}
	, m_arrPlane{}
	, m_pCam(nullptr)
	, m_bShowFrustum{false}
{
	//   4 ---- 5
	//  /|    / |
	// 0 -- 1   |
	// | 6 -|-- 7
	// 2 -- 3/
	m_ProjPos[0] = Vec3(-1.f, 1.f, 0.f);
	m_ProjPos[1] = Vec3(1.f, 1.f, 0.f);
	m_ProjPos[2] = Vec3(-1.f, -1.f, 0.f);
	m_ProjPos[3] = Vec3(1.f, -1.f, 0.f);

	m_ProjPos[4] = Vec3(-1.f, 1.f, 1.f);
	m_ProjPos[5] = Vec3(1.f, 1.f, 1.f);
	m_ProjPos[6] = Vec3(-1.f, -1.f, 1.f);
	m_ProjPos[7] = Vec3(1.f, -1.f, 1.f);


	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PentahedronMesh");
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DWireShader.mtrl");
}

CFrustum::~CFrustum() {}

void CFrustum::finalupdate()
{
	CalculateFrustumMat(); // Frustum Mesh ��� ���� ��� ( �ð�ȭ ���� ) 

	// Frustum �� �����ϰ� �ִ� ī�޶��� Proj �����, View ����� �� �����´�.
	const Matrix& matViewInv = m_pCam->GetViewInvMat();
	const Matrix& matProjInv = m_pCam->GetProjInvMat();

	// VP ������� ���ؼ� WorldPos �� ���Ѵ�.
	const Matrix matInvViewPort = matProjInv * matViewInv;
	for (int i = 0; i < 8; ++i)
	{
		m_WorldPos[i] = XMVector3TransformCoord(m_ProjPos[i], matInvViewPort);
	}

	// 8���� ���� ��ǥ�� �̿��ؼ� ����󿡼� ����ü�� �����ϴ� 6���� ����� �����Ѵ�.
	//   4 ---- 5
	//  /|    / |
	// 0 -- 1   |
	// | 6 -|-- 7
	// 2 -- 3/
	m_arrPlane[(UINT)PLANE::PL_LEFT]  = XMPlaneFromPoints(m_WorldPos[4], m_WorldPos[0], m_WorldPos[2]);
	m_arrPlane[(UINT)PLANE::PL_RIGHT] = XMPlaneFromPoints(m_WorldPos[1], m_WorldPos[5], m_WorldPos[7]);

	m_arrPlane[(UINT)PLANE::PL_UP]   = XMPlaneFromPoints(m_WorldPos[0], m_WorldPos[4], m_WorldPos[5]);
	m_arrPlane[(UINT)PLANE::PL_DOWN] = XMPlaneFromPoints(m_WorldPos[2], m_WorldPos[3], m_WorldPos[7]);

	m_arrPlane[(UINT)PLANE::PL_NEAR] = XMPlaneFromPoints(m_WorldPos[2], m_WorldPos[0], m_WorldPos[1]);
	m_arrPlane[(UINT)PLANE::PL_FAR]  = XMPlaneFromPoints(m_WorldPos[7], m_WorldPos[5], m_WorldPos[4]);
}

bool CFrustum::PointCheck(Vec3 _vPos)
{
	for (int i = 0; i < (UINT)PLANE::END; ++i)
	{
		// N dot P = D
		const float fDot = _vPos.Dot(m_arrPlane[i]);
		if (fDot + m_arrPlane[i].w > 0)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::SphereCheck(Vec3 _vPos, float _fRadius)
{
	for (int i = 0; i < (UINT)PLANE::END; ++i)
	{
		// N dot P = D
		const float fDot = _vPos.Dot(m_arrPlane[i]);
		if (fDot + m_arrPlane[i].w > _fRadius)
		{
			return false;
		}
	}

	return true;
}


void CFrustum::UpdateData()
{
	if (m_bShowFrustum == false)
	{
		return;
	}

	g_transform.matWorld = m_matFrustumWorld;
	g_transform.matWV    = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP   = g_transform.matWV * g_transform.matProj;

	Vec4 FrustumColor = Vec4(0.1f, 0.6f, 1.f, 1.f);
	m_pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &FrustumColor);

	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CFrustum::render()
{
	if (m_bShowFrustum == false)
	{
		return;
	}
	UpdateData();

	m_pMtrl->UpdateData();
	m_pMesh->render(0);
}

void CFrustum::CalculateFrustumMat()
{
	if (m_bShowFrustum == false)
	{
		return;
	}
	/*
		  1 -- 0
		  |	   |
	4(0,0)|	   |
		  |    |
		  2	-- 3
	*/

	const float fAspectRatio = m_pCam->GetAspectRatio();
	const float fFOV         = m_pCam->GetFOV();
	const float fFar         = m_pCam->GetFar();

	const float FrustumHeight = 2.f * fFar * tanf(fFOV * 0.5f);
	const float FrustumWidth  = FrustumHeight * fAspectRatio;

	const Matrix matScale    = XMMatrixScaling(FrustumWidth, FrustumHeight, fFar);
	m_matFrustumWorld        = matScale;
	const Vec3   scale       = m_pCam->Transform()->GetWorldScale();
	const Matrix matInvScale = XMMatrixInverse(nullptr, XMMatrixScaling(scale.x, scale.y, scale.z));

	// -> FrustumWorld : Camera �� ȸ��/�̵� ��Ŀ� ���� ���� ũ��� ��ü������ ���� 
	// �浹ü ������ * ������Ʈ ���� ũ�� �����(ũ��^-1) * ������Ʈ ���� ���(ũ�� * ȸ�� * �̵�)
	m_matFrustumWorld = m_matFrustumWorld * matInvScale * m_pCam->Transform()->GetWorldMat();
}

bool CFrustum::CheckBoundingBoxInFrustum(Vec3 _BoundingBoxCenterPos, Vec3 _BoundingBoxScale)
{
	const Vec3 Center = _BoundingBoxCenterPos;
	const Vec3 Size   = _BoundingBoxScale;

	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (size_t i = 0; i < std::size(m_arrPlane); i++)
	{
		Vec3 vPos = Vec3((Center.x - Size.x), (Center.y - Size.y), (Center.z - Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x + Size.x), (Center.y - Size.y), (Center.z - Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x - Size.x), (Center.y + Size.y), (Center.z - Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x - Size.x), (Center.y - Size.y), (Center.z + Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x + Size.x), (Center.y + Size.y), (Center.z - Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x + Size.x), (Center.y - Size.y), (Center.z + Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x - Size.x), (Center.y + Size.y), (Center.z + Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;
		vPos = Vec3((Center.x + Size.x), (Center.y + Size.y), (Center.z + Size.z));
		if (vPos.Dot(m_arrPlane[i]) + m_arrPlane[i].w <= 0)
			continue;

		return false;
	}

	return true;
}
