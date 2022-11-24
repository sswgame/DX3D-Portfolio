#include "pch.h"
#include "UIProgressBarScript.h"

#include "UIImageScript.h"


UIProgressBarScript::UIProgressBarScript()
	: CScript{(int)SCRIPT_TYPE::UIPROGRESSBARSCRIPT}
	, m_percentage{1.f}
	, m_dir{PROGRESS_DIR::RIGHT_TO_LEFT}
	, m_pImageScript{nullptr}
{
}

UIProgressBarScript::~UIProgressBarScript() = default;

void UIProgressBarScript::SetPercent(float value)
{
	m_percentage = ClampData(value, 0.f, 1.f);
}

void UIProgressBarScript::start()
{
	m_pImageScript = GetOwner()->GetScript<UIImageScript>();
}

void UIProgressBarScript::lateupdate()
{
	const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(m_textureName);
	assert(nullptr != pTexture);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);

	if (nullptr != m_pImageScript)
	{
		int usePercentage = true;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &usePercentage);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_3, &m_dir);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_percentage);
	}
}
