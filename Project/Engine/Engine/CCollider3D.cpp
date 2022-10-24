#include "pch.h"
#include "CCollider3D.h"

#include "CConstBuffer.h"
#include "CScript.h"
#include "CTransform.h"

CCollider3D::CCollider3D()
	: CComponent{COMPONENT_TYPE::COLLIDER3D}
	, m_vOffsetScale{1.f, 1.f, 1.f}
	, m_iCollisionCount{0}
	, m_eCollider3DType{COLLIDER3D_TYPE::CUBE}
{
	m_pMesh     = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	m_pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider3D.mtrl");
}

CCollider3D::CCollider3D(const CCollider3D& _origin)
	: CComponent{COMPONENT_TYPE::COLLIDER3D}
	, m_vOffsetPos{_origin.m_vOffsetPos}
	, m_vOffsetScale{_origin.m_vOffsetScale}
	, m_iCollisionCount{0}
	, m_eCollider3DType{_origin.m_eCollider3DType}
	, m_pMesh{_origin.m_pMesh}
	, m_pMaterial{_origin.m_pMaterial}
{
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::SetCollider3DType(COLLIDER3D_TYPE _type)
{
	if (COLLIDER3D_TYPE::CUBE == _type)
	{
		m_eCollider3DType = _type;
		m_pMesh           = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	}
	else
	{
		m_eCollider3DType = _type;
		m_pMesh           = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		SetOffseetScale(m_vOffsetScale);
	}
}

void CCollider3D::SetOffsetPos(Vec3 _vOffsetPos)
{
	m_vOffsetPos = _vOffsetPos;
}

void CCollider3D::SetOffseetScale(Vec3 _vOffsetScale)
{
	m_vOffsetScale = _vOffsetScale;

	if (COLLIDER3D_TYPE::SPHERE == m_eCollider3DType)
	{
		m_vOffsetScale.x = m_vOffsetScale.y = m_vOffsetScale.z;
	}
}

COLLIDER3D_TYPE CCollider3D::GetCollider3DType() const
{
	return m_eCollider3DType;
}

Vec3 CCollider3D::GetWorldPos() const
{
	return m_matWorld.Translation();
}

Vec3 CCollider3D::GetWorldScale() const
{
	return m_vOffsetScale;
}

Matrix CCollider3D::GetWorldMatrix() const
{
	return m_matWorld;
}

void CCollider3D::OnCollisionEnter(CCollider3D* _pOther)
{
	++m_iCollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (auto& pScript : vecScript)
	{
		pScript->OnCollisionEnter(_pOther->GetOwner());
	}
}

void CCollider3D::OnCollision(CCollider3D* _pOther)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (auto& pScript : vecScript)
	{
		pScript->OnCollision(_pOther->GetOwner());
	}
}

void CCollider3D::OnCollisionExit(CCollider3D* _pOther)
{
	--m_iCollisionCount;
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (auto& pScript : vecScript)
	{
		pScript->OnCollisionExit(_pOther->GetOwner());
	}
}


void CCollider3D::UpdateData()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	g_transform.matWorld     = m_matWorld;
	g_transform.matWorldInv  = XMMatrixInverse(nullptr, m_matWorld);
	g_transform.matWV        = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP       = g_transform.matWV * g_transform.matProj;

	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CCollider3D::finalupdate()
{
	const Matrix matTranslation = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);
	const Matrix matRotation    = Matrix{};
	const Matrix matScale       = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matWorld                  = matScale * matRotation * matTranslation;

	const Vec3   vGameObjectScale          = Transform()->GetWorldScale();
	const Matrix matGameObjectScaleInverse = XMMatrixInverse(nullptr,
	                                                         XMMatrixScaling(vGameObjectScale.x
	                                                                         , vGameObjectScale.y
	                                                                         , vGameObjectScale.z));
	const Matrix matGameobjectWorld = Transform()->GetWorldMat();
	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	m_matWorld = m_matWorld * matGameObjectScaleInverse * matGameobjectWorld;
}

void CCollider3D::render()
{
	UpdateData();

	m_pMaterial->SetScalarParam(SCALAR_PARAM::INT_0, &m_iCollisionCount);
	m_pMaterial->UpdateData();

	m_pMesh->render(0);
}

void CCollider3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_eCollider3DType, sizeof(UINT), 1, _pFile);
}

void CCollider3D::LoadFromScene(FILE* _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec2), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec2), 1, _pFile);
	fread(&m_eCollider3DType, sizeof(UINT), 1, _pFile);

	SetCollider3DType(m_eCollider3DType);
}
