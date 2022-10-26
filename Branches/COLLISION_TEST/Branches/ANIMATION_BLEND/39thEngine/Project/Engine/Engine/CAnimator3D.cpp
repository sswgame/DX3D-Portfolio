#include "pch.h"

#include "CAnimator3D.h"

#include "CAnimation3D.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"

CAnimator3D::CAnimator3D()
	:
	CComponent(COMPONENT_TYPE::ANIMATOR3D)
  , m_pCurrentAnimation{nullptr}
  , m_pNextAnimation{nullptr}
  , m_repeat{false}
  , m_pVecBones(nullptr)
  , m_pVecClip(nullptr)
  , m_iFrameCount(30)
  , m_currentTime(0.)
  , m_iCurClip(0)
  , m_currentFrameIndex(0)
  , m_nextFrameIndex(0)
  , m_fRatio(0.f)
  , m_pBoneFinalMatBuffer(nullptr)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	:
	CComponent(COMPONENT_TYPE::ANIMATOR3D)
  , m_pCurrentAnimation{nullptr}
  , m_pNextAnimation{nullptr}
  , m_repeat{false}
  , m_pVecBones(_origin.m_pVecBones)
  , m_pVecClip(_origin.m_pVecClip)
  , m_iFrameCount(_origin.m_iFrameCount)
  , m_currentTime(_origin.m_currentTime)
  , m_iCurClip(_origin.m_iCurClip)
  , m_currentFrameIndex(_origin.m_currentFrameIndex)
  , m_nextFrameIndex(_origin.m_nextFrameIndex)
  , m_fRatio(_origin.m_fRatio)
  , m_pBoneFinalMatBuffer(nullptr)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
	Safe_Del_Map(m_mapAnimation);
}


void CAnimator3D::finalupdate()
{
	if (nullptr == m_pCurrentAnimation)
	{
		return;
	}

	m_pCurrentAnimation->finalupdate();

	if (m_pCurrentAnimation->IsFinish() && m_repeat)
	{
		m_pCurrentAnimation->Reset();
	}
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	//어차피 강사님 것은 하나이므로,
	m_pVecClip = _vecAnimClip;

	if (nullptr == m_pCurrentAnimation)
	{
		Create(m_pVecClip->at(0));
		Play(m_pVecClip->at(0).strAnimName, true);
	}
}


void CAnimator3D::UpdateData()
{
	if (nullptr == m_pCurrentAnimation)
	{
		return;
	}
	m_pCurrentAnimation->UpdateData();
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT           iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl      = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

CAnimation3D* CAnimator3D::FindAnim(const std::wstring& animationName)
{
	const auto iter = m_mapAnimation.find(animationName);
	if (iter != m_mapAnimation.end())
	{
		return iter->second;
	}
	return nullptr;
}

void CAnimator3D::Play(const std::wstring& animationName, bool repeat)
{
	m_pCurrentAnimation = FindAnim(animationName);
	assert(m_pCurrentAnimation);

	m_repeat = repeat;
}


size_t CAnimator3D::GetBoneCount() const
{
	if (nullptr != m_pVecBones)
	{
		return m_pVecBones->size();
	}
	assert(nullptr);
	return 0;
}

int CAnimator3D::GetMaxFrameCount() const
{
	static int INIT_COUNT = m_pVecClip->at(0).iFrameLength;;
	return INIT_COUNT;
}

float CAnimator3D::GetMaxTime() const
{
	static float INIT_TIME = (float)m_pVecClip->at(0).dTimeLength;;
	return INIT_TIME;
}

void CAnimator3D::Create(const tMTAnimClip& animClip)
{
	CAnimation3D* pAnimation = new CAnimation3D{};
	pAnimation->Create(this, animClip);
	m_mapAnimation.insert({pAnimation->GetName(), pAnimation});
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveToScene(FILE* _pFile) {}

void CAnimator3D::LoadFromScene(FILE* _pFile) {}
