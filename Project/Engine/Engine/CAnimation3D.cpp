#include "pch.h"
#include "CAnimation3D.h"
#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

CAnimation3D::CAnimation3D()
	: m_pOwner(nullptr)
	, m_tClip{}
	, m_eCurState{}
	, m_iFrameCnt(30)
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_bPlay(true)
	, m_iCurClip(0)
	, m_iPrevAnimEndFrameIdx{0}
	, m_iCurFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fSpeed(1.f)
	, m_fRatio(0.f)
	, m_dCurTime(0.f)
	, m_fLerpTime(0.5f)

{
}


CAnimation3D::CAnimation3D(const CAnimation3D& _origin)
	: m_pOwner(nullptr)
	, m_tClip{_origin.m_tClip}
	, m_eCurState{}
	, m_iFrameCnt{_origin.m_iFrameCnt}
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_bPlay{false}
	, m_iCurClip(_origin.m_iCurClip)
	, m_iPrevAnimEndFrameIdx{0}
	, m_iCurFrameIdx{0}
	, m_iNextFrameIdx{0}
	, m_fSpeed(_origin.m_fSpeed)
	, m_fRatio{0.f}
	, m_dCurTime{0.f}
	, m_fLerpTime(_origin.m_fLerpTime)
{
	m_vecClipUpdateTime.resize(1);
}

CAnimation3D::~CAnimation3D()
{
}


void CAnimation3D::finalupdate()
{
	if (nullptr == m_pOwner || true == m_bFinish)
		return;

	m_dCurTime = 0.f;
	switch (m_eCurState)
	{
	case ANIMATION_STATE::BEFORE_PLAY:
		{
			/*
			 [ (이전 애니메이션  끝	  프레임) ]
												-> 보간
			 [ (현재 애니메이션  시작 프레임) ]
			*/

			// 현재 재생중인 Clip 의 시간을 진행한다.

			m_vecClipUpdateTime[m_iCurClip] += DT * m_fSpeed;

			m_iCurFrameIdx  = m_iPrevAnimEndFrameIdx;
			m_iNextFrameIdx = m_tClip.iStartFrame + m_accTime * m_iFrameCnt;
			++m_temp;

			m_accTime += DT;
			m_fRatio = ClampData(m_accTime / m_fLerpTime, 0.f, 1.f);

			// 보간 끝! -> 메인 애니메이션을 출력하라. 
			if (m_accTime >= m_fLerpTime)
			{
				m_accTime   = 0.f;
				m_temp      = 0;
				m_eCurState = ANIMATION_STATE::PLAY;
			}
		}
		break;
	case ANIMATION_STATE::PLAY:
		{
			m_vecClipUpdateTime[m_iCurClip] += DT * m_fSpeed;

			if (m_tClip.dStartTime + m_vecClipUpdateTime[m_iCurClip] >= m_tClip.dEndTime)
			{
				m_eCurState = ANIMATION_STATE::FINISH;
				m_bFinish   = true;
				m_bPlay     = false;
				//m_vecClipUpdateTime[m_iCurClip] = 0.f;
			}

			m_dCurTime = m_tClip.dStartTime + m_vecClipUpdateTime[m_iCurClip];

			// 현재 프레임 인덱스 구하기
			int    iFrameCount = m_pOwner->GetFrameCount();
			double dFrameIdx   = m_dCurTime * (double)m_iFrameCnt;
			m_iCurFrameIdx     = (int)(dFrameIdx);

			// 다음 프레임 인덱스
			if (m_iCurFrameIdx >= m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1)
				m_iNextFrameIdx = m_iCurFrameIdx;	// 끝이면 현재 인덱스를 유지
			else
				m_iNextFrameIdx = m_iCurFrameIdx + 1;

			// 프레임간의 시간에 따른 비율을 구해준다.
			m_fRatio = (float)(dFrameIdx - (double)m_iCurFrameIdx);
		}
		break;
	case ANIMATION_STATE::STOP:
		{
			m_dCurTime = m_tClip.dStartTime + m_vecClipUpdateTime[m_iCurClip];

			double dFrameIdx = m_dCurTime * (double)m_iFrameCnt;
			m_iCurFrameIdx   = (int)(dFrameIdx);

			if (m_iCurFrameIdx >= m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1)
				m_iNextFrameIdx = m_iCurFrameIdx;
			else
				m_iNextFrameIdx = m_iCurFrameIdx + 1;

			m_fRatio = (float)(dFrameIdx - (double)m_iCurFrameIdx);
		}
		break;
	}

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnimation3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<
			CComputeShader>(L"Animation3DUpdateShader").Get();

		// Bone Data
		Ptr<CMesh> pMesh = m_pOwner->MeshRender()->GetMesh();
		check_mesh(pMesh);

		// Data Setting  
		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pOwner->GetBoneFinalMatBuffer());

		UINT iBoneCount = m_pOwner->GetBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iCurFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// Execute Update Shader 
		pUpdateShader->Excute();

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pOwner->GetBoneFinalMatBuffer()->UpdateData(PIPELINE_STAGE::VS, 30);
}

void CAnimation3D::ClearData()
{
	m_pOwner->GetBoneFinalMatBuffer()->Clear();

	UINT           iMtrlCount = m_pOwner->MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl      = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = m_pOwner->MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimation3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pOwner->GetBoneFinalMatBuffer()->GetElementCount() != iBoneCount)
	{
		m_pOwner->GetBoneFinalMatBuffer()->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimation3D::Play(bool _b)
{
	m_bPlay = _b;
	if (m_bPlay)
		m_eCurState = ANIMATION_STATE::PLAY;
	else
		m_eCurState = ANIMATION_STATE::STOP;
}

void CAnimation3D::Reset()
{
	m_eCurState            = ANIMATION_STATE::BEFORE_PLAY;
	m_iPrevAnimEndFrameIdx = m_tClip.iEndFrame;
	m_iCurFrameIdx         = m_iPrevAnimEndFrameIdx;

	m_bFinish                       = false;
	m_bPlay                         = true;
	m_vecClipUpdateTime[m_iCurClip] = 0.f;

	ResetStartEndFrameTime();
	ResetStartEndFrameIdx();
}

void CAnimation3D::ResetStartEndFrameIdx()
{
	int iFrameCnt = m_pOwner->GetFrameCount();
	// 시간 -> 인덱스 
	double dFrameIdx    = m_tClip.dStartTime * (double)m_iFrameCnt;
	m_tClip.iStartFrame = (int)(dFrameIdx);

	dFrameIdx         = m_tClip.dEndTime * (double)m_iFrameCnt;
	m_tClip.iEndFrame = (int)(dFrameIdx);
}

void CAnimation3D::ResetStartEndFrameTime()
{
	int iFrameCnt = m_pOwner->GetFrameCount();

	// 인덱스 -> 시간 
	m_tClip.dStartTime = (double)m_tClip.iStartFrame / (double)m_iFrameCnt;
	m_tClip.dEndTime   = (double)m_tClip.iEndFrame / (double)m_iFrameCnt;
}

void CAnimation3D::CopyInfo(CAnimation3D** _pCopyAnim)
{
	(*_pCopyAnim)->SetClipInfo(m_tClip);
	(*_pCopyAnim)->SetAnimState(m_eCurState);
	(*_pCopyAnim)->SetFinish(m_bFinish);
	(*_pCopyAnim)->SetPlay(m_bPlay);
	(*_pCopyAnim)->SetClipNum(m_iCurClip);
	(*_pCopyAnim)->SetPrevFrameEndIdx(m_iPrevAnimEndFrameIdx);
	(*_pCopyAnim)->SetCurFrameIdx(m_iCurFrameIdx);
	(*_pCopyAnim)->SetSpeed(m_fSpeed);
	(*_pCopyAnim)->SetLerpTime(m_fLerpTime);
	(*_pCopyAnim)->CopyClipUpdateTime(m_vecClipUpdateTime);
}

void CAnimation3D::SetFrameInfoByFrame(int   _ClipNum
                                       , int _startFrameIdx, int _EndFrameIdx)
{
	m_iCurClip = _ClipNum;

	SetStartFrameIdx(_startFrameIdx);
	SetEndFrameIdx(_EndFrameIdx);
	m_tClip.iFrameLength = _EndFrameIdx - _startFrameIdx + 1;
}

void CAnimation3D::SetFrameInfoByTime(int _ClipNum, double _StartTime, double _EndTime)
{
	m_iCurClip = _ClipNum;

	SetStartTime(_StartTime);
	SetEndTime(_EndTime);
	m_tClip.iFrameLength = m_tClip.iEndFrame - m_tClip.iStartFrame + 1;
}

void CAnimation3D::SetClipUpdateTime(int _vecClipSize)
{
	m_vecClipUpdateTime.resize(_vecClipSize);

	static float fTime = 0.f;
	//fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;
}

void CAnimation3D::SetStartTime(double _startTime)
{
	if (m_tClip.dStartTime == _startTime)
		return;
	if ((float)m_tClip.dStartTime == (float)_startTime)
		return;
	m_tClip.dStartTime = _startTime;

	if (_startTime <= 0.0)
		m_tClip.dStartTime = 0.0;
	if (_startTime >= m_pOwner->GetAnimClip(m_iCurClip).dTimeLength)
		m_tClip.dStartTime = m_pOwner->GetAnimClip(m_iCurClip).dTimeLength;

	// TIME 으로 INDEX 를 구한다. 
	m_tClip.iStartFrame = m_tClip.dStartTime * (double)m_iFrameCnt;
}

void CAnimation3D::SetEndTime(double _endTime)
{
	if (m_tClip.dEndTime == _endTime)
		return;
	if ((float)m_tClip.dEndTime == (float)_endTime)
		return;
	m_tClip.dEndTime = _endTime;

	if (_endTime <= 0.0)
		m_tClip.dEndTime = 0.0;
	if (_endTime >= m_pOwner->GetAnimClip(m_iCurClip).dTimeLength)
		m_tClip.dEndTime = m_pOwner->GetAnimClip(m_iCurClip).dTimeLength;

	// TIME 으로 INDEX 를 구한다. 
	m_tClip.iEndFrame = m_tClip.dEndTime * (double)m_iFrameCnt;
}

void CAnimation3D::SetStartFrameIdx(int _startIdx)
{
	if (m_tClip.iStartFrame == _startIdx)
		return;
	m_tClip.iStartFrame = _startIdx;

	if (_startIdx <= 0)
		m_tClip.iStartFrame = 0;
	if (_startIdx >= m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1)
		m_tClip.iStartFrame = m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1;

	// INDEX 로 TIME 을 구한다. 
	m_tClip.dStartTime = (double)m_tClip.iStartFrame / (double)m_iFrameCnt;
}

void CAnimation3D::SetEndFrameIdx(int _endIdx)
{
	if (m_tClip.iEndFrame == _endIdx)
		return;

	m_tClip.iEndFrame = _endIdx;

	if (_endIdx <= 0)
		m_tClip.iEndFrame = 0;
	if (_endIdx >= m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1)
		m_tClip.iEndFrame = m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1;

	// INDEX 로 TIME 을 구한다. 
	m_tClip.dEndTime = (double)m_tClip.iEndFrame / (double)m_iFrameCnt;
}

void CAnimation3D::SetCurFrameIdx(int _curIdx)
{
	if (m_eCurState == ANIMATION_STATE::STOP)
	{
		m_iCurFrameIdx                  = _curIdx;
		double FrameTime                = (double)m_iCurFrameIdx / (double)m_iFrameCnt;
		m_vecClipUpdateTime[m_iCurClip] = FrameTime - m_tClip.dStartTime;

		// ex)  244.999999997 -> 이 될때가 있는데 이러면
		//		245 로 예상했지만 244로 인덱스가 변경이 안될때가 있다. 
		double FrameErrorRange = 0.00001;
		m_vecClipUpdateTime[m_iCurClip] += FrameErrorRange;
	}
}

int CAnimation3D::GetMaxFrameIdx()
{
	return m_pOwner->GetAnimClip(m_iCurClip).iFrameLength - 1;
}

void CAnimation3D::SaveToScene(FILE* _pFile)
{
}

void CAnimation3D::LoadFromScene(FILE* _pFile)
{
}

void CAnimation3D::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << NAME_OF(m_iCurClip) << YAML::Value << m_iCurClip;
	emitter << YAML::Key << NAME_OF(m_fLerpTime) << YAML::Value << m_fLerpTime;
	emitter << YAML::Key << NAME_OF(m_fSpeed) << YAML::Value << m_fSpeed;

	emitter << YAML::Key << NAME_OF(m_tClip);
	emitter << YAML::Value << YAML::BeginMap;
	emitter << YAML::Key << NAME_OF(strAnimName) << YAML::Value << ToString(m_tClip.strAnimName);
	emitter << YAML::Key << NAME_OF(iStartFrame) << YAML::Value << m_tClip.iStartFrame;
	emitter << YAML::Key << NAME_OF(iEndFrame) << YAML::Value << m_tClip.iEndFrame;
	emitter << YAML::Key << NAME_OF(iFrameLength) << YAML::Value << m_tClip.iFrameLength;
	emitter << YAML::Key << NAME_OF(dStartTime) << YAML::Value << m_tClip.dStartTime;
	emitter << YAML::Key << NAME_OF(dEndTime) << YAML::Value << m_tClip.dEndTime;
	emitter << YAML::Key << NAME_OF(dTimeLength) << YAML::Value << m_tClip.dTimeLength;
	emitter << YAML::Key << NAME_OF(eMode) << YAML::Value << m_tClip.eMode;
	emitter << YAML::EndMap;
}

void CAnimation3D::Deserialize(const YAML::Node& node)
{
	m_iCurClip  = node[NAME_OF(m_iCurClip)].as<int>();
	m_fLerpTime = node[NAME_OF(m_fLerpTime)].as<float>();
	m_fSpeed    = node[NAME_OF(m_fSpeed)].as<float>();

	YAML::Node clipNode = node[NAME_OF(m_tClip)];

	m_tClip.strAnimName  = ToWString(clipNode[NAME_OF(strAnimName)].as<std::string>());
	m_tClip.iStartFrame  = clipNode[NAME_OF(iStartFrame)].as<int>();
	m_tClip.iEndFrame    = clipNode[NAME_OF(iEndFrame)].as<int>();
	m_tClip.iFrameLength = clipNode[NAME_OF(iFrameLength)].as<int>();
	m_tClip.dStartTime   = clipNode[NAME_OF(dStartTime)].as<double>();
	m_tClip.dEndTime     = clipNode[NAME_OF(dEndTime)].as<double>();
	m_tClip.dTimeLength  = clipNode[NAME_OF(dTimeLength)].as<double>();
	m_tClip.eMode        = (FbxTime::EMode)clipNode[NAME_OF(eMode)].as<int>();
	m_tClip.iFrameLength = m_tClip.iEndFrame - m_tClip.iStartFrame + 1;
	m_vecClipUpdateTime.resize(1);
}
