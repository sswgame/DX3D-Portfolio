#include "pch.h"
#include "ObjMgrState_LOADING.h"

#include <Engine/CFSM.h>
#include <Engine/CGameObject.h>
#include <Engine/CRenderEffectMgr.h>

#include "CObjectManager.h"

ObjMgrState_LOADING::ObjMgrState_LOADING()
	: CState{L"LOADING"} {}

ObjMgrState_LOADING::~ObjMgrState_LOADING() {}


void ObjMgrState_LOADING::Enter()
{
	CRenderEffectMgr::GetInst()->Enable_FadeInPaperBurn();
}


void ObjMgrState_LOADING::LateUpdate()
{
	if (CRenderEffectMgr::GetInst()->IsEnable_FadeInPaperBurn() && CRenderEffectMgr::GetInst()->IsFadeInFinished())
	{
		CRenderEffectMgr::GetInst()->Enable_FadeOutPaperBurn();
		EnableSceneObject();
	}
	else if (CRenderEffectMgr::GetInst()->IsEnable_FadeOutPaperBurn() && CRenderEffectMgr::GetInst()->
	         IsFadeOutFinished())
	{
		GetOwner()->FSM()->ChangeState(L"IDLE");
	}
}

void ObjMgrState_LOADING::Exit()
{
}

void ObjMgrState_LOADING::EnableSceneObject()
{
	const MAP_TYPE type = CObjectManager::GetInst()->GetCurrentMapType();
	for (auto& pGameObject : CObjectManager::GetInst()->GetSceneObjectList(type))
	{
		pGameObject->Deactivate();
	}
	for (auto& pGameObject : CObjectManager::GetInst()->GetDontDestroyList())
	{
		pGameObject->Activate();
	}

	int nextMapType = (int)type + 1;
	if ((MAP_TYPE)nextMapType != MAP_TYPE::END)
	{
		CObjectManager::GetInst()->SetMapType((MAP_TYPE)nextMapType);
		for (auto& pGameObject : CObjectManager::GetInst()->GetSceneObjectList((MAP_TYPE)nextMapType))
		{
			pGameObject->Activate();
		}

		CObjectManager::GetInst()->SetMapType((MAP_TYPE)nextMapType);
	}
}
