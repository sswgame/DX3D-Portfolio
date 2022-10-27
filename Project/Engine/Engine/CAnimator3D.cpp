#include "pch.h"

#include "CAnimator3D.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimation3DShader.h"
#include "CGameObject.h"
#include "CComponent.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pPrevAnim(nullptr)
	, m_pCurAnim(nullptr)
	, m_pNextAnim(nullptr)
	, m_bRepeat(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;

}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pPrevAnim(_origin.m_pPrevAnim)
	, m_pCurAnim(_origin.m_pCurAnim)
	, m_pNextAnim(_origin.m_pNextAnim)
	, m_bRepeat(_origin.m_bRepeat)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;

}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
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

void CAnimator3D::CopyAllAnim(const map<wstring, CAnimation3D*> _mapAnim)
{
	Clear();
	map<wstring, CAnimation3D*>::const_iterator iter = _mapAnim.begin();
	for (; iter != _mapAnim.end(); ++iter)
	{
		CreateAnim(iter->second->GetName(),
			iter->second->GetClipNum(),
			iter->second->GetStartTime(),
			iter->second->GetEndTime(),
			iter->second->GetStartFrameIdx(),
			iter->second->GetEndFrameIdx()
		);
	}
}

void CAnimator3D::CreateAnim(const wstring& _strName, int _clipNum
	, double _startTime, double _endTime, int _startFrame, int _EndFrame)
{
	if (FindAnim(_strName) != nullptr) return;
	//assert(!FindAnim(_strName));

	CAnimation3D* pAnim = new CAnimation3D;

	pAnim->SetName(_strName);
	pAnim->SetOwner(this);
	pAnim->SetClipUpdateTime((int)m_pVecClip->size());
	pAnim->SetFrameInfo(_clipNum, _startTime, _endTime, _startFrame, _EndFrame);

	m_mapAnim.insert(make_pair(_strName, pAnim));

	if (m_bPlayWithChild)
	{
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				pChildAnimator3D->CreateAnim(_strName, _clipNum, _startTime
					, _endTime, _startFrame, _EndFrame);
			}
		}
	}

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

	if (m_bPlayWithChild)
	{
		// 하위 객체들도 같이 애니메이션 Play 
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{

			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				CAnimation3D* pChildAnim = pChildAnimator3D->FindAnim(_strName);
				if (pChildAnim != nullptr)
				{
					pAnim->CopyInfo(&pChildAnim); // pChildAnim 에 pAnim 의 정보를 복사한다. 
					pChildAnimator3D->Play(_strName, _bRepeat);
				}
			}
		}
	}
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

	if (m_bPlayWithChild)
	{
		// [ DELETE PARENT OBJ / ANIMATION ] - 부모 애니 삭제 
		CGameObject* pParentObj = GetOwner()->GetParent();
		if (pParentObj != nullptr)
		{
			CAnimator3D* pAnimator3D = pParentObj->Animator3D();
			if (pAnimator3D != nullptr)
			{
				pAnimator3D->DeleteAnim(_wstrName);
			}
		}
		// [ DELETE CHILD OBJ / ANIMTION ] - 자식 애니 삭제 
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pAnimator3D = vecChildObj[i]->Animator3D();
			if (pAnimator3D != nullptr)
			{
				pAnimator3D->DeleteAnim(_wstrName);

			}
		}
	}




}


void CAnimator3D::SetLerpTimeOnAllAnim(float _fLerpTime)
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->SetLerpTime(_fLerpTime);
	}

	if (m_bPlayWithChild)
	{
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				pChildAnimator3D->SetLerpTimeOnAllAnim(_fLerpTime);
			}
		}
	}
}

void CAnimator3D::SetAnimState(wstring _Name, ANIMATION_STATE _eState)
{
	switch (_eState)
	{
	case ANIMATION_STATE::BEFORE_PLAY:
	{
		CAnimation3D* pAnim = FindAnim(_Name);
		pAnim->SetAnimState(_eState);

		// CHILD OBJECT SETTING 
		if (m_bPlayWithChild)
		{
			vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
			for (int i = 0; i < vecChildObj.size(); ++i)
			{
				CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
				if (pChildAnimator3D != nullptr)
				{
					CAnimation3D* pChildAnim3D = pChildAnimator3D->FindAnim(_Name);
					if (pChildAnim3D != nullptr)
						pChildAnim3D->SetAnimState(_eState);
				}
			}
		}
	}
	break;
	case ANIMATION_STATE::PLAY:
	{
		// OBJECT SETTING 
		CAnimation3D* pAnim = FindAnim(_Name);
		pAnim->SetAnimState(_eState);
		pAnim->Play(true);
		pAnim->SetFinish(false);

		// CHILD OBJECT SETTING 
		if (m_bPlayWithChild)
		{
			vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
			for (int i = 0; i < vecChildObj.size(); ++i)
			{
				CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
				if (pChildAnimator3D != nullptr)
				{
					CAnimation3D* pChildAnim3D = pChildAnimator3D->FindAnim(_Name);
					if (pChildAnim3D != nullptr)
					{
						pChildAnim3D->SetAnimState(_eState);
						pChildAnim3D->Play(true);
						pChildAnim3D->SetFinish(false);
					}
				}
			}
		}
	}
	break;
	case ANIMATION_STATE::STOP:
	{
		// OBJECT SETTING 
		CAnimation3D* pAnim = FindAnim(_Name);
		pAnim->SetAnimState(_eState);
		pAnim->Play(false);

		// CHILD OBJECT SETTING 
		if (m_bPlayWithChild)
		{
			vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
			for (int i = 0; i < vecChildObj.size(); ++i)
			{
				CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
				if (pChildAnimator3D != nullptr)
				{
					CAnimation3D* pChildAnim3D = pChildAnimator3D->FindAnim(_Name);
					if (pChildAnim3D != nullptr)
					{
						pChildAnim3D->SetAnimState(_eState);
						pChildAnim3D->Play(false);
					}
				}
			}
		}
	}
	break;
	case ANIMATION_STATE::FINISH:
	{
		CAnimation3D* pAnim = FindAnim(_Name);
		pAnim->SetAnimState(_eState);

		// CHILD OBJECT SETTING 
		if (m_bPlayWithChild)
		{
			vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
			for (int i = 0; i < vecChildObj.size(); ++i)
			{
				CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
				if (pChildAnimator3D != nullptr)
				{
					CAnimation3D* pChildAnim3D = pChildAnimator3D->FindAnim(_Name);
					if (pChildAnim3D != nullptr)
						pChildAnim3D->SetAnimState(_eState);
				}
			}
		}
	}
	break;
	}
}

void CAnimator3D::SetSpeed(float _fSpeed)
{
	m_pCurAnim->SetSpeed(_fSpeed);
	if (m_bPlayWithChild)
	{
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				CAnimation3D* pAnim = pChildAnimator3D->GetCurAnim();
				if (pAnim != nullptr)
					pAnim->SetSpeed(_fSpeed);

			}
		}
	}

}

void CAnimator3D::SetLerpTime(float _fTime)
{

	m_pCurAnim->SetLerpTime(_fTime);
	if (m_bPlayWithChild)
	{
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				CAnimation3D* pAnim = pChildAnimator3D->GetCurAnim();
				if (pAnim != nullptr)
					pAnim->SetLerpTime(_fTime);
			}
		}
	}
}


void CAnimator3D::SetPlayWithChild(bool _bPlayWithChild)
{
	m_bPlayWithChild = _bPlayWithChild;
	// 하위 객체들도 같이 애니메이션 Play 
	vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
	for (int i = 0; i < vecChildObj.size(); ++i)
	{
		CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
		if (pChildAnimator3D != nullptr)
		{
			pChildAnimator3D->SetPlayWithChild(_bPlayWithChild);
		}
	}
}


void CAnimator3D::SetRepeat(bool _b)
{
	m_bRepeat = _b;

	if (m_bPlayWithChild)
	{
		vector<CGameObject*> vecChildObj = GetOwner()->GetChild();
		for (int i = 0; i < vecChildObj.size(); ++i)
		{
			CAnimator3D* pChildAnimator3D = vecChildObj[i]->Animator3D();
			if (pChildAnimator3D != nullptr)
			{
				pChildAnimator3D->SetRepeat(_b);
			}
		}
	}
}