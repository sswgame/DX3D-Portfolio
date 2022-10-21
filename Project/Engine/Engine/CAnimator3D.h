#pragma once
#include "CComponent.h"

#include "Ptr.h"


class CStructuredBuffer;
class CAnimation3D;

class CAnimator3D :
    public CComponent
{
private:
    const vector<tMTBone>*      m_pVecBones;
    const vector<tMTAnimClip>*  m_pVecClip;
    int						    m_iFrameCount;			// 30

private:
    map<wstring, CAnimation3D*> m_mapAnim;               // ��ü �ִϸ��̼� 

    CAnimation3D*               m_pPrevAnim;             // ���� �ִϸ��̼� 
    CAnimation3D*               m_pCurAnim;              // ���� �ִϸ��̼� 
    CAnimation3D*               m_pNextAnim;             // ���� �ִϸ��̼� 

    bool                        m_bRepeat;               // �ִϸ��̼� �ݺ� ���� 

public:
    virtual void    finalupdate() override;
    virtual void    UpdateData() override;
    void            ClearData();


public:
    CAnimation3D* FindAnim(const wstring& _strName);
    void          DeleteAnim(const wstring& _wstrName);
    void          CreateAnim(   const  wstring& _strName,
                                int     _clipNum = 0,
                                double  _startTime = (0.0),
                                double  _endTime = (0.0),
                                int     _startFrame = 0,
                                int     _EndFrame = 0);
    void          Play(const wstring& _strName, bool _bRepeat = false);
    void          Clear();

public:
    // [ SET PART ]
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
    void SetRepeat(bool _b) { m_bRepeat = _b; }
    void SetFrameCount(int _iFrameCnt) { m_iFrameCount = _iFrameCnt; }
    void SetLerpTimeOnAllAnim(float _fLerpTime);

public:
    // [ GET PART ]
    const map<wstring, CAnimation3D*>   GetAllAnim() { return m_mapAnim; }  // ��ü �ִϸ��̼��� �޴´�. 

    UINT            GetBoneCount()              { return (UINT)m_pVecBones->size(); }
    tMTAnimClip     GetAnimClip(int _clipIdx)   { return m_pVecClip->at(_clipIdx); }
    bool            GetRepeat()                 { return m_bRepeat; }

    CAnimation3D* GetPrevAnim() { return m_pPrevAnim; }
    CAnimation3D* GetCurAnim()  { return m_pCurAnim; }
    CAnimation3D* GetNextAnim() { return m_pNextAnim; }

    int GetFrameCount() { return m_iFrameCount; }


public:
    void RegisterPrevAnim(CAnimation3D* _pPrevAnim) { m_pPrevAnim = _pPrevAnim; }
    void RegisterNextAnim(CAnimation3D* _pNextAnim);

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};

