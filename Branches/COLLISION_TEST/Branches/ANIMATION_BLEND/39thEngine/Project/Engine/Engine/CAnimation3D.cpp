#include "pch.h"
#include "CAnimation3D.h"

#include "CAnimation3DShader.h"
#include "CAnimator3D.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"

CAnimation3D::CAnimation3D()
	:
	m_pOwner{nullptr}
  , m_lerpTime{0}
  , m_clip{}
  , m_currentFrameIndex{0}
  , m_nextFrameIndex{0}
  , m_ratio{0}
  , m_accTime{0}
  , m_bFinalMatUpdate{false}
  , m_isFinish{false}
  , m_isPaused{false} {}

CAnimation3D::~CAnimation3D() {}

void CAnimation3D::finalupdate()
{
	if (false == m_isPaused)
	{
		m_accTime += DT;
	}

	//�ش� �ִϸ��̼��� ��� �����Ͽ��ٸ�
	static float accInterpolationTime = 0;
	if (m_accTime >= m_clip.dTimeLength)
	{
		accInterpolationTime += DT;
		if (accInterpolationTime >= m_lerpTime)
		{
			accInterpolationTime = 0.f;
			m_isFinish           = true;
		}
	}
	//��� �������� �ʾҴٸ�
	if (false == m_isFinish)
	{
		// ���� ������ �ε��� ���ϱ�
		const double frameIndex = (m_accTime + m_clip.dStartTime) * static_cast<double>(m_pOwner->GetLeastFrameCount());
		m_currentFrameIndex     = static_cast<int>(frameIndex);

		//���� ������ �ε���
		const size_t lastFrameIndex = m_clip.iFrameLength - 1 + m_clip.iStartFrame;
		if (m_currentFrameIndex >= lastFrameIndex)
		{
			m_nextFrameIndex = m_currentFrameIndex; // ���̸� ���� �ε����� ����
			//���� ���� �ִϸ��̼��� �����Ѵٸ�
			const auto& pNextAnim = m_pOwner->GetNextAnim();
			if (pNextAnim)
			{
				m_nextFrameIndex = pNextAnim->m_currentFrameIndex;
				pNextAnim->m_currentFrameIndex++;
				pNextAnim->m_accTime += DT;
			}
		}
		else
		{
			m_nextFrameIndex = m_currentFrameIndex + 1;
		}

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		if (nullptr != m_pOwner->GetNextAnim() && m_accTime >= m_clip.dTimeLength)
		{
			m_ratio = accInterpolationTime / m_lerpTime;
		}
		else
		{
			m_ratio = static_cast<float>(frameIndex - static_cast<float>(m_currentFrameIndex));
		}

		// ��ǻƮ ���̴� ���꿩��
		m_bFinalMatUpdate = false;
	}
}

void CAnimation3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		auto pComputeShader = CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateShader");

		CAnimation3DShader* pUpdateShader = static_cast<CAnimation3DShader*>(pComputeShader.Get());

		// Bone Data
		Ptr<CMesh> pMesh = m_pOwner->MeshRender()->GetMesh();
		m_pOwner->check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pOwner->GetFinalBoneMat());

		const UINT iBoneCount = static_cast<UINT>(m_pOwner->GetBoneCount());
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_currentFrameIndex);
		pUpdateShader->SetNextFrameIdx(m_nextFrameIndex);
		pUpdateShader->SetFrameRatio(m_ratio);

		// ������Ʈ ���̴� ����
		pUpdateShader->Excute();

		m_bFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_pOwner->GetFinalBoneMat()->UpdateData(PIPELINE_STAGE::VS, 30);
}

void CAnimation3D::Create(CAnimator3D* pOwner, const tMTAnimClip& clip)
{
	m_pOwner = pOwner;
	m_clip   = clip;
}

bool CAnimation3D::IsFinish()
{
	return m_isFinish;
}

void CAnimation3D::SetLerpTime(float time)
{
	m_lerpTime = time;
}

float CAnimation3D::GetLerpTime() const
{
	return m_lerpTime;
}

void CAnimation3D::Reset()
{
	m_accTime           = 0.f;
	m_isFinish          = false;
	m_isPaused          = false;
	m_currentFrameIndex = m_clip.iStartFrame;
}

void CAnimation3D::SetPause(bool pause)
{
	m_isPaused = pause;
}

bool CAnimation3D::IsPaused() const
{
	return m_isPaused;
}

void CAnimation3D::SetCurrentIndex(int index)
{
	if (index <= m_clip.iStartFrame)
	{
		index = m_clip.iStartFrame;
	}
	else if (index >= m_clip.iEndFrame)
	{
		index = m_clip.iEndFrame - 1;
	}
	m_currentFrameIndex = index;
	m_accTime           = static_cast<float>(m_clip.dTimeLength)
	                      * static_cast<float>(m_currentFrameIndex - m_clip.iStartFrame) / m_clip.iFrameLength;
	//accTime�� ���� ���� �������� �� �۴ٸ� ���� 
	const double frameIndex = static_cast<int>(m_accTime * static_cast<double>(m_pOwner->GetLeastFrameCount()));
	if (index > frameIndex)
	{
		m_accTime = static_cast<float>(m_clip.dTimeLength)
		            * static_cast<float>(m_currentFrameIndex + 1 - m_clip.iStartFrame) / m_clip.iFrameLength;
	}
}

void CAnimation3D::SetFrameTime(float time)
{
	if (time <= m_clip.dStartTime)
	{
		time = static_cast<float>(m_clip.dStartTime);
	}
	else if (time > m_clip.dEndTime)
	{
		time = static_cast<float>(m_clip.dEndTime);
	}
	m_accTime = time - static_cast<float>(m_clip.dStartTime);
}

int CAnimation3D::GetCurrentIndex() const
{
	const int frameIndex = m_currentFrameIndex;
	return frameIndex;
}
