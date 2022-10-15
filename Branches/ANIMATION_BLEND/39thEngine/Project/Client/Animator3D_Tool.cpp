#include "pch.h"
#include "Animator3D_Tool.h"

#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include <ImGuiGizumo/ImSequencer.h>

#define TO_STRING(wstr) std::string{wstr.begin(),wstr.end()}
#define TO_WSTRING(str) std::wstring{str.begin(),str.end()}

namespace
{
	static char  szAnimName[100]{};
	static bool  showError{};
	static int   arrInputFrameIndex[2]{};
	static float arrInputFrameTime[2]{};
	static float frameTime  = 0.f;
	static int   frameIndex = 0;
	static bool  createAnim{};
	static bool  firstOpenPopUp{};
	static float lerpTime{};
	static bool  playList{};
}

Animator3D_Tool::Animator3D_Tool()
	:
	UI{"Animator3D_Tool"}
  , m_pAnimator{nullptr} {}

Animator3D_Tool::~Animator3D_Tool() {}

void Animator3D_Tool::render_update()
{
	RenderCurrentAnimInfo();
	RenderPopUp();
	RenderPlayList();
	ImGui::SameLine();
	RenderTimeLine();


	//CLOSED
	if (false == IsActive())
	{
		m_pAnimator = nullptr;
	}
}

void Animator3D_Tool::RenderCurrentAnimInfo()
{
	ImGui::BeginGroup();
	ImGui::PushItemWidth(100);
#pragma region DRAW_INFO

	const auto& mapAnim = m_pAnimator->GetAnimList();
	ImGui::Text("ANIM CLIP COUNT : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(mapAnim.size()).c_str());

	const auto& pCurrentAnim = m_pAnimator->GetCurrentAnim();
	ImGui::Text("CLIP NAME :");
	ImGui::SameLine();
	const std::string name = (nullptr != pCurrentAnim) ? TO_STRING(pCurrentAnim->GetName()).c_str() : "NONE";
	ImGui::Text(name.c_str());

	ImGui::Text("TOTAL FRAME COUNT : ");
	ImGui::SameLine();
	const int count = (nullptr != pCurrentAnim) ? pCurrentAnim->GetAnimInfo().iFrameLength : 0;
	ImGui::Text(std::to_string(count).c_str());

	ImGui::Text("DURATION : ");
	ImGui::SameLine();
	const float duration = (nullptr != pCurrentAnim) ? pCurrentAnim->GetAnimInfo().dTimeLength : 0.f;
	ImGui::Text(std::to_string(duration).c_str());

	ImGui::Text("FPS : ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_pAnimator->GetLeastFrameCount()).c_str());

#pragma endregion

#pragma region DRAW_INDEX
	ImGui::BeginGroup();
	ImGui::Text("FRAME RANGE");
	ImGui::SameLine();
	if (ImGui::DragInt("##START_INDEX",
	                   &arrInputFrameIndex[0],
	                   1,
	                   0,
	                   m_pAnimator->GetMaxFrameCount()))
	{
		Calculate(arrInputFrameIndex, arrInputFrameTime, true);
	}
	ImGui::SameLine();
	ImGui::Text(" - ");
	ImGui::SameLine();
	if (ImGui::DragInt("##END_INDEX",
	                   &arrInputFrameIndex[1],
	                   1,
	                   0,
	                   m_pAnimator->GetMaxFrameCount()))
	{
		Calculate(arrInputFrameIndex, arrInputFrameTime, true);
	}
	ImGui::SameLine();
	ImGui::Text("(MAX RANGE : %d ~ %d)",
	            m_pAnimator->GetCurrentAnim()->GetAnimInfo().iStartFrame,
	            m_pAnimator->GetMaxFrameCount());
	ImGui::EndGroup();
#pragma endregion

#pragma region DRAW_TIME

	ImGui::BeginGroup();
	ImGui::Text("TIME RANGE");
	ImGui::SameLine();
	if (ImGui::DragFloat("##START_TIME",
	                     &arrInputFrameTime[0],
	                     1.f / m_pAnimator->GetLeastFrameCount(),
	                     0.f,
	                     m_pAnimator->GetMaxTime(),
	                     "%.3f",
	                     ImGuiSliderFlags_AlwaysClamp))
	{
		Calculate(arrInputFrameIndex, arrInputFrameTime, false);
	}
	ImGui::SameLine();
	ImGui::Text(" - ");
	ImGui::SameLine();
	if (ImGui::DragFloat("##END_TIME",
	                     &arrInputFrameTime[1],
	                     1.f / m_pAnimator->GetLeastFrameCount(),
	                     0.f,
	                     m_pAnimator->GetMaxTime(),
	                     "%.3f",
	                     ImGuiSliderFlags_AlwaysClamp))
	{
		Calculate(arrInputFrameIndex, arrInputFrameTime, false);
	}
	ImGui::SameLine();
	ImGui::Text("(MAX RANGE : %.3f ~ %.3f)",
	            0.f,
	            m_pAnimator->GetMaxTime());
	ImGui::EndGroup();


#pragma endregion

#pragma region DRAW_BUTTON
	ImGui::Text("LERP TIME : ");
	ImGui::SameLine();

	lerpTime = m_pAnimator->GetCurrentAnim()->GetLerpTime();
	if (ImGui::InputFloat("##LERP_INPUT", &lerpTime))
	{
		lerpTime = Clamp(lerpTime, 0.f, static_cast<float>(m_pAnimator->GetCurrentAnim()->GetAnimInfo().dTimeLength));
	}
	m_pAnimator->GetCurrentAnim()->SetLerpTime(lerpTime);


	if (ImGui::Button("Play##ANIMATOR_3D"))
	{
		m_pAnimator->GetCurrentAnim()->SetPause(false);
		m_pAnimator->SetRepeat(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop##ANIMATOR3D"))
	{
		m_pAnimator->GetCurrentAnim()->SetPause(true);
		playList = false;
	}
	if (ImGui::Button("MAKE ANIM CLIP"))
	{
		createAnim     = true;
		firstOpenPopUp = true;
	}

	if (ImGui::Button("ADD TO PLAY LIST"))
	{
		//중복되서 넣지 않아야 한다.
		const auto iter = std::find(m_vecPlayList.begin(), m_vecPlayList.end(), m_pAnimator->GetCurrentAnim());
		if (iter == m_vecPlayList.end())
		{
			m_vecPlayList.push_back(m_pAnimator->GetCurrentAnim());
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("REMOVE FROM PLAY LIST"))
	{
		const auto iter = std::find(m_vecPlayList.begin(), m_vecPlayList.end(), m_pAnimator->GetCurrentAnim());
		if (iter != m_vecPlayList.end())
		{
			m_vecPlayList.erase(iter);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("PLAY THE LIST") && m_vecPlayList.size() != 0)
	{
		for (const auto& pAnimation : m_vecPlayList)
		{
			pAnimation->Reset();
		}
		m_pAnimator->Play(m_vecPlayList[0]->GetName(), false);
		if (m_vecPlayList.size() > 1)
		{
			m_pAnimator->SetNextAnim(m_vecPlayList[1]);
		}
		playList = true;
	}

	static size_t nextIndex = 1;
	if (playList)
	{
		if (m_pAnimator->GetCurrentAnim()->IsFinish())
		{
			m_pAnimator->GetCurrentAnim()->Reset();
			if (m_pAnimator->GetNextAnim())
			{
				m_pAnimator->Play(m_pAnimator->GetNextAnim()->GetName(), false);
				if (++nextIndex < m_vecPlayList.size())
				{
					m_pAnimator->SetNextAnim(m_vecPlayList[nextIndex]);
				}
				else
				{
					nextIndex = 0;
					m_pAnimator->SetNextAnim(nullptr);
				}
			}
			else
			{
				m_pAnimator->Play(m_vecPlayList[nextIndex]->GetName(), false);
				if (++nextIndex < m_vecPlayList.size())
				{
					m_pAnimator->SetNextAnim(m_vecPlayList[nextIndex]);
				}
			}
		}
	}
	if (ImGui::Button("CLEAR LIST"))
	{
		nextIndex = 1;
		m_vecPlayList.clear();
		m_pAnimator->SetNextAnim(nullptr);
		m_pAnimator->Play(m_pAnimator->GetAnimList().begin()->second->GetName(), false);
		m_pAnimator->GetCurrentAnim()->Reset();
		playList = false;
	}
#pragma endregion

	ImGui::PopItemWidth();
	ImGui::EndGroup();
}

void Animator3D_Tool::RenderPopUp()
{
	if (createAnim)
	{
		ImGui::OpenPopup("ANIM_POP_UP");
	}
	if (ImGui::BeginPopup("ANIM_POP_UP"))
	{
		static int   arrNewIndex[2]{};
		static float arrNewTime[2]{};
		static float newLerpTime{};
		if (firstOpenPopUp)
		{
			arrNewIndex[0] = arrInputFrameIndex[0];
			arrNewIndex[1] = arrInputFrameIndex[1];

			arrNewTime[0] = arrInputFrameTime[0];
			arrNewTime[1] = arrInputFrameTime[1];

			newLerpTime    = lerpTime;
			firstOpenPopUp = false;
		}
		ImGui::Text("ANIM NAME :");
		ImGui::SameLine();
		static char szNewName[100]{};
		ImGui::InputTextWithHint("##ANIM_NAME", "Input animation name", szNewName, std::size(szNewName));


		ImGui::Text("START/END FRAME INDEX : ");
		ImGui::SameLine();
		if (ImGui::InputInt2("##INPUT_FRAME_INDEX", arrNewIndex))
		{
			Calculate(arrNewIndex, arrNewTime, true);
		}

		ImGui::Text("START/END FRAME TIME : ");
		ImGui::SameLine();

		if (ImGui::InputFloat2("##INPUT_FRAME_TIME", arrNewTime))
		{
			Calculate(arrNewIndex, arrNewTime, false);
		}

		ImGui::Text("LERP TIME");
		ImGui::SameLine();
		if (ImGui::InputFloat("##NEW_LERP_INPUT", &newLerpTime))
		{
			newLerpTime = Clamp(newLerpTime, 0.f, arrNewTime[1] - arrNewTime[0]);
		}

		if (ImGui::Button("Create"))
		{
			const std::string animName = szNewName;
			if (animName.empty())
			{
				showError = true;
			}
			else
			{
				if (nullptr != m_pAnimator->FindAnim(TO_WSTRING(animName)))
				{
					showError = true;
				}
				else
				{
					tMTAnimClip clip{};
					clip.eMode        = m_pAnimator->GetMode();
					clip.strAnimName  = TO_WSTRING(animName);
					clip.iStartFrame  = arrNewIndex[0];
					clip.iEndFrame    = arrNewIndex[1];
					clip.iFrameLength = clip.iEndFrame - clip.iStartFrame + 1;
					clip.dStartTime   = arrNewTime[0];
					clip.dEndTime     = arrNewTime[1];
					clip.dTimeLength  = clip.dEndTime - clip.dStartTime;
					m_pAnimator->Create(clip);
					m_pAnimator->FindAnim(clip.strAnimName)->SetLerpTime(newLerpTime);

					createAnim = false;
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("CANCEL"))
		{
			createAnim = false;
			ImGui::CloseCurrentPopup();
		}

		if (showError)
		{
			ImGui::OpenPopup("##POP_UP_ERROR");
			if (ImGui::BeginPopup("##POP_UP_ERROR"))
			{
				ImGui::Text("INVALID NAME OR ALREADY EXISTS");
				if (ImGui::Button("CLOSE"))
				{
					ImGui::CloseCurrentPopup();
					showError = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void Animator3D_Tool::RenderPlayList()
{
	ImGui::PushItemWidth(200);

	static int selectedIndex = -1;
	if (ImGui::BeginListBox("##PLAYLIST"))
	{
		for (size_t i = 0; i < m_vecPlayList.size(); ++i)
		{
			const bool isSelected = (selectedIndex == i);
			if (ImGui::Selectable(TO_STRING(m_vecPlayList[i]->GetName()).c_str(), isSelected))
			{
				m_pAnimator->Play(m_vecPlayList[i]->GetName(), false);
				selectedIndex = i;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		if (false == ImGui::IsItemFocused())
		{
			selectedIndex = -1;
		}
		ImGui::EndListBox();
	}

	ImGui::PopItemWidth();
}

void Animator3D_Tool::RenderTimeLine()
{
	if (ImGui::BeginChild("TIME LINE"))
	{
		static bool expanded      = true;
		static int  selectedEntry = -1;
		static int  firstFrame    = 0;
		frameIndex                = m_pAnimator->GetCurrentAnim()->GetCurrentIndex();

		int beforeEdit = frameIndex;
		if (m_animSequence.m_pAnimator == nullptr)
		{
			m_animSequence.SetAnimator(m_pAnimator);
		}
		Sequencer(&m_animSequence,
		          &frameIndex,
		          &expanded,
		          &selectedEntry,
		          &firstFrame,
		          ImSequencer::SEQUENCER_EDIT_ALL | ImSequencer::SEQUENCER_DEL);
		int afterEdit = frameIndex;
		//맵에서 삭제한 것이므로, 벡터에서 있다면 제거 필요
		if (m_animSequence.HasDelete())
		{
			auto vecIter = m_vecPlayList.begin();
			while (vecIter != m_vecPlayList.end())
			{
				auto mapIter = m_pAnimator->GetAnimList().find((*vecIter)->GetName());
				if (mapIter == m_pAnimator->GetAnimList().end())
				{
					vecIter = m_vecPlayList.erase(vecIter);
					break;
				}
				else
				{
					++vecIter;
				}
			}
			m_animSequence.ClearDeleteState();
		}
		//사용자가 임의로 현재 진행 중인 인덱스를 변경한 것이므로 반영 필요
		if (beforeEdit != afterEdit)
		{
			float time = (float)afterEdit / m_pAnimator->GetMaxFrameCount() * m_pAnimator->GetMaxTime();
			m_pAnimator->GetCurrentAnim()->SetFrameTime(time);
		}
		if (selectedEntry != -1)
		{
			CAnimation3D* pAnimation = m_animSequence.FindAnimWithIndex(selectedEntry);
			assert(pAnimation);
			auto& info            = pAnimation->GetAnimInfo();
			arrInputFrameIndex[0] = info.iStartFrame;
			arrInputFrameIndex[1] = info.iEndFrame;

			arrInputFrameTime[0] = (float)info.dStartTime;
			arrInputFrameTime[1] = (float)info.dEndTime;

			//m_pAnimator->Play(pAnimation->GetName(), false);
		}
		ImGui::EndChild();
	}
}

void Animator3D_Tool::Calculate(int (&arrIndex)[2], float (&arrFloat)[2], bool byIndex)
{
	const int   totalFrameCount = m_pAnimator->GetMaxFrameCount();
	const float totalTime       = m_pAnimator->GetMaxTime();
	const float FPS             = m_pAnimator->GetLeastFrameCount();

	int& startIndex = arrIndex[0];
	int& endIndex   = arrIndex[1];

	float& startTime = arrFloat[0];
	float& endTime   = arrFloat[1];

	if (byIndex)
	{
		startIndex = Clamp(startIndex, 0, totalFrameCount);
		endIndex   = Clamp(endIndex, 0, totalFrameCount);

		startTime = static_cast<float>(startIndex) / totalFrameCount * totalTime;
		endTime   = static_cast<float>(endIndex) / totalFrameCount * totalTime;

		startTime = Clamp(startTime, 0.f, totalTime);
		endTime   = Clamp(endTime, 0.f, totalTime);
	}
	else
	{
		startTime = Clamp(startTime, 0.f, totalTime);
		endTime   = Clamp(endTime, 0.f, totalTime);

		startIndex = static_cast<int>(startTime * FPS);
		endIndex   = static_cast<int>(endTime * FPS);

		startIndex = Clamp(startIndex, 0, totalFrameCount);
		endIndex   = Clamp(endIndex, 0, totalFrameCount);
	}

	if (startIndex > endIndex)
	{
		std::swap(startIndex, endIndex);
		std::swap(startTime, endTime);
	}

	if (false == createAnim)
	{
		CAnimation3D* pAnimation = m_pAnimator->GetCurrentAnim();
		auto&         info       = pAnimation->GetAnimInfo();
		info.iStartFrame         = startIndex;
		info.iEndFrame           = endIndex;
		info.dStartTime          = startTime;
		info.dEndTime            = endTime;
		info.iFrameLength        = endIndex - startIndex + 1;
		info.dTimeLength         = endTime - startTime;
	}
}

void Animator3D_Tool::SetAnimator3D(CAnimator3D* pAnimator)
{
	m_pAnimator = pAnimator;
}
