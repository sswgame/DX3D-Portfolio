#pragma once
#include "ComponentUI.h"

class ParticleSystemUI final
	: public ComponentUI
{
private:
	CParticleSystem* m_pParticleSystem;

	int  m_iMaxCount;   // ��ƼŬ �ִ� ����
	bool m_bPosInherit; // ������Ʈ ��ǥ�� ����
	int  m_iAliveCount; // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��

	float m_fArrMinMaxTime[2];    // ��ƼŬ�� �ּ�/�ִ� ���� �ð�
	float m_fArrMinMaxSpeed[2];   // ��ƼŬ�� �ּ�/�ִ� �ӷ�
	Vec4  m_vArrStartEndColor[2]; // ��ƼŬ �ʱ�/���� ����
	Vec3  m_vArrStartEndScale[2]; // ��ƼŬ �ʱ�/���� ũ��
	int   m_iArrDirection[2];     // ��ƼŬ ����

	float m_fRange; // ��ƼŬ ���� ����
	float m_fTerm;  // ��ƼŬ ���� ����

	bool m_bShaderUseDirection;

private:
	void TextureSelect(void* _pTextureName);
	void ShaderSelect(void* _pShaderName);

	void SetData();

public:
	void render_update() override;

public:
	ParticleSystemUI();
	virtual ~ParticleSystemUI();
};
