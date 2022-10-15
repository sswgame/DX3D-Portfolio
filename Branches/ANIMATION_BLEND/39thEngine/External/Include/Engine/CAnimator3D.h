#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CAnimation3D;
class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
	std::map<std::wstring, CAnimation3D*> m_mapAnimation;
	CAnimation3D*                         m_pCurrentAnimation;
	CAnimation3D*                         m_pNextAnimation;
	bool                                  m_repeat;

private:
	const vector<tMTBone>*     m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>  m_vecClipUpdateTime;
	vector<Matrix> m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int            m_iFrameCount;     // 30
	double         m_currentTime;
	int            m_iCurClip; // 클립 인덱스	

	int   m_currentFrameIndex; // 클립의 현재 프레임
	int   m_nextFrameIndex;    // 클립의 다음 프레임
	float m_fRatio;            // 프레임 사이 비율

	CStructuredBuffer* m_pBoneFinalMatBuffer; // 특정 프레임의 최종 행렬

public:
	void finalupdate() override;
	void UpdateData() override;

public:
	void SetBones(const vector<tMTBone>* _vecBones)
	{
		m_pVecBones = _vecBones;
		m_vecFinalBoneMat.resize(m_pVecBones->size());
	}

	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
	void               ClearData();

public:
	size_t GetBoneCount() const;
	int    GetLeastFrameCount() const { return m_iFrameCount; }
	float  GetRatio() const { return m_fRatio; }

	CAnimation3D* FindAnim(const std::wstring& animationName);
	CAnimation3D* GetCurrentAnim() const { return m_pCurrentAnimation; }

	void          SetNextAnim(CAnimation3D* pAnim) { m_pNextAnimation = pAnim; }
	CAnimation3D* GetNextAnim() const { return m_pNextAnimation; }

	void SetRepeat(bool repeat) { m_repeat = repeat; }
	bool IsRepeat() const { return m_repeat; }

	std::map<std::wstring, CAnimation3D*>& GetAnimList() { return m_mapAnimation; }

	void           Play(const std::wstring& animationName, bool repeat = false);
	FbxTime::EMode GetMode() const { return m_pVecClip->at(0).eMode; }
	int            GetMaxFrameCount() const;
	float          GetMaxTime() const;
	void           Create(const tMTAnimClip& animClip);

private:
	void check_mesh(Ptr<CMesh> _pMesh);

public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;
	CLONE(CAnimator3D);

public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	virtual ~CAnimator3D();

	friend class CAnimation3D;
};
