#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	:
	CComponent(COMPONENT_TYPE::LIGHT2D)
  , m_LightInfo{}
  , m_iLightIdx(-1) {}

CLight2D::~CLight2D() {}

void CLight2D::update() {}

void CLight2D::finalupdate()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vLightDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);
}

void CLight2D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);
}

void CLight2D::Serialize(YAML::Emitter& emitter)
{
	emitter << YAML::Key << "LIGHT INFO" << YAML::Value << YAML::Flow << YAML::BeginMap;
	emitter << YAML::Key << "DIFFUSE" << YAML::Value << m_LightInfo.color.vDiff;
	emitter << YAML::Key << "SPECULAR" << YAML::Value << m_LightInfo.color.vSpec;
	emitter << YAML::Key << "AMBIENT" << YAML::Value << m_LightInfo.color.vAmb;
	emitter << YAML::Key << "LIGHT DIR" << YAML::Value << m_LightInfo.vLightDir;
	emitter << YAML::Key << "LIGHT TYPE" << YAML::Value << m_LightInfo.iLightType;
	emitter << YAML::Key << "WORLD POS" << YAML::Value << m_LightInfo.vWorldPos;
	emitter << YAML::Key << "ANGLE" << YAML::Value << m_LightInfo.fAngle;
	emitter << YAML::Key << "RANGE" << YAML::Value << m_LightInfo.fRange;
	emitter << YAML::EndMap;
}

void CLight2D::Deserialize(const YAML::Node& node)
{
	YAML::Node lightInfoNode = node["LIGHT INFO"];

	m_LightInfo.color.vDiff = lightInfoNode["DIFFUSE"].as<Vec4>();
	m_LightInfo.color.vSpec = lightInfoNode["SPECULAR"].as<Vec4>();
	m_LightInfo.color.vAmb  = lightInfoNode["AMBIENT"].as<Vec4>();
	m_LightInfo.vLightDir   = lightInfoNode["LIGHT DIR"].as<Vec3>();
	m_LightInfo.iLightType  = lightInfoNode["LIGHT TYPE"].as<int>();
	m_LightInfo.vWorldPos   = lightInfoNode["WORLD POS"].as<Vec3>();
	m_LightInfo.fAngle      = lightInfoNode["ANGLE"].as<float>();
	m_LightInfo.fRange      = lightInfoNode["RANGE"].as<float>();
}
