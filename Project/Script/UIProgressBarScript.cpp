#include "pch.h"
#include "UIProgressBarScript.h"

#include "UIImageScript.h"


UIProgressBarScript::UIProgressBarScript()
	: CScript{(int)SCRIPT_TYPE::UIPROGRESSBARSCRIPT}
	, m_percentage{1.f}
	, m_dir{PROGRESS_DIR::RIGHT_TO_LEFT}
	, m_pImageScript{nullptr}
{
	AddScriptParam("TEXTURE NAME",
	               SCRIPTPARAM_TYPE::TEXTURE,
	               &m_textureName,
	               [this]() { this->SetTexture(m_textureName); });
	AddScriptParam("PERCENT", SCRIPTPARAM_TYPE::FLOAT, &m_percentage);
	AddScriptParamAsDropDown("DIRECTION",
	                         SCRIPTPARAM_TYPE::INT,
	                         &m_dir,
	                         {
		                         "RIGHT_TO_LEFT",
		                         "LEFT_TO_RIGHT",
		                         "TOP_TO_BOTTOM",
		                         "BOTTOM_TO_TOP"
	                         });
}

UIProgressBarScript::UIProgressBarScript(const UIProgressBarScript& _origin)
	: CScript{_origin}
	, m_percentage{_origin.m_percentage}
	, m_dir{_origin.m_dir}
	, m_pImageScript{nullptr}
	, m_textureName{_origin.m_textureName}
{
	RenewScalarParam("TEXTURE NAME", &m_textureName);
	RenewScalarParam("PERCENT", &m_percentage);
	RenewScalarParam("DIRECTION", &m_dir);
}

UIProgressBarScript::~UIProgressBarScript() = default;

void UIProgressBarScript::SetPercent(float value)
{
	m_percentage = ClampData(value, 0.f, 1.f);
}

void UIProgressBarScript::SetTexture(const std::string& texturePath)
{
	m_textureName = texturePath;
}

void UIProgressBarScript::start()
{
	m_pImageScript = GetOwner()->GetScript<UIImageScript>();
}

void UIProgressBarScript::lateupdate()
{
	if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial(0) || nullptr == MeshRender()->GetMesh())
	{
		return;
	}

	if (nullptr != m_pImageScript)
	{
		const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(ToWString(m_textureName));
		if (nullptr != pTexture)
		{
			MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
		}

		int usePercentage = true;
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &usePercentage);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_3, &m_dir);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_percentage);
	}
}

void UIProgressBarScript::Serialize(YAML::Emitter& emitter)
{
	CScript::Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_dir) << YAML::Value << static_cast<int>(m_dir);
	emitter << YAML::Key << NAME_OF(m_textureName) << m_textureName;
}

void UIProgressBarScript::Deserialize(const YAML::Node& node)
{
	CScript::Deserialize(node);
	m_dir         = (PROGRESS_DIR)node[NAME_OF(m_dir)].as<int>();
	m_textureName = node[NAME_OF(m_textureName)].as<std::string>();
	RemoveScalarParam("TEXTURE NAME");
	RenewScalarParam("TEXTURE NAME", &m_textureName);
}
