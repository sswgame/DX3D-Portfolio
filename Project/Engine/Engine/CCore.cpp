#include "pch.h"
#include "CCore.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"
#include "CRenderEffectMgr.h"


CCore::CCore()
	: m_hWnd(nullptr)
	, m_ptResolution{} {}

CCore::~CCore() = default;

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	RECT rt = {0, 0, _ptResolution.x, _ptResolution.y};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hWnd         = _hWnd;
	m_ptResolution = _ptResolution;

	// Manager ÃÊ±âÈ­
	if (FAILED(CDevice::GetInst()->init(m_hWnd, Vec2(static_cast<float>(m_ptResolution.x), static_cast<float>(
		           m_ptResolution.y)))))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CRenderEffectMgr::GetInst()->Init();
;	CSceneMgr::GetInst()->init();
	CFontMgr::GetInst()->Init();

	return S_OK;
}


void CCore::Frame_Init()
{
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
}

void CCore::Frame_Clear() {}

void CCore::progress()
{
	Frame_Init();

	CSceneMgr::GetInst()->progress();
	CResMgr::GetInst()->update();
	CRenderMgr::GetInst()->render();
	CEventMgr::GetInst()->update();
	Frame_Clear();
}
