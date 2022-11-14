#pragma once
#include "CAnimation3D.h"
#include "CComponent.h"
#include "Ptr.h"


class CStructuredBuffer;

enum class CREATE_ANIMATION_MODE
{
	FRAME
	, TIME
	, END
	,
};

class CAnimator3D : public CComponent
{
private:
	// [ 3D MODEL INFO ]
	const vector<tMTBone>*     m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;
	int                        m_iFrameCount;			 // 30
	CStructuredBuffer*         m_pBoneFinalMatBuffer;   // 특정 프레임의 최종 행렬'

	//TODO:소켓 정보를 저장할 구조화 버퍼 및 들고온 본 행렬 정보를 저장할 벡터.
	CStructuredBuffer*  m_pSocketBuffer = nullptr;
	std::vector<Matrix> m_vecSocketMatrix;
private:
	// [ 3D ANIMATION INFO ]
	map<wstring, CAnimation3D*> m_mapAnim;               // 전체 애니메이션 

	CAnimation3D* m_pPrevAnim;             // 이전 애니메이션 
	CAnimation3D* m_pCurAnim;              // 현재 애니메이션 
	CAnimation3D* m_pNextAnim;             // 다음 애니메이션 

	bool m_bRepeat;               // 애니메이션 반복 여부 
	bool m_bPlayWithChild;        // 하위 객체 오브젝트와 동시에 재생 여부 

	CREATE_ANIMATION_MODE m_eCreateMode;			// 애니메이션 생성 모드 

public:
	virtual void finalupdate() override;
	virtual void UpdateData() override;
	void         ClearData();


public:
	CAnimation3D* FindAnim(const wstring& _strName);
	void          CopyAllAnim(const map<wstring, CAnimation3D*> _mapAnim);
	void          DeleteAnim(const wstring& _wstrName);
	CAnimation3D* CreateAnimByFrame(const wstring& _strName, int _clipNum = 0, int _startFrame = 0, int _EndFrame = 0);
	CAnimation3D* CreateAnimByTime(const wstring& _strName
	                               , int          _clipNum   = 0
	                               , double       _StartTime = 0.0
	                               , double       _EndTime   = 0.0);
	void Play(const wstring& _strName, bool _bRepeat = false);
	void Clear();

	void MakeAnimationFromTXT(string _txtName);
	void MakeAnimationFromTXT_Extended(string _txtName);
	void CopyAllAnimToChild();


public:
	// [ SET PART ]
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void SetFrameCount(int _iFrameCnt) { m_iFrameCount = _iFrameCnt; }

	void SetPlayWithChild(bool _bPlayWithChild);
	void SetAnimState(wstring _Name, ANIMATION_STATE _eState);
	void SetSpeed(float _fSpeed);
	void SetSpeedOnAnim(wstring _AnimName, float _fSpeed);
	void SetLerpTime(float _fTime);
	void SetRepeat(bool _b);
	void SetLerpTimeOnAllAnim(float _fLerpTime);
	void SetLerpTimeOnAnim(wstring _AnimName, float _fLerpTime);
	void SetCreateMode(CREATE_ANIMATION_MODE _eMode) { m_eCreateMode = _eMode; }


public:
	// [ GET PART ]
	const map<wstring, CAnimation3D*> GetAllAnim() { return m_mapAnim; }  // 전체 애니메이션을 받는다. 

	UINT        GetBoneCount() { return (UINT)m_pVecBones->size(); }
	tMTAnimClip GetAnimClip(int _clipIdx) { return m_pVecClip->at(_clipIdx); }
	bool        GetRepeat() { return m_bRepeat; }
	bool        GetPlayWithChild() { return m_bPlayWithChild; }

	CAnimation3D* GetPrevAnim() { return m_pPrevAnim; }
	CAnimation3D* GetCurAnim() { return m_pCurAnim; }
	CAnimation3D* GetNextAnim() { return m_pNextAnim; }

	int                GetFrameCount() { return m_iFrameCount; }
	CStructuredBuffer* GetBoneFinalMatBuffer() { return m_pBoneFinalMatBuffer; }

	CREATE_ANIMATION_MODE GetCreateMode() { return m_eCreateMode; }

	//TODO:Animation3D에 넘겨주기 위한 Get
	CStructuredBuffer* GetSocketBuffer() const { return m_pSocketBuffer; }
	//TODO:Script에서 접근하는 본 행렬 정보 매트릭스
	const std::vector<Matrix>& GetSocketMatrix() const { return m_vecSocketMatrix; }
	//TODO: 만약에 본 개수가 달라질 경우, 그에 대응하는 소켓 행렬들과 버퍼 크기를 재조정하기 위한 함수
	void ResizeSocketMatrix()
	{
		m_pSocketBuffer->Create(sizeof(Matrix), GetBoneCount(), SB_TYPE::READ_WRITE, true, nullptr);
		m_vecSocketMatrix.clear();
		m_vecSocketMatrix.shrink_to_fit();
		m_vecSocketMatrix.resize(GetBoneCount());
	}

public:
	void RegisterPrevAnim(CAnimation3D* _pPrevAnim) { m_pPrevAnim = _pPrevAnim; }
	void RegisterNextAnim(CAnimation3D* _pNextAnim);

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CLONE(CAnimator3D);
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	~CAnimator3D();
};
