#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem : public CRenderComponent
{
private:
	Ptr<CParticleUpdateShader> m_CS; // ��ƼŬ ������Ʈ ���̴�

	CStructuredBuffer* m_ParticleBuffer; // ��ƼŬ ����ȭ ����
	CStructuredBuffer* m_DataBuffer;     // �߰� ������ ����(��ƼŬ AliveCount)
	UINT               m_iMaxCount;      // ��ƼŬ �ִ� ����

	int m_bPosInherit; // ������Ʈ ��ǥ�� ����
	int m_iAliveCount; // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��

	float m_fMinLifeTime; // ��ƼŬ�� �ּ� ���� �ð�
	float m_fMaxLifeTime; // ��ƼŬ�� �ִ� ���� �ð�

	float m_fStartSpeed; // ��ƼŬ�� �ּ� �ӷ�
	float m_fEndSpeed;   // ��ƼŬ�� �ִ� �ӷ�

	Vec4 m_vStartColor; // ��ƼŬ �ʱ� ����
	Vec4 m_vEndColor;   // ��ƼŬ ���� ����

	Vec3 m_vStartScale; // ��ƼŬ �ʱ� ũ��
	Vec3 m_vEndScale;   // ��ƼŬ ���� ũ��


	float m_fParticleCreateDistance; // ��ƼŬ ���� ����
	float m_fParticleCreateTerm;     // ��ƼŬ ���� ����
	float m_fAccTime;

	int m_iEmissive; // ��ƼŬ �߱�����
	int m_iLighting; // ���� ���� ����

	Vec2 m_vDirection;

public:
	void                       SetEmissive(bool _bEmissive);
	void                       SetApplyLighting(bool _bLighting);
	void                       SetComputeShader(Ptr<CParticleUpdateShader> pCS) { m_CS = pCS; }
	Ptr<CParticleUpdateShader> GetComputeShader() const { return m_CS; }

	void SetMaxParticleCount(UINT count);
	UINT GetMaxParticleCount() const { return m_iMaxCount; }

	void SetPosInherit(bool _enable) { m_bPosInherit = _enable; }
	bool PosInherit() const { return m_bPosInherit; }

	void SetAliveCount(UINT _iCount);
	UINT GetAliveCount() const { return m_iAliveCount; }

	void SetMinMaxLifeTime(float fMin, float fMax);
	Vec2 GetMinMaxLifeTime() const { return Vec2{ m_fMinLifeTime, m_fMaxLifeTime }; }

	void SetMinMaxSpeed(float fMin, float fMax);
	Vec2 GetMinMaxSpeed() const { return Vec2{ m_fStartSpeed, m_fEndSpeed }; }

	void                  SetStartEndColor(const Vec4& vStartColor, const Vec4& vEndColor);
	std::pair<Vec4, Vec4> GetStartEndColor() const { return std::pair<Vec4, Vec4>{m_vStartColor, m_vEndColor}; }

	void                  SetStartEndScale(const Vec3& vStartScale, const Vec3& vEndScale);
	std::pair<Vec3, Vec3> GetStartEndScale() const { return std::pair<Vec3, Vec3>{m_vStartScale, m_vEndScale}; }

	void  SetRange(float _fRange);
	float GetRange() const { return m_fParticleCreateDistance; }

	void  SetTerm(float _fTerm);
	float GetTerm() const { return m_fParticleCreateTerm; }

	void SetDirection(Vec2 direction) { m_vDirection = direction; }
	Vec2 GetDirection() const { return m_vDirection; }

public:
	virtual void finalupdate() override;
	virtual void render() override;


public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;
	CLONE(CParticleSystem);

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;
public:
	CParticleSystem();
	CParticleSystem(const CParticleSystem& _origin);
	virtual ~CParticleSystem();
};
