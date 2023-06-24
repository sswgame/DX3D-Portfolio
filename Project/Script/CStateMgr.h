#pragma once
#include <Engine/CEntity.h>
#include "CObjKeyMgr.h"

class CStateMgr
	: public CEntity
{
private:
	CGameObject* m_pTrailSocketCollider;

	CGameObject*			m_pOwnerObj;
	CGameObject*			m_pCamera;


	wstring					m_sPrevState;
	wstring					m_sCurstate;

	tKey_Zip				m_tCurKeyInfo;
	tKey_Zip				m_tPrevKeyInfo;


	bool					m_bOnGround;	// ������ �ִ��� Ȯ�� 

	// ī�޶� ��� ��� ������ ���� ���� 
	bool					m_bCameraLock;			// ī�޶� ��� ���� ������� Ȯ�� 
	Vec3					m_vCamLock_TargetPos;	// ī�޶� ��� ��� ���� ���� ��ǥ 
	CGameObject*			m_pCamLock_TargetObj;	// ī�޶� ��� ���� ������ ���� ������Ʈ ���� 



public:
	void	Update();
	void	LateUpdate();


public:
	void InitState(CGameObject* _pOwner);
	void ChangeCurStateType(wstring _StateTypeName); 	// �ܺο��� ����Ǿ��� �� Curstate�� �����ϱ� ���ؼ� ������ �Լ�

	


public:
	// [ SET PART ]
	void SetCurKeyState(KEY_STATE _eKeyState)		{ m_tCurKeyInfo.eKeyState = _eKeyState; }
	void SetCurKey(KEY _eKey)						{ m_tCurKeyInfo.eKey = _eKey; }
	void SetKeyInfo(KEY_STATE _eKeyState, KEY _eKey, tKeyOptionFlags_Zip _tFlags);
	void SetCurKeyInfo(tKey_Zip _tCurKeyInfo)		{ m_tCurKeyInfo = _tCurKeyInfo; }
	void SetPrevKeyInfo(tKey_Zip _tPrevKeyInfo)		{ m_tPrevKeyInfo = _tPrevKeyInfo; }
	void SetCameraLock(bool _b);
	void SetCameraLockTargetPos(Vec3 _vPos)			{ m_vCamLock_TargetPos = _vPos; }
	void SetCamera(CGameObject* _pCam)				{ m_pCamera = _pCam; }

	// [ GET PART ]
	wstring		 GetCurstate()							{ return m_sCurstate; }
	wstring		 GetPrevState()							{ return m_sPrevState; }
	tKey_Zip	 GetCurKeyInfo()						{ return m_tCurKeyInfo; }
	tKey_Zip	 GetPrevKeyInfo()						{ return m_tPrevKeyInfo; }
	bool		 IsCameraLock()							{ return m_bCameraLock; }
	Vec3		 GetCurCameraLockTargetPos()			{ return m_vCamLock_TargetPos; }
	CGameObject* GetCamera()							{ return m_pCamera; }

	Vec3		GetCameraLockTargetPos();

private:
	wstring ChangeStateByKeyInfo();
	void ChangeState(wstring _sStateName);

	wstring Check_Tap();
	wstring Check_Pressed();
	wstring Check_Away();



	CLONE(CStateMgr)

public:
	CStateMgr();
	virtual ~CStateMgr();

};

