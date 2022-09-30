#include "pch.h"
#include "CFrustum.h"


#include "CCamera.h"
#include <math.h>

#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CFrustum::CFrustum()
	:
	m_ProjPos{}
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
	CalculateFrustumMat(); // Frustum Mesh 행렬 정보 계산 ( 시각화 목적 ) 

	// Frustum 을 소유하고 있는 카메라의 Proj 역행렬, View 역행렬 을 가져온다.
	const Matrix& matViewInv = m_pCam->GetViewInvMat();
	const Matrix& matProjInv = m_pCam->GetProjInvMat();

	// VP 역행렬을 곱해서 WorldPos 를 구한다.
	Matrix matVPInv = matProjInv * matViewInv;

	for (int i = 0; i < 8; ++i)
	{
		m_WorldPos[i] = XMVector3TransformCoord(m_ProjPos[i], matVPInv);
	}

	// 8개의 월드 좌표를 이용해서 월드상에서 절두체를 구성하는 6개의 평면을 정의한다.
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
		float fDot = _vPos.Dot(m_arrPlane[i]);
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
		float fDot = _vPos.Dot(m_arrPlane[i]);
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
		return;

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
		return;


	UpdateData();

	m_pMtrl->UpdateData();
	m_pMesh->render();
}

void CFrustum::CalculateFrustumMat()
{
	if (m_bShowFrustum == false)
		return;

	/*
		  1 -- 0
		  |	   |
	4(0,0)|	   |
		  |    |
		  2	-- 3
	*/

	float fWidth       = m_pCam->GetWidth();
	float fAspectRatio = m_pCam->GetAspectRatio();
	float fHeight      = fWidth / fAspectRatio;
	float fFOV         = m_pCam->GetFOV();
	float fFar         = m_pCam->GetFar();

	float FrustumHeight = 2.f * fFar * tanf(fFOV * 0.5f);
	float FrustumWidth  = FrustumHeight * fAspectRatio;

	Matrix matScale   = XMMatrixScaling(FrustumWidth, FrustumHeight, fFar);
	m_matFrustumWorld = matScale;

	Vec3   vObjScale      = m_pCam->Transform()->GetWorldScale();
	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z));

	// -> FrustumWorld : Camera 의 회전/이동 행렬에 영향 받음 크기는 자체적으로 구함 
	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬(크기^-1) * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	m_matFrustumWorld = m_matFrustumWorld * matObjScaleInv * m_pCam->Transform()->GetWorldMat();
}
