#pragma once
#include <Engine/CScript.h>

class CStructuredBuffer;
class CAnimator3D;
class CGameObject;
class CMeshData;

struct tTrail
{
	CGameObject* pTrailObj;
	float		 fTimer;
};

class TrailScript :
	public CScript
{
private:
	CGameObject* m_pCam;

	CGameObject* m_pOriginObj;			// �ܻ��� ����Ų ���� ������Ʈ  
	Ptr<CMeshData>			m_pOribinObj_MeshData;  // �ܻ��� ����Ų ���� ������Ʈ�� �޽� ������  
	CGameObject* m_pTrail;


	float					m_fCreateTerm;			// �ܻ��� �Ͼ�� �ð� ���� 
	float					m_fTimer;				// �ܻ��� �Ͼ ���� �ð�
	float					m_fDestroyTime;			// �ܻ��� ����� �ð� ( �ܻ� ���� �ð� )

	bool					m_bCreateTrail;
	vector<tTrail>			m_vecTrail;				// �ܻ� ������Ʈ ����
	int						m_iTrailMaxCnt;			// �ܻ� ����


	bool					m_bPlayTrail;			// �ܻ� ��� ���� 


public:
	void SetOriginObject(CGameObject* _Origin);

	void SetTrailMeshData(wstring _strKey);
	void CreateTrail(int _FrameIdx);

	bool DeactivateTrail();
	bool ActivateTrail();
	void UpdateActiveTrail();


	void SetCreateTermTime(float _fTimer) { m_fCreateTerm = _fTimer; }
	void SetDestroyTermTime(float _fTimer) { m_fDestroyTime = _fTimer; }

	void TimerReset() { m_fTimer = 0.f; }
	int GetTrailFrameIdx();

	void On() { m_bPlayTrail = true; }
	void Off() { m_bPlayTrail = false; }


	CGameObject* SetTrailObj();
	CGameObject* CloneTrailObj();



public:
	virtual void start();
	virtual void update();
	virtual void lateupdate();

	virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
	virtual void OnCollision(CGameObject* _OtherObject) override;
	virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(TrailScript)

public:
	TrailScript(const TrailScript& _origin);
	TrailScript();
	virtual ~TrailScript();

};

