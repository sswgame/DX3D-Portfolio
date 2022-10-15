#pragma once
#include "CEntity.h"

class CAnimator3D;

class CAnimation3D : public CEntity
{
private:
	CAnimator3D* m_pOwner;
	float        m_lerpTime;

	tMTAnimClip m_clip;

	int   m_currentFrameIndex;
	int   m_nextFrameIndex;
	float m_ratio;
	float m_accTime;
	bool  m_bFinalMatUpdate; // 최종행렬 연산 수행여부
	bool  m_isFinish;

	bool m_isPaused;

private:
	void Create(CAnimator3D* pOwner, const tMTAnimClip& clip);
public:
	void finalupdate();
	void UpdateData() override;
public:
	bool IsFinish();
	void Reset();

	void  SetLerpTime(float time);
	float GetLerpTime() const;

	void SetPause(bool pause);
	bool IsPaused() const;
	void SetCurrentIndex(int index);
	void SetFrameTime(float time);

	tMTAnimClip&       GetAnimInfo() { return m_clip; }
	const tMTAnimClip& GetAnimInfo() const { return m_clip; }

	const std::wstring& GetName() const { return m_clip.strAnimName; }
	int                 GetCurrentIndex() const;

	CAnimator3D* GetOwner() const { return m_pOwner; }
public:
	CLONE(CAnimation3D);
	CAnimation3D();
	virtual ~CAnimation3D();

	friend class CAnimator3D;
};
