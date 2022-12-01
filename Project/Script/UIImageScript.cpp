#include "pch.h"
#include "UIImageScript.h"

UIImageScript::UIImageScript()
	: CUIBase{SCRIPT_TYPE::UIIMAGESCRIPT}
	, m_currentFrameName{"NO DATA"}
	, m_useInfo{0}
	, m_preserveRatio{false}
	, m_ratio{1.f}
{
	AddScriptParamAsCheckBox("USE SPRITE", &m_useInfo, [this]() { SetUseInfo(m_useInfo); });
	AddScriptParamAsCheckBox("PRESERVE RATIO", (int*)&m_preserveRatio);
	AddScriptParam("TEXTURE NAME", SCRIPTPARAM_TYPE::TEXTURE, &m_textureName);
}

UIImageScript::UIImageScript(const UIImageScript& _origin)
	: CUIBase{_origin}
	, m_currentInfo{_origin.m_currentInfo}
	, m_currentFrameName{_origin.m_currentFrameName}
	, m_useInfo{0}
	, m_preserveRatio{false}
	, m_ratio{1.f}
	, m_textureName{_origin.m_textureName}
{
	RenewScalarParam("USE SPRITE", &m_useInfo, [this]() { SetUseInfo(m_useInfo); });
	RenewScalarParam("PRESERVE RATIO", &m_preserveRatio);
	RenewScalarParam("TEXTURE NAME", &m_textureName);
}

UIImageScript::~UIImageScript() = default;

void UIImageScript::SetUseInfo(bool enable)
{
	if (enable)
	{
		AddScriptParam("CURRENT FRAME NAME", SCRIPTPARAM_TYPE::TEXT_READONLY, &m_currentFrameName);
		AddScriptParam("CURRENT FRAME POS/SIZE",
		               SCRIPTPARAM_TYPE::VEC4,
		               &m_currentInfo,
		               [this]()
		               {
			               const auto iter = FindImageInfo(ToWString(m_currentFrameName));
			               iter->second    = m_currentInfo;
		               });
	}
	else
	{
		RemoveScalarParam("CURRENT FRAME NAME");
		RemoveScalarParam("CURRENT FRAME POS/SIZE");
	}
}

void UIImageScript::update()
{
	if (m_preserveRatio)
	{
		if (m_useInfo && m_mapInfo.size() > 0)
		{
			m_ratio           = m_currentInfo.imageSize.x / m_currentInfo.imageSize.y;
			Vec3 currentScale = Transform()->GetRelativeScale();
			currentScale.y    = currentScale.x / m_ratio;
			Transform()->SetRelativeScale(currentScale);
		}
		else
		{
			Ptr<CTexture> pTexture = MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
			if (nullptr != pTexture)
			{
				m_ratio           = pTexture->Width() / pTexture->Height();
				Vec3 currentScale = Transform()->GetRelativeScale();
				currentScale.y    = currentScale.x / m_ratio;
				Transform()->SetRelativeScale(currentScale);
			}
		}
	}
	CUIBase::update();
}

void UIImageScript::lateupdate()
{
	if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial(0) || nullptr == MeshRender()->GetMesh())
	{
		return;
	}
	CUIBase::lateupdate();

	const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(ToWString(m_textureName));
	if (nullptr != pTexture)
	{
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	}
	if (m_useInfo)
	{
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &m_useInfo);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_currentInfo);
	}
}

void UIImageScript::Clear()
{
	m_mapInfo.clear();
	m_useInfo = false;
}

void UIImageScript::AddImageInfo(const std::wstring& frameName, const Vec2& pos, const Vec2& size)
{
	const auto iter = FindImageInfo(frameName);
	assert(iter == m_mapInfo.end());
	m_mapInfo.insert({frameName, tImageInfo{pos, size}});
}

tImageInfo UIImageScript::GetImageInfo(const std::wstring& frameName)
{
	const auto iter = FindImageInfo(frameName);
	assert(iter != m_mapInfo.end());
	return iter->second;
}

void UIImageScript::SetImageInfo(const std::wstring& frameName)
{
	m_currentInfo      = GetImageInfo(frameName);
	m_currentFrameName = ToString(frameName);
	m_useInfo          = true;
}

void UIImageScript::SetPreserveRatio(bool enable)
{
	m_preserveRatio = enable;
}

void UIImageScript::Serialize(YAML::Emitter& emitter)
{
	CUIBase::Serialize(emitter);
	emitter << YAML::Key << "IMAGE INFO LIST" << YAML::Value << YAML::BeginMap;
	for (auto& [infoName, info] : m_mapInfo)
	{
		emitter << YAML::Key << ToString(infoName) << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << NAME_OF(info.imagePos) << YAML::Value << info.imagePos;
		emitter << YAML::Key << NAME_OF(info.imageSize) << YAML::Value << info.imageSize;
		emitter << YAML::EndMap;
	}
	emitter << YAML::EndMap;

	emitter << YAML::Key << NAME_OF(m_useInfo) << YAML::Value << m_useInfo;
	emitter << YAML::Key << NAME_OF(m_currentFrameName) << YAML::Value << m_currentFrameName;
	emitter << YAML::Key << NAME_OF(m_currentInfo.imagePos) << YAML::Value << m_currentInfo.imagePos;
	emitter << YAML::Key << NAME_OF(m_currentInfo.imageSize) << YAML::Value << m_currentInfo.imageSize;
	emitter << YAML::Key << NAME_OF(m_preserveRatio) << YAML::Value << m_preserveRatio;
	emitter << YAML::Key << NAME_OF(m_textureName) << YAML::Value << m_textureName;
}

void UIImageScript::Deserialize(const YAML::Node& node)
{
	CUIBase::Deserialize(node);
	for (auto& infoNameNode : node["IMAGE INFO LIST"])
	{
		tImageInfo         info{};
		const std::wstring infoName = ToWString(infoNameNode.first.as<std::string>());
		for (auto& infoNode : infoNameNode.second)
		{
			info.imagePos  = infoNode[NAME_OF(info.imagePos)].as<Vec2>();
			info.imageSize = infoNode[NAME_OF(info.imageSize)].as<Vec2>();
		}
		m_mapInfo.insert({infoName, info});
	}
	m_useInfo               = node[NAME_OF(m_useInfo)].as<int>();
	m_currentFrameName      = node[NAME_OF(m_currentFrameName)].as<std::string>();
	m_currentInfo.imagePos  = node[NAME_OF(m_currentInfo.imagePos)].as<Vec2>();
	m_currentInfo.imageSize = node[NAME_OF(m_currentInfo.imageSize)].as<Vec2>();
	m_preserveRatio         = node[NAME_OF(m_preserveRatio)].as<bool>();
	m_textureName           = node[NAME_OF(m_textureName)].as<std::string>();
	RenewScalarParam("TEXTURE NAME", &m_textureName);
	SetUseInfo(m_useInfo);
}
