#pragma once
#include "UI.h"

class CParticleSystem;
class CSound;

class ParticleTool : public UI
{
private:
	Ptr<CTexture> m_pParticleTarget;
	Ptr<CTexture> m_pEmissiveTarget;
	Ptr<CTexture> m_pDepthTarget;

	vector<CGameObject*> m_vecParticleSystem;
	CGameObject* m_pTargetParticle;
	vector<string>       m_strvecParticleName;

	Vec2 m_vecTargetResolution;

	int  m_iMaxCount;   // ��ƼŬ �ִ� ����
	bool m_bPosInherit; // ������Ʈ ��ǥ�� ����
	int  m_iAliveCount; // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��

	float m_fArrMinMaxTime[2];    // ��ƼŬ�� �ּ�/�ִ� ���� �ð�
	float m_fArrMinMaxSpeed[2];   // ��ƼŬ�� �ּ�/�ִ� �ӷ�
	Vec4  m_vArrStartEndColor[2]; // ��ƼŬ �ʱ�/���� ����
	Vec3  m_vArrStartEndScale[2]; // ��ƼŬ �ʱ�/���� ũ��
	float m_fArrDirection[2];     // ��ƼŬ ����
	Vec4  m_vArrStartEndEmissive[2]; // ��ƼŬ �ʱ�/���� Emissive

	float m_fRange; // ��ƼŬ ���� ����
	float m_fTerm;  // ��ƼŬ ���� ����
	float m_fAngle;

	bool m_bShaderUseEmissive;
	bool m_bShaderUseSpeedDetail;
	bool m_bLinerParticle;
	bool m_bUseSoftParticle;

	int m_iParticleComboIDX;

	wstring m_strSoundName;
	CSound* m_pSound;

private:
	void SetData();
	void TextureSelect(void* _pTextureName);

public:
	void ParticleCreate();
	void ParticleErase();
	void ParticleSaveNLoad();
	void AddParticleCombo(CGameObject* _newparti);
	void MakeTargetParticleCombo();
	void ParticleOptionSetting();
	void SaveParticle();
	void LoadParticle(CGameObject* _load);
	void SetSound();
	void SoundLoad();

public:
	void update() override;
	void render_update() override; //


public:
	ParticleTool();
	virtual ~ParticleTool();
};
