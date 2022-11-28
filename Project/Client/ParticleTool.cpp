#include "pch.h"
#include "ParticleTool.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/define.h>
#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>

#include "ListUI.h"
#include "CImGuiMgr.h"
#include "ParticleSystemUI.h"

ParticleTool::ParticleTool()
	:UI("##ParticleToolUI")
	, m_pParticleTarget(nullptr)
	, m_pEmissiveTarget(nullptr)
	, m_vecParticleSystem()
	, m_pTargetParticle(nullptr)
	, m_strvecParticleName()
	, m_vecTargetResolution(0.f)
	, m_iMaxCount{ 5 }
	, m_bPosInherit{ false }
	, m_iAliveCount{ 1 }
	, m_fArrMinMaxTime{ {0.f}, {5.f} }
	, m_fArrMinMaxSpeed{ {10.f}, {100.f} }
	, m_fRange{ 10.f }
	, m_fTerm{ 3.f }
	, m_fAngle(180.f)
	, m_bShaderUseEmissive{ false }
	, m_bShaderUseSpeedDetail(false)
	, m_fArrDirection{ {1.f}, {0.f} }
{
}

ParticleTool::~ParticleTool()
{
}

void ParticleTool::ParticleOptionSetting()
{
	ImGui::Separator();
	ImGui::Text("PARTICLE OPTION SETTING");

	CParticleSystem* pTargetParticleSystem = m_pTargetParticle->ParticleSystem();

	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::BulletText("Pos Inherit");
	ImGui::SameLine(200);
	ImGui::Checkbox("##POS_INHERIT", &m_bPosInherit);
	pTargetParticleSystem->SetPosInherit(m_bPosInherit);

	ImGui::BulletText("Max Count");
	ImGui::SameLine(200);
	ImGui::InputInt("##MAX_COUNT", &m_iMaxCount);
	pTargetParticleSystem->SetMaxParticleCount(m_iMaxCount);

	ImGui::BulletText("Alive Count");
	ImGui::SameLine(200);
	ImGui::InputInt("##ALIVE_COUNT", &m_iAliveCount);
	pTargetParticleSystem->SetAliveCount(m_iAliveCount);

	ImGui::BulletText("MIN/MAX Time");
	ImGui::SameLine(200);
	ImGui::InputFloat2("##MIN_MAX_TIME", m_fArrMinMaxTime);
	pTargetParticleSystem->SetMinMaxLifeTime(m_fArrMinMaxTime[0], m_fArrMinMaxTime[1]);

	ImGui::BulletText("MIN/MAX Speed");
	ImGui::SameLine(200);
	ImGui::InputFloat2("##MIN_MAX_SPEED", m_fArrMinMaxSpeed);
	pTargetParticleSystem->SetMinMaxSpeed(m_fArrMinMaxSpeed[0], m_fArrMinMaxSpeed[1]);

	ImGui::BulletText("SPEED DETAIL");
	ImGui::SameLine();
	ImGui::Checkbox("##SHADER_USE_SPEED_DETAIL", &m_bShaderUseSpeedDetail);
	if (m_bShaderUseSpeedDetail)
	{
		static const char* s_arrSpeedDetailName[] = {
			"y = x",
			"y = log(2)(x)",
			"y = (2) ^ (x)",
		};

		int iSpeedDetailIndex = pTargetParticleSystem->GetSpeedDetailFunc();

		ImGui::SameLine(200);
		if (ImGui::BeginCombo("##PARTICLE_SPEED_DETAIL_COMBO", s_arrSpeedDetailName[iSpeedDetailIndex]))
		{
			for (size_t i = 0; i < std::size(s_arrSpeedDetailName); ++i)
			{
				if (ImGui::Selectable(s_arrSpeedDetailName[i]))
				{
					pTargetParticleSystem->SetSpeedDetailFunc(i);
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::BulletText("Start Color");
	ImGui::SameLine(200);
	ImGui::ColorEdit4("##START_COLOR", m_vArrStartEndColor[0]);
	ImGui::BulletText("End Color");
	ImGui::SameLine(200);
	ImGui::ColorEdit4("##END_COLOR", m_vArrStartEndColor[1]);
	pTargetParticleSystem->SetStartEndColor(m_vArrStartEndColor[0], m_vArrStartEndColor[1]);

	ImGui::BulletText("Start Scale");
	ImGui::SameLine(200);
	ImGui::InputFloat3("##START_SCALE", m_vArrStartEndScale[0]);
	ImGui::BulletText("End Scale");
	ImGui::SameLine(200);
	ImGui::InputFloat3("##END_SCALE", m_vArrStartEndScale[1]);
	pTargetParticleSystem->SetStartEndScale(m_vArrStartEndScale[0], m_vArrStartEndScale[1]);

	ImGui::BulletText("Creation Range");
	ImGui::SameLine(200);
	ImGui::InputFloat("##CREATION_RANGE", &m_fRange);
	pTargetParticleSystem->SetRange(m_fRange);

	ImGui::BulletText("Creation Term");
	ImGui::SameLine(200);
	ImGui::InputFloat("##CREATION_TERM", &m_fTerm);
	pTargetParticleSystem->SetTerm(m_fTerm);

	ImGui::BulletText("Direction");
	ImGui::Indent();
	ImGui::BulletText("Dir X  :"); 	ImGui::SameLine(200);
	ImGui::DragFloat("##PARTICLE_DIR_X", &m_fArrDirection[0], 0.1f, -1.0f, 1.0f, "%.1f");
	ImGui::BulletText("Dir Y  :"); 	ImGui::SameLine(200);
	ImGui::DragFloat("##PARTICLE_DIR_Y", &m_fArrDirection[1], 0.1f, -1.0f, 1.0f, "%.1f");
	ImGui::Unindent();

	Vec2 dirNormal = Vec2(m_fArrDirection[0], m_fArrDirection[1]);
	dirNormal.Normalize();
	m_fArrDirection[0] = dirNormal.x;
	m_fArrDirection[1] = dirNormal.y;
	pTargetParticleSystem->SetDirection(Vec2(m_fArrDirection[0], m_fArrDirection[1]));

	ImGui::BulletText("Angle");
	ImGui::SameLine((200));
	ImGui::DragFloat("##PARTICLE_ANGLE_FLOAT", &m_fAngle, 1.f, 0, 360, "%.1f");
	pTargetParticleSystem->SetAngle((m_fAngle));


	ImGui::BulletText("PARTICLE IMAGE");
	ImGui::SameLine(200);
	Ptr<CTexture> pTexture = pTargetParticleSystem->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
	if (nullptr != pTexture)
	{
		ImGui::BulletText(ToString(pTexture->GetKey()).c_str());
	}
	else
		ImGui::BulletText("NONE");
	ImGui::SameLine(200);
	if (ImGui::Button("##PARTICLE_TEXTURE_BUTTON", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const auto& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		ListUI* pListUI = static_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("##ListUI"));
		pListUI->Clear();
		pListUI->SetTitle("TEXTURE_LIST");

		for (const auto& pairData : mapRes)
		{
			pListUI->AddList(ToString(pairData.first));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&ParticleSystemUI::TextureSelect);
	}


}

void ParticleTool::SetData()
{
	CParticleSystem* pTargetParticleSystem = m_pTargetParticle->ParticleSystem();

	m_iMaxCount = pTargetParticleSystem->GetMaxParticleCount();
	m_bPosInherit = pTargetParticleSystem->PosInherit();
	m_iAliveCount = pTargetParticleSystem->GetAliveCount();

	const Vec2& vLifeTime = pTargetParticleSystem->GetMinMaxLifeTime();
	m_fArrMinMaxTime[0] = vLifeTime.x;
	m_fArrMinMaxTime[1] = vLifeTime.y;

	const Vec2& vMinMaxSpeed = pTargetParticleSystem->GetMinMaxSpeed();
	m_fArrMinMaxSpeed[0] = vMinMaxSpeed.x;
	m_fArrMinMaxSpeed[1] = vMinMaxSpeed.y;

	const std::pair<Vec4, Vec4>& pairColor = pTargetParticleSystem->GetStartEndColor();
	m_vArrStartEndColor[0] = pairColor.first;
	m_vArrStartEndColor[1] = pairColor.second;

	const std::pair<Vec3, Vec3>& pairScale = pTargetParticleSystem->GetStartEndScale();
	m_vArrStartEndScale[0] = pairScale.first;
	m_vArrStartEndScale[1] = pairScale.second;

	//const Vec2& vDirection = m_pParticleSystem->GetDirection();
	//m_iArrDirection[0]     = static_cast<int>(vDirection.x);
	//m_iArrDirection[1]     = static_cast<int>(vDirection.y);

	m_fAngle = pTargetParticleSystem->GetAngle();

	m_fRange = pTargetParticleSystem->GetRange();
	m_fTerm = pTargetParticleSystem->GetTerm();
}

void ParticleTool::MakeTargetParticleCombo()
{
	ImGui::Text("Change Target Particle : "); ImGui::SameLine();

	for (size_t i = 0; i < m_vecParticleSystem.size(); i++)
	{
		string particleName;
		particleName.assign(m_vecParticleSystem[i]->GetName().begin(), m_vecParticleSystem[i]->GetName().end());
		m_strvecParticleName.push_back(particleName);
	}

	static int item_current_idx = 0; 
	const char* combo_preview_value;
	if (0 == m_strvecParticleName.size())
	{
		combo_preview_value = "- Particle Is Not Existed-";

		ImGui::Text(combo_preview_value);
	}
	else
	{
		combo_preview_value = m_strvecParticleName[item_current_idx].c_str();

		if (ImGui::BeginCombo("##SetTarget_ParticleCombo", combo_preview_value, 0))
		{
			for (int n = 0; n < m_vecParticleSystem.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(m_strvecParticleName[n].c_str(), is_selected))
					item_current_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				m_pTargetParticle = m_vecParticleSystem[item_current_idx];
			}
			ImGui::EndCombo();
		}


		ParticleOptionSetting();
	}
}

void ParticleTool::update()
{
	if (nullptr == m_pParticleTarget || nullptr == m_pEmissiveTarget)
	{
		CMRT* pParticleMrt = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::PARTICLE);

		m_pParticleTarget = pParticleMrt->GetArrRt(0);
		m_pEmissiveTarget = pParticleMrt->GetArrRt(1);

		m_vecTargetResolution.x = m_pParticleTarget.Get()->Width();
		m_vecTargetResolution.y = m_pParticleTarget.Get()->Height();
	}

	// 현재 파티클을 vector안에다 넣어주어야함.
	// 일단 현재 씬에서 가져옴. 후에는...

	if (0 == m_vecParticleSystem.size())
	{
		vector<CGameObject*> pVecBGObj = CSceneMgr::GetInst()->GetCurScene()->GetLayer(0)->GetObjects();

		for (size_t i = 0; i < pVecBGObj.size(); i++)
		{
			if (nullptr != pVecBGObj[i]->GetComponent(COMPONENT_TYPE::PARTICLESYSTEM))
			{
				m_vecParticleSystem.push_back(pVecBGObj[i]);
			}
		}
	}

	if (0 != m_vecParticleSystem.size())
	{
		if (nullptr == m_pTargetParticle)
		{
			m_pTargetParticle = m_vecParticleSystem[0];
		}
	}


}

void ParticleTool::render_update()
{
	// 구현...

	// 필요한것
	// 1. particle target
	// 2. scene outliner 에 있는 정보들
	// 3. 저장하기
	// 4. 불러오기
	// 5. 파티클 여러개 저장하는 기능.

	SetData();

	ImVec2 pTargetImageSize;
	pTargetImageSize.x = m_vecTargetResolution.x * 2.f / 3.f;
	pTargetImageSize.y = m_vecTargetResolution.y * 2.f / 3.f;

	ImGui::Image(m_pParticleTarget.Get()->GetSRV().Get(), pTargetImageSize, ImVec2(0, 0)
	, ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));	

	if (nullptr == m_pTargetParticle)
	{
		return;
	}
	else
	{
		string particleName;
		particleName.assign(m_pTargetParticle->GetName().begin(), m_pTargetParticle->GetName().end());
		ImGui::Text("Cur Target Particle : "); ImGui::SameLine();
		ImGui::Text(particleName.c_str());
	}

	MakeTargetParticleCombo();

}
