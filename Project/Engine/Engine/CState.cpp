#include "pch.h"
#include "CState.h"
#include "CTimeMgr.h"


CState::CState(wstring _sState)
	: m_sStateType(_sState)
	, m_bTimePlay(true)
	, m_pOwner(nullptr)
{

}

CState::~CState()
{

}


void CState::Update()
{
	if (m_bTimePlay)
		m_fTimer += DT;

}

void CState::LateUpdate()
{
}


void CState::StartTimer()
{
	m_bTimePlay = true;
}

void CState::StopTimer()
{
	m_bTimePlay = false;
}

void CState::ResetTimer()
{
	m_bTimePlay = true;
	m_fTimer = 0.f;

}
