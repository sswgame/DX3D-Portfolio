#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimation3DShader.h"

#include "CKeyMgr.h"

#include "CAnimation3D.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iFrameCount(30)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{

}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iFrameCount(_origin.m_iFrameCount)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
}

CAnimator3D::~CAnimator3D()
{
	Safe_Del_Map(m_mapAnim);
}



void CAnimator3D::finalupdate()
{
	if (nullptr == m_pCurAnim)
		return;

	m_pCurAnim->finalupdate();

	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		m_pCurAnim->Reset();
	}
}



void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
}


void CAnimator3D::UpdateData()
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->UpdateData();
}

void CAnimator3D::ClearData()
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->ClearData();
}

CAnimation3D* CAnimator3D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator3D::CreateAnim(const wstring& _strName, int _clipNum
	, double _startTime, double _endTime, int _startFrame, int _EndFrame)
{
	if (FindAnim(_strName) != nullptr) return;
	//assert(!FindAnim(_strName));

	CAnimation3D* pAnim = new CAnimation3D;

	pAnim->SetName(_strName);
	pAnim->SetOwner(this);
	pAnim->SetClipUpdateTime(m_pVecClip->size());
	pAnim->SetFrameInfo(_clipNum, _startTime, _endTime, _startFrame, _EndFrame);

	m_mapAnim.insert(make_pair(_strName, pAnim));

}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnimation3D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pPrevAnim = m_pCurAnim;
	m_bRepeat = _bRepeat;

	if (m_pPrevAnim != nullptr)
	{
		// 애니메이션 종료 후 다른 애니메이션을 재생한 경우 
		if (m_pPrevAnim->IsFinish())
			pAnim->SetPrevFrameEndIdx(m_pPrevAnim->GetEndFrameIdx());

		// 애니메이션 진행 중 다른 애니메이션을 재생한 경우 
		else
			pAnim->SetPrevFrameEndIdx(m_pPrevAnim->GetCurFrameIdx());

	}

	m_pCurAnim = pAnim;
	m_pCurAnim->SetAnimState(ANIMATION_STATE::BEFORE_PLAY);

	if (m_pPrevAnim != nullptr)
		m_pPrevAnim->Reset();
}


void CAnimator3D::SaveToScene(FILE* _pFile)
{
}

void CAnimator3D::LoadFromScene(FILE* _pFile)
{
}


void CAnimator3D::RegisterNextAnim(CAnimation3D* _pNextAnim)
{
	m_pNextAnim = _pNextAnim;
	if (m_pCurAnim == m_pNextAnim || _pNextAnim == nullptr)
	{
		m_pNextAnim = nullptr;
	}
}


void CAnimator3D::Clear()
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		CAnimation3D* pDelAnim = iter->second;
		SAFE_DELETE(pDelAnim);
		iter->second = nullptr;
	}
	m_mapAnim.clear();

	m_pPrevAnim = nullptr;
	m_pCurAnim = nullptr;
	m_pNextAnim = nullptr;

	m_bRepeat = false;

}

void CAnimator3D::DeleteAnim(const wstring& _wstrName)
{
	if (m_mapAnim.find(_wstrName) == m_mapAnim.end())
		return;

	if (m_pCurAnim == m_mapAnim.find(_wstrName)->second)
	{
		if (m_pNextAnim != nullptr)
			m_pCurAnim = m_pNextAnim;
		else
			m_pCurAnim = nullptr;

		m_bRepeat = false;
	}
	CAnimation3D* pDelAnim = m_mapAnim.find(_wstrName)->second;
	SAFE_DELETE(pDelAnim);
	m_mapAnim.erase(_wstrName);
}


void CAnimator3D::SetLerpTimeOnAllAnim(float _fLerpTime)
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->SetLerpTime(_fLerpTime);
	}
}

