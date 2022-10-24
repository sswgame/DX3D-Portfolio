#pragma once
#include "CEntity.h"

class CAnimator3D;

enum class ANIMATION_STATE
{
	BEFORE_PLAY,	// ��� ���� 
	PLAY,			// ��� 
	STOP,			// �Ͻ� ���� 
	FINISH,			// �� 
	END,
};


class CAnimation3D :
	public CEntity
{

private:
	CAnimator3D*			m_pOwner;
	tMTAnimClip				m_tClip;				// �ִϸ��̼� ����  

	ANIMATION_STATE			m_eCurState;			// ���� �ִϸ��̼� ���� 

	int						m_iFrameCnt;			// 30 ������ - Default 

	bool					m_bFinalMatUpdate;		// ������� ���� ���࿩��
	bool					m_bFinish;				// �ִϸ��̼� ���� ����
	bool					m_bPlay;				// �ִϸ��̼� ���� ���� 

	// [ CLIP INFO	] 
	int						m_iCurClip;				// Ŭ�� ��ȣ ( Idx )
	// [ FRAME INFO ]
	int						m_iPrevAnimEndFrameIdx; // ���� �ִϸ��̼� �� ������ ��ȣ
	int						m_iCurFrameIdx;			// ���� ������ ��ȣ
	int						m_iNextFrameIdx;		// ���� ������ ��ȣ 
	float					m_fSpeed;				// ������ �ӵ� 
	float					m_fRatio;				// ������ - ������ ���� �ð��� ���� ���� 
	// [ TIME INFO ]
	vector<double>			m_vecClipUpdateTime;	// �ִϸ��̼� �÷��� ���� �����ð� 
	double					m_dCurTime;				// ���� �ð� 
	float					m_fLerpTime;			// ���� �ð� 


public:
	void			finalupdate();
	virtual void	UpdateData() override;
	void			ClearData();


public:
	void Play(bool _b);

	void Reset();
	void ResetStartEndFrameIdx();
	void ResetStartEndFrameTime();
	void CopyInfo(CAnimation3D** _pCopyAnim); // Copy�� Animation�� ���������͸� �Ѱܶ� 



public:
	// [ SET PART ]
	void SetOwner(CAnimator3D* _owner) { m_pOwner = _owner; }
	void SetFrameInfo(int _ClipNum, double _startTime = (0.0), double _endTime = (0.0), int _startFrameIdx = 0, int _EndFrame = 0);
	void SetClipUpdateTime(int _vecClipSize);
	void SetPrevFrameEndIdx(int _idx) { m_iPrevAnimEndFrameIdx = _idx; }
	void CopyClipUpdateTime(vector<double> _vecClipUpdateTime) { m_vecClipUpdateTime = _vecClipUpdateTime; }



	// - SET CLIP INFO 
	void SetClipInfo(tMTAnimClip _tClip)	{ m_tClip = _tClip; }
	void SetClipNum(int _ClipNum)			{ m_iCurClip = _ClipNum; }
	void SetStartTime(double _startTime);
	void SetEndTime(double _endTime);
	void SetStartFrameIdx(int _startIdx);
	void SetEndFrameIdx(int _endIdx);
	void SetCurFrameIdx(int _curIdx);

	void SetSpeed(float _fSpeed)				{ m_fSpeed = _fSpeed; }
	void SetFinish(bool _b)						{ if (m_bFinish && !_b) m_vecClipUpdateTime[m_iCurClip] = 0.f; m_bFinish = _b; }
	void SetPlay(bool _b)						{ m_bPlay = _b; m_eCurState = ANIMATION_STATE::STOP; }
	void SetAnimState(ANIMATION_STATE _eState)	{ m_eCurState = _eState; }
	void SetLerpTime(float _fTime)				{ m_fLerpTime = _fTime; }

public:
	// [ GET PART ]
	bool			IsPlay()				{ return m_bPlay; }
	bool			IsFinish()				{ return m_bFinish; }
	int				GetClipNum()			{ return m_iCurClip; }
	float			GetSpeed()				{ return m_fSpeed; }
	ANIMATION_STATE	GetState()				{ return m_eCurState; }
	// - GET INDEX -
	int				GetCurFrameIdx()		{ return m_iCurFrameIdx; }
	int				GetStartFrameIdx()		{ return m_tClip.iStartFrame; }
	int				GetEndFrameIdx()		{ return m_tClip.iEndFrame; }
	int				GetMaxFrameIdx();


	// - GET TIME -
	float			GetLerpTime()			{ return m_fLerpTime; }
	double			GetStartTime()			{ return m_tClip.dStartTime; }
	double			GetEndTime()			{ return m_tClip.dEndTime; }
	double			GetAccTime()			{ return m_vecClipUpdateTime[m_iCurClip]; }


private:
	void check_mesh(Ptr<CMesh> _pMesh);


public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;
	CLONE(CAnimation3D)
public:
	CAnimation3D();
	CAnimation3D(const CAnimation3D& _origin);
	~CAnimation3D();

	friend class CAnimator3D;

};

