#include "pch.h"
#include "CDieState.h"


CDieState::CDieState()
	: CState(L"DIE")
{
}

CDieState::CDieState(const CDieState& _origin)
	:CState(_origin)

{
}

CDieState::~CDieState()
{
}


void CDieState::Enter()
{
	CState::ResetTimer();


}

void CDieState::Exit()
{

}

void CDieState::Update()
{
	CState::Update();

}

void CDieState::LateUpdate()
{
	CState::LateUpdate();

}
