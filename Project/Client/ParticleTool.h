#pragma once
#include "UI.h"

class CParticleSystem;

class ParticleTool :
    public UI
{
private:
    Ptr<CTexture> m_pParticleTarget;
    Ptr<CTexture> m_pEmissiveTarget;

    vector<CGameObject*> m_vecParticleSystem;
    CGameObject* m_pTargetParticle;
    vector<string> m_strvecParticleName;

    Vec2            m_vecTargetResolution;

    int  m_iMaxCount;   // ��ƼŬ �ִ� ����
    bool m_bPosInherit; // ������Ʈ ��ǥ�� ����
    int  m_iAliveCount; // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��

    float m_fArrMinMaxTime[2];    // ��ƼŬ�� �ּ�/�ִ� ���� �ð�
    float m_fArrMinMaxSpeed[2];   // ��ƼŬ�� �ּ�/�ִ� �ӷ�
    Vec4  m_vArrStartEndColor[2]; // ��ƼŬ �ʱ�/���� ����
    Vec3  m_vArrStartEndScale[2]; // ��ƼŬ �ʱ�/���� ũ��
    float   m_fArrDirection[2];     // ��ƼŬ ����
    Vec4  m_vArrStartEndEmissive[2]; // ��ƼŬ �ʱ�/���� Emissive

    float m_fRange; // ��ƼŬ ���� ����
    float m_fTerm;  // ��ƼŬ ���� ����

    float m_fAngle;

    bool	m_bShaderUseEmissive;
    bool m_bShaderUseSpeedDetail;

private:
    void SetData();


public:
    void MakeTargetParticleCombo();
    void ParticleOptionSetting();

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    ParticleTool();
    ~ParticleTool();
};
