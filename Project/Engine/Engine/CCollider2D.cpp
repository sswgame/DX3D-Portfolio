#include "pch.h"
#include "CCollider2D.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTransform.h"
#include "CScript.h"
#include "CSerializer.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_eColliderType(COLLIDER2D_TYPE::BOX)
	, m_vOffsetPos(Vec2(0.f, 0.f))
	, m_vOffsetScale(Vec2(1.f, 1.f))
	, m_iCollisionCount(0)
{
	// Collider2D ��翡 �´� �޽� ����
	SetCollider2DType(m_eColliderType);

	// Collider2D ���� ���� ����	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl");
}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: CComponent(_Origin)
	, m_eColliderType(_Origin.m_eColliderType)
	, m_vOffsetPos(_Origin.m_vOffsetPos)
	, m_vOffsetScale(_Origin.m_vOffsetScale)
	, m_iCollisionCount(0)
{
	// Collider2D ��翡 �´� �޽� ����
	SetCollider2DType(m_eColliderType);

	// Collider2D ���� ���� ����	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider2DMtrl");
}

CCollider2D::~CCollider2D() {}

void CCollider2D::SetCollider2DType(COLLIDER2D_TYPE _type)
{
	m_eColliderType = _type;

	if (COLLIDER2D_TYPE::BOX == m_eColliderType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_LineStrip");
	}
	else
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_LineStrip");
	}
}

void CCollider2D::SetOffsetScale(Vec2 _vOffsetScale)
{
	m_vOffsetScale = _vOffsetScale;

	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}

void CCollider2D::SetOffsetScale(float _x, float _y)
{
	m_vOffsetScale = Vec2(_x, _y);

	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}

void CCollider2D::finalupdate()
{
	const Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, 0.f);
	const Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);
	m_matColWorld         = matScale * matTrans;

	const Vec3   scale       = Transform()->GetWorldScale();
	const Matrix matInvScale = XMMatrixInverse(nullptr, XMMatrixScaling(scale.x, scale.y, scale.z));

	// �浹ü ������ * ������Ʈ ���� ũ�� �����(ũ��^-1) * ������Ʈ ���� ���(ũ�� * ȸ�� * �̵�)
	m_matColWorld = m_matColWorld * matInvScale * Transform()->GetWorldMat();
}

void CCollider2D::UpdateData()
{
	g_transform.matWorld = m_matColWorld;
	g_transform.matWV    = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP   = g_transform.matWV * g_transform.matProj;

	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CCollider2D::render()
{
	UpdateData();

	m_pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iCollisionCount);
	m_pMtrl->UpdateData();

	m_pMesh->render(0);
}

void CCollider2D::OnCollisionEnter(CCollider2D* _Other)
{
	++m_iCollisionCount;

	for (auto& pScript : GetOwner()->GetScripts())
	{
		pScript->OnCollisionEnter(_Other->GetOwner());
	}
}

void CCollider2D::OnCollision(CCollider2D* _Other)
{
	for (auto& pScript : GetOwner()->GetScripts())
	{
		pScript->OnCollision(_Other->GetOwner());
	}
}

void CCollider2D::OnCollisionExit(CCollider2D* _Other)
{
	--m_iCollisionCount;

	for (auto& pScript : GetOwner()->GetScripts())
	{
		pScript->OnCollisionExit(_Other->GetOwner());
	}
}

void CCollider2D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vOffsetPos, sizeof(Vec2), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec2), 1, _pFile);
}

void CCollider2D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fread(&m_vOffsetPos, sizeof(Vec2), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec2), 1, _pFile);

	SetCollider2DType(m_eColliderType);
}

void CCollider2D::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_eColliderType) << YAML::Value << (int)m_eColliderType;
	emitter << YAML::Key << NAME_OF(m_vOffsetPos) << YAML::Value << m_vOffsetPos;
	emitter << YAML::Key << NAME_OF(m_vOffsetScale) << YAML::Value << m_vOffsetScale;
}

void CCollider2D::Deserialize(const YAML::Node& node)
{
	m_eColliderType = static_cast<COLLIDER2D_TYPE>(node[NAME_OF(m_eColliderType)].as<int>());
	m_vOffsetPos    = node[NAME_OF(m_vOffsetPos)].as<Vec2>();
	m_vOffsetScale  = node[NAME_OF(m_vOffsetScale)].as<Vec2>();

	SetCollider2DType(m_eColliderType);
}
