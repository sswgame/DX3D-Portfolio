#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"
#include "CEventMgr.h"
#include "CLayer.h"

CLight3D::CLight3D()
	:
	CComponent(COMPONENT_TYPE::LIGHT3D)
  , m_LightInfo{}
  , m_iLightIdx(-1)
  , m_pLightCam(nullptr)
  , m_pLightMeshObj(nullptr)
{
	m_pLightCam = new CGameObject;
	m_pLightCam->AddComponent(new CTransform);
	m_pLightCam->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _origin)
	:
	CComponent(_origin)
  , m_LightInfo(_origin.m_LightInfo)
  , m_iLightIdx(-1)
  , m_pLightCam(nullptr)
  , m_pLightMeshObj(nullptr)
{
	m_pLightCam = _origin.m_pLightCam->Clone();
}

CLight3D::~CLight3D()
{
	if (nullptr != m_pLightMeshObj)
		m_pLightMeshObj->Destroy();
	SAFE_DELETE(m_pLightCam);
}

void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_LightInfo.iLightType = (int)_eType;

	switch (_eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DirLightMtrl.mtrl");
		m_pLightCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pLightCam->Camera()->SetWidth(4096.f);
		m_pLightCam->Camera()->SetAspectRatio(1.f);
		m_pLightCam->Camera()->SetFar(100000.f);
		break;
	case LIGHT_TYPE::POINT:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PointLightMtrl.mtrl");
		break;
	case LIGHT_TYPE::SPOT:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SpotLightMtrl.mtrl");
		break;
	}
}

void CLight3D::SetObject()
{
	if (nullptr != m_pLightMeshObj)
		return;
	if (m_LightInfo.iLightType == 0)
		return;

	m_pLightMeshObj = new CGameObject;
	m_pLightMeshObj->SetName(L"Volume Mesh Line");
	m_pLightMeshObj->AddComponent(new CTransform);
	m_pLightMeshObj->AddComponent(new CMeshRender);
	m_pLightMeshObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	m_pLightMeshObj->Transform()->SetRelativeScale(GetOwner()->Transform()->GetRelativeScale());
	m_pLightMeshObj->Transform()->SetRelativeRotation(GetOwner()->Transform()->GetRelativeRotation());

	switch (m_LightInfo.iLightType)
	{
	case 1:
		m_pLightMeshObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
		break;
	case 2:
		m_pLightMeshObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh_LineStrip"));
		break;
	}

	m_pLightMeshObj->MeshRender()->
	                 SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DWireShader.mtrl"),0);
	CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Default");
	CSceneMgr::GetInst()->SpawnObject(m_pLightMeshObj, pLayer->GetLayerIdx());
}

void CLight3D::SetLightDir(Vec3 _vDir)
{
	m_LightInfo.vLightDir = _vDir;
	m_LightInfo.vLightDir.Normalize();

	Vec3 vFront = m_LightInfo.vLightDir;
	Vec3 vUp    = Vec3(0.f, 1.f, 0.f);
	Vec3 vRight = XMVector3Cross(vUp, vFront);
	vRight.Normalize();
	vUp = XMVector3Cross(vFront, vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();
	matRot._11    = vRight.x;
	matRot._12    = vRight.y;
	matRot._13    = vRight.z;
	matRot._21    = vUp.x;
	matRot._22    = vUp.y;
	matRot._23    = vUp.z;
	matRot._31    = vFront.x;
	matRot._32    = vFront.y;
	matRot._33    = vFront.z;

	Vec3 vRot = DecomposeRotMat(matRot);

	// ������ ����Ű�� ������ Transform �� Front �� �ǵ��� ȸ�������ش�.
	Transform()->SetRelativeRotation(vRot);
}

void CLight3D::SetRange(float _fRange)
{
	m_LightInfo.fRange = _fRange;
	Transform()->SetRelativeScale(_fRange * 2.f, _fRange * 2.f, _fRange * 2.f);
}

void CLight3D::update() {}

void CLight3D::finalupdate()
{
	if (nullptr != m_pLightMeshObj)
	{
		m_pLightMeshObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
		m_pLightMeshObj->Transform()->SetRelativeScale(Transform()->GetRelativeScale());
		m_pLightMeshObj->Transform()->SetRelativeRotation(Transform()->GetRelativeRotation());
	}

	if (KEY_TAP(KEY::_1))
	{
		SetObject();
	}
	if (KEY_TAP(KEY::_2))
	{
		if (nullptr != m_pLightMeshObj)
			m_pLightMeshObj->Destroy();
		m_pLightMeshObj = nullptr;
	}


	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vLightDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_LightInfo.fRange    = Transform()->GetWorldScale().x / 2.f;

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);


	// ������ ī�޶� ������ ������ Transform �� �ǵ��� ������Ʈ �Ѵ�.
	m_pLightCam->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_pLightCam->Transform()->SetRelativeRotation(DecomposeRotMat(Transform()->GetWorldRotation()));
	m_pLightCam->finalupdate_module();
}

void CLight3D::render()
{
	m_pLightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iLightIdx);

	// ���⼺ ������ ��� �׸��� ó���� ���ؼ� ����ī�޶�� ������ų �� �ְ� View * Proj ����� ����
	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.iLightType)
	{
		Matrix matLightVP = m_pLightCam->Camera()->GetViewMat() * m_pLightCam->Camera()->GetProjMat();
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, &matLightVP);
	}

	Transform()->UpdateData();
	m_pLightMtrl->UpdateData();

	m_pVolumeMesh->render(0);
}

void CLight3D::render_shadowmap()
{
	m_pLightCam->Camera()->SortShadowObject();
	m_pLightCam->Camera()->render_shadowmap();
}

void CLight3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);
}

void CLight3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);
}
