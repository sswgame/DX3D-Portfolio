#pragma once
#include "ImGuiGizumo/ImSequencer.h"
class CAnimation3D;
class CAnimator3D;

class AnimSequence : public ImSequencer::SequenceInterface
{
	friend class Animator3D_Tool;
public:
	AnimSequence();

public:
	void SetAnimator(CAnimator3D* pAnimator) { m_pAnimator = pAnimator; }
private:
	CAnimator3D*  m_pAnimator;
	bool          m_hadDelete;
	CAnimation3D* m_pDelete;
public:
	CAnimation3D* FindAnimWithIndex(int index) const;
	bool          HasDelete() const { return m_hadDelete; }

	void ClearDeleteState();

public:
	int GetFrameMin() const override;
	int GetFrameMax() const override;
	int GetItemCount() const override;

	const char* GetItemLabel(int index) const override;
	void        BeginEdit(int) override;
	void        EndEdit() override;
	void        Get(int index, int** start, int** end, int* type, unsigned int* color) override;
	void        Del(int index) override;
};
