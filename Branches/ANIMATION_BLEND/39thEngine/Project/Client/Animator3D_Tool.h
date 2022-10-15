#pragma once
#include "UI.h"
#include "AnimSequence.h"
class CAnimator3D;

class Animator3D_Tool final : public UI
{
private:
	CAnimator3D*               m_pAnimator;
	AnimSequence               m_animSequence;
	std::vector<CAnimation3D*> m_vecPlayList;
private:
	void RenderCurrentAnimInfo();
	void RenderPopUp();
	void RenderPlayList();
	void RenderTimeLine();

	void Calculate(int (&arrIndex)[2], float (&arrFloat)[2], bool byIndex);

	template <typename T>
	T Clamp(T val, T min, T max);
public:
	void render_update() override;

	void SetAnimator3D(CAnimator3D* pAnimator);
public:
	Animator3D_Tool();
	virtual ~Animator3D_Tool();
};

template <typename T>
T Animator3D_Tool::Clamp(T val, T min, T max)
{
	if (val <= min)
	{
		val = min;
	}
	if (val >= max)
	{
		val = max;
	}
	return val;
}
