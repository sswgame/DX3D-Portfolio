#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CTransform.h"
#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	:
	CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
  , m_iMaxCount(1000)
  , m_bPosInherit(0)
  , m_iAliveCount(4)
  , m_fMinLifeTime(0.5f)
  , m_fMaxLifeTime(2.f)
  , m_fStartSpeed(100.f)
  , m_fEndSpeed(10.f)
  , m_vStartColor(Vec4(1.f, 0.2f, 0.7f, 1.f))
  , m_vEndColor(Vec4(1.f, 1.f, 1.f, 1.f))
  , m_vStartScale(Vec3(10.f, 10.f, 1.f))
  , m_vEndScale(Vec3(1.f, 1.f, 1.f))
  , m_fParticleCreateDistance(50.f)
  , m_fParticleCreateTerm(0.02f)
  , m_fAccTime(0.f)
  , m_iEmissive(0)
  , m_iLighting(0)

{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"), 0);

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	:
	CRenderComponent(_origin)
  , m_iMaxCount(_origin.m_iMaxCount)
  , m_bPosInherit(_origin.m_bPosInherit)
  , m_iAliveCount(_origin.m_iAliveCount)
  , m_fMinLifeTime(_origin.m_fMinLifeTime)
  , m_fMaxLifeTime(_origin.m_fMaxLifeTime)
  , m_fStartSpeed(_origin.m_fStartSpeed)
  , m_fEndSpeed(_origin.m_fEndSpeed)
  , m_vStartColor(_origin.m_vStartColor)
  , m_vEndColor(_origin.m_vEndColor)
  , m_vStartScale(_origin.m_vStartScale)
  , m_vEndScale(_origin.m_vEndScale)
  , m_fParticleCreateDistance(_origin.m_fParticleCreateDistance)
  , m_fParticleCreateTerm(_origin.m_fParticleCreateTerm)
  , m_fAccTime(_origin.m_fAccTime)
  , m_iEmissive(_origin.m_iEmissive)
  , m_iLighting(_origin.m_iLighting)
  , m_vDirection{_origin.m_vDirection}
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"), 0);

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}


void CParticleSystem::SetEmissive(bool _bEmissive)
{
	m_iEmissive = (int)_bEmissive;
}

void CParticleSystem::SetApplyLighting(bool _bLighting)
{
	m_iLighting = (int)_bLighting;
}

void CParticleSystem::SetMaxParticleCount(UINT _iMax)
{
	if (m_iMaxCount < _iMax)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), _iMax, SB_TYPE::READ_WRITE, false, nullptr);
	}
	m_iMaxCount = _iMax;
}

void CParticleSystem::SetAliveCount(UINT _iCount)
{
	m_iAliveCount = ClampData(_iCount, 0u, m_iMaxCount);
}

void CParticleSystem::SetMinMaxLifeTime(float fMin, float fMax)
{
	m_fMinLifeTime = ClampData(fMin, 0.f,D3D11_FLOAT32_MAX);
	m_fMaxLifeTime = ClampData(fMax, 0.f, D3D11_FLOAT32_MAX);
}

void CParticleSystem::SetMinMaxSpeed(float fMin, float fMax)
{
	m_fStartSpeed = fMin;
	m_fEndSpeed   = fMax;
}

void CParticleSystem::SetStartEndColor(const Vec4& vStartColor, const Vec4& vEndColor)
{
	m_vStartColor = vStartColor;
	m_vEndColor   = vEndColor;
}

void CParticleSystem::SetStartEndScale(const Vec3& vStartScale, const Vec3& vEndScale)
{
	m_vStartScale = vStartScale;
	m_vEndScale   = vEndScale;
}

void CParticleSystem::SetRange(float _fRange)
{
	m_fParticleCreateDistance = ClampData(_fRange, 0.f, D3D11_FLOAT32_MAX);
}

void CParticleSystem::SetTerm(float _fTerm)
{
	m_fParticleCreateTerm = ClampData(_fTerm, 0.f, D3D11_FLOAT32_MAX);
}

void CParticleSystem::finalupdate()
{
	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;

		tParticleData data = {};
		data.iAliveCount   = m_iAliveCount;
		m_DataBuffer->SetData(&data, 1);
	}

	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);
	m_CS->SetDirection(m_vDirection);
	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());

	m_CS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &m_iLighting);
	GetMaterial(0)->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
}

void CParticleSystem::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	wstring strKey;
	if (nullptr != m_CS)
		strKey = m_CS->GetKey();
	SaveWStringToFile(strKey, _pFile);

	fwrite(&m_iMaxCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_bPosInherit, sizeof(int), 1, _pFile);
	fwrite(&m_iAliveCount, sizeof(int), 1, _pFile);
	fwrite(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fwrite(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
	fwrite(&m_vDirection, sizeof(Vec2), 1, _pFile);
	fwrite(&m_iEmissive, sizeof(int), 1, _pFile);
	fwrite(&m_iLighting, sizeof(int), 1, _pFile);
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	wstring strKey;
	LoadWStringFromFile(strKey, _pFile);
	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();

	UINT iMaxCount = 0;
	fread(&iMaxCount, sizeof(UINT), 1, _pFile);
	SetMaxParticleCount(iMaxCount);

	fread(&m_bPosInherit, sizeof(int), 1, _pFile);
	fread(&m_iAliveCount, sizeof(int), 1, _pFile);
	fread(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fread(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fread(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fread(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
	fread(&m_vDirection, sizeof(Vec2), 1, _pFile);
	fread(&m_iEmissive, sizeof(int), 1, _pFile);
	fread(&m_iLighting, sizeof(int), 1, _pFile);
}

void CParticleSystem::Serialize(YAML::Emitter& emitter)
{
	CRenderComponent::Serialize(emitter);
	CRes& shader = *m_CS.Get();
	emitter << YAML::Key << "SHADER" << YAML::Value << shader;

	emitter << YAML::Key << NAME_OF(m_iMaxCount) << YAML::Value << m_iMaxCount;
	emitter << YAML::Key << NAME_OF(m_bPosInherit) << YAML::Value << m_bPosInherit;
	emitter << YAML::Key << NAME_OF(m_iAliveCount) << YAML::Value << m_iAliveCount;
	emitter << YAML::Key << NAME_OF(m_fMinLifeTime) << YAML::Value << m_fMinLifeTime;
	emitter << YAML::Key << NAME_OF(m_fMaxLifeTime) << YAML::Value << m_fMaxLifeTime;
	emitter << YAML::Key << NAME_OF(m_fStartSpeed) << YAML::Value << m_fStartSpeed;
	emitter << YAML::Key << NAME_OF(m_fEndSpeed) << YAML::Value << m_fEndSpeed;
	emitter << YAML::Key << NAME_OF(m_vStartColor) << YAML::Value << m_vStartColor;
	emitter << YAML::Key << NAME_OF(m_vEndColor) << YAML::Value << m_vEndColor;
	emitter << YAML::Key << NAME_OF(m_fParticleCreateDistance) << YAML::Value << m_fParticleCreateDistance;
	emitter << YAML::Key << NAME_OF(m_fParticleCreateTerm) << YAML::Value << m_fParticleCreateTerm;
	emitter << YAML::Key << NAME_OF(m_vDirection) << YAML::Value << m_vDirection;
	emitter << YAML::Key << NAME_OF(m_iEmissive) << YAML::Value << m_iEmissive;
	emitter << YAML::Key << NAME_OF(m_iLighting) << YAML::Value << m_iLighting;
}

void CParticleSystem::Deserialize(const YAML::Node& node)
{
	CRenderComponent::Deserialize(node);
	auto keyValue = GetResourceInfo(node["SHADER"]);
	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(keyValue.first).Get();
	m_iMaxCount = node[NAME_OF(m_iMaxCount)].as<int>();
	m_bPosInherit = node[NAME_OF(m_bPosInherit)].as<int>();
	m_iAliveCount = node[NAME_OF(m_iAliveCount)].as<int>();
	m_fMinLifeTime = node[NAME_OF(m_fMinLifeTime)].as<float>();
	m_fMaxLifeTime = node[NAME_OF(m_fMaxLifeTime)].as<float>();
	m_fStartSpeed = node[NAME_OF(m_fStartSpeed)].as<float>();
	m_fEndSpeed = node[NAME_OF(m_fEndSpeed)].as<float>();
	m_vStartColor = node[NAME_OF(m_vStartColor)].as<Vec4>();
	m_vEndColor = node[NAME_OF(m_vEndColor)].as<Vec4>();
	m_fParticleCreateDistance = node[NAME_OF(m_fParticleCreateDistance)].as<float>();
	m_fParticleCreateTerm = node[NAME_OF(m_fParticleCreateTerm)].as<float>();
	m_vDirection = node[NAME_OF(m_vDirection)].as<Vec2>();
	m_iEmissive = node[NAME_OF(m_iEmissive)].as<int>();
	m_iLighting = node[NAME_OF(m_iLighting)].as<int>();
}
