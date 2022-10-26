#include"pch.h"
#include "AnimSequence.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>
#define TO_STRING(wstr) std::string{wstr.begin(),wstr.end()}
#define TO_WSTRING(str) std::wstring{str.begin(),str.end()}

AnimSequence::AnimSequence()
	:
	m_pAnimator{nullptr}
  , m_hadDelete{false}
  , m_pDelete{nullptr} {}

void AnimSequence::ClearDeleteState()
{
	m_hadDelete = false;
	SAFE_DELETE(m_pDelete);
	m_pDelete = nullptr;
}

int AnimSequence::GetFrameMin() const
{
	return 0;
}

int AnimSequence::GetFrameMax() const
{
	return m_pAnimator->GetMaxFrameCount();
}

int AnimSequence::GetItemCount() const
{
	return static_cast<int>(m_pAnimator->GetAnimList().size());
}

const char* AnimSequence::GetItemLabel(int index) const
{
	const auto pAnimation = FindAnimWithIndex(index);
	assert(pAnimation);

	static std::string temp{};
	temp = TO_STRING(pAnimation->GetName()).c_str();
	return temp.c_str();
}

void AnimSequence::BeginEdit(int index)
{
	CAnimation3D* pAnimation = FindAnimWithIndex(index);
	assert(pAnimation);
	m_pAnimator->Play(pAnimation->GetName(), false);
	pAnimation->SetPause(true);
}

void AnimSequence::EndEdit()
{
	//여기선 프레임만 수정했으므로, 마지막에 그에 따른 시간 값으로 저장되도록 보정한다.
	for (auto& data : m_pAnimator->GetAnimList())
	{
		auto& info      = data.second->GetAnimInfo();
		info.dStartTime = static_cast<float>(info.iStartFrame) / m_pAnimator->GetMaxFrameCount() * m_pAnimator->
		                  GetMaxTime();
		info.dEndTime = static_cast<float>(info.iEndFrame) / m_pAnimator->GetMaxFrameCount() * m_pAnimator->
		                GetMaxTime();
		info.dTimeLength  = info.dEndTime - info.dStartTime;
		info.iFrameLength = info.iEndFrame - info.iStartFrame + 1;
	}
}

void AnimSequence::Get(int index, int** start, int** end, int* type, unsigned* color)
{
	auto pAnimation = FindAnimWithIndex(index);
	assert(pAnimation);
	tMTAnimClip& pItem = pAnimation->GetAnimInfo();
	if (color)
	{
		*color = 0xFFAA8080;
	}
	if (start)
	{
		*start = &pItem.iStartFrame;
	}
	if (end)
	{
		*end = &pItem.iEndFrame;
	}
	if (type)
	{
		*type = 0;
	}
}

void AnimSequence::Del(int index)
{
	auto pAnimation = FindAnimWithIndex(index);
	assert(pAnimation);
	if (m_pAnimator->GetAnimList().size() == 1)
	{
		return;
	}
	if (m_pAnimator->GetCurrentAnim() == pAnimation)
	{
		auto iter = std::find_if(m_pAnimator->GetAnimList().begin(),
		                         m_pAnimator->GetAnimList().end(),
		                         [pAnimation](const std::pair<std::wstring, CAnimation3D*>& data)
		                         {
			                         return data.second != pAnimation;
		                         });

		m_pAnimator->Play(iter->second->GetName(), false);
		m_pAnimator->GetCurrentAnim()->SetPause(true);
	}

	m_pAnimator->GetAnimList().erase(pAnimation->GetName());
	m_pDelete   = pAnimation;
	m_hadDelete = true;
}


CAnimation3D* AnimSequence::FindAnimWithIndex(int index) const
{
	auto iter = m_pAnimator->GetAnimList().begin();
	while (iter != m_pAnimator->GetAnimList().end())
	{
		if (std::distance(m_pAnimator->GetAnimList().begin(), iter) == index)
		{
			return iter->second;
		}
		++iter;
	}
	return nullptr;
}
