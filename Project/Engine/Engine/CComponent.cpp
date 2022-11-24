#include "pch.h"
#include "CComponent.h"

#include "CEventMgr.h"

#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CCamera.h"
#include "CCollider2D.h"
#include "CDecal.h"
#include "CLandScape.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CMeshRender.h"
#include "CParticleSystem.h"
#include "CSkyBox.h"
#include "CTileMap.h"
#include "CTransform.h"
#include "CCollider3D.h"
#include "CFSM.h"

CComponent::CComponent(COMPONENT_TYPE _eType)
	: m_eComType(_eType)
	, m_pOwner{nullptr}
	, m_bActive(true)
	, m_pDebugObj(nullptr)
{
}

CComponent::~CComponent()
{
	if (nullptr != m_pDebugObj)
		delete(m_pDebugObj);
}

void CComponent::Activate()
{
	tEventInfo info = {};

	info.eType  = EVENT_TYPE::ACTIVATE_COMPONENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CComponent::Deactivate()
{
	tEventInfo info = {};

	info.eType  = EVENT_TYPE::DEACTIVATE_COMOPNENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

CComponent* CComponent::MakeComponent(const std::wstring& name)
{
	if (L"TRANSFORM" == name) return new CTransform{};
	if (L"CAMERA" == name) return new CCamera{};
	if (L"COLLIDER2D" == name) return new CCollider2D{};
	if (L"COLLIDER3D" == name) return new CCollider3D{};
	if (L"ANIMATOR2D" == name) return new CAnimator2D{};
	if (L"ANIMATOR3D" == name) return new CAnimator3D{};
	if (L"LIGHT2D" == name) return new CLight2D{};
	if (L"LIGHT3D" == name) return new CLight3D{};
	//	if (L"BOUNDINGBOX" == name) return new CBoundingBox{};
	if (L"MESHRENDER" == name) return new CMeshRender{};
	if (L"TILEMAP" == name) return new CTileMap{};
	if (L"PARTICLESYSTEM" == name) return new CParticleSystem{};
	if (L"LANDSCAPE" == name) return new CLandScape{};
	if (L"DECAL" == name) return new CDecal{};
	if (L"SKYBOX" == name) return new CSkyBox{};
	if (L"FINITE_STATE_MACHINE" == name) return new CFSM{};

	return nullptr;
}
