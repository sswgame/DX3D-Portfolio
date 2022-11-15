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
	CStructuredBuffer*         m_pBoneFinalMatBuffer;   // Ư�� �������� ���� ���'

	//TODO:���� ������ ������ ����ȭ ���� �� ���� �� ��� ������ ������ ����.
	CStructuredBuffer*  m_pSocketBuffer = nullptr;
	std::vector<Matrix> m_vecSocketMatrix;
	bool                m_bUseSocket = false;

private:
	// [ 3D ANIMATION INFO ]
	map<wstring, CAnimation3D*> m_mapAnim;               // ��ü �ִϸ��̼� 

	CAnimation3D* m_pPrevAnim;             // ���� �ִϸ��̼� 
	CAnimation3D* m_pCurAnim;              // ���� �ִϸ��̼� 
	CAnimation3D* m_pNextAnim;             // ���� �ִϸ��̼� 

	bool m_bRepeat;               // �ִϸ��̼� �ݺ� ���� 
	bool m_bPlayWithChild;        // ���� ��ü ������Ʈ�� ���ÿ� ��� ���� 

	CREATE_ANIMATION_MODE m_eCreateMode;			// �ִϸ��̼� ���� ��� 

public:
	void finalupdate() override;
	void UpdateData() override;
	void ClearData();


public:
	CAnimation3D* FindAnim(const wstring& _strName);
	void          CopyAllAnim(map<wstring, CAnimation3D*> _mapAnim);
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
	const map<wstring, CAnimation3D*> GetAllAnim() { return m_mapAnim; }  // ��ü �ִϸ��̼��� �޴´�. 

	UINT        GetBoneCount() { return static_cast<UINT>(m_pVecBones->size()); }
	tMTAnimClip GetAnimClip(int _clipIdx) { return m_pVecClip->at(_clipIdx); }
	bool        GetRepeat() { return m_bRepeat; }
	bool        GetPlayWithChild() { return m_bPlayWithChild; }

	CAnimation3D* GetPrevAnim() { return m_pPrevAnim; }
	CAnimation3D* GetCurAnim() { return m_pCurAnim; }
	CAnimation3D* GetNextAnim() { return m_pNextAnim; }

	int                GetFrameCount() { return m_iFrameCount; }
	CStructuredBuffer* GetBoneFinalMatBuffer() { return m_pBoneFinalMatBuffer; }

	CREATE_ANIMATION_MODE GetCreateMode() { return m_eCreateMode; }

	//TODO:Animation3D�� �Ѱ��ֱ� ���� Get
	CStructuredBuffer* GetSocketBuffer() const { return m_pSocketBuffer; }
	//TODO: ���࿡ �� ������ �޶��� ���, �׿� �����ϴ� ���� ��ĵ�� ���� ũ�⸦ �������ϱ� ���� �Լ�
	void ResizeSocketMatrix()
	{
		m_pSocketBuffer->Create(sizeof(Matrix), GetBoneCount(), SB_TYPE::READ_WRITE, true, nullptr);
		m_vecSocketMatrix.clear();
		m_vecSocketMatrix.shrink_to_fit();
		m_vecSocketMatrix.resize(GetBoneCount());
	}

	void DisableSocket() { m_bUseSocket = false; }

	const Matrix& GetSocket(int index)
	{
		index        = ClampData(index, 0, (int)m_vecSocketMatrix.size());
		m_bUseSocket = true;

		return m_vecSocketMatrix[index];
	}

public:
	void RegisterPrevAnim(CAnimation3D* _pPrevAnim) { m_pPrevAnim = _pPrevAnim; }
	void RegisterNextAnim(CAnimation3D* _pNextAnim);

private:
public:
	template <typename T, typename...Args>
	void AddCallback(const std::wstring& animationName
	                 , int               frameIndex
	                 , T*                pInstance
	                 , void (T::*        callback)(Args ...)
	                 , Args ...          args)
	{
		auto arguments = std::make_tuple(pInstance, std::forward<decltype(args)>(args)...);
		auto func      = [callback,arguments = std::move(arguments)]()
		{
			std::apply(callback, arguments);
		};

		CAnimation3D* pAnimation = FindAnim(animationName);
		assert(pAnimation && "ANIMAITION NOT FOUND");

		frameIndex = ClampData(frameIndex, pAnimation->GetStartFrameIdx(), pAnimation->GetEndFrameIdx());
		pAnimation->SetCallback(std::move(func), frameIndex);
	}

public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CLONE(CAnimator3D);
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	virtual ~CAnimator3D();
};
