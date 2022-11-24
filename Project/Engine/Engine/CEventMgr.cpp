#include "pch.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CRenderMgr.h"
#include "CComponent.h"
#include "CFSM.h"
#include "CMgrScript.h"

CEventMgr::CEventMgr()
	: m_bObjEvn(false)
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::update()
{
	m_bObjEvn = false;

	// Dead Object 삭제
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		assert(m_vecDead[i]);

		// 삭제되는 오브젝트가 부모가 있다면(자식 오브젝트라면)
		if (m_vecDead[i]->GetParent())
		{
			m_vecDead[i]->DisconnectBetweenParent();
		}

		delete m_vecDead[i];
		m_bObjEvn = true;
	}
	m_vecDead.clear();


	// Event 처리
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJ:
			// lParam : Object Adress, wParam : Layer Index
			{
				CGameObject* pObj      = (CGameObject*)m_vecEvent[i].lParam;
				int          iLayerIdx = (int)m_vecEvent[i].wParam;
				CScene*      pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->AddObject(pObj, iLayerIdx);

				pObj->start();

				m_bObjEvn = true;
			}
			break;

		case EVENT_TYPE::DELETE_OBJ:
			// lParam : Object Adress
			{
				CGameObject* pDeleteObject = (CGameObject*)m_vecEvent[i].lParam;

				if (false == pDeleteObject->m_bDead)
				{
					m_vecDead.push_back(pDeleteObject);

					// 보유하고 있는 자식 오브젝트도 전부 Dead 체크 해둔다.
					static list<CGameObject*> queue;
					queue.clear();

					queue.push_back(pDeleteObject);

					while (!queue.empty())
					{
						CGameObject* pObj = queue.front();
						queue.pop_front();
						pObj->m_bDead = true;

						const vector<CGameObject*>& vecChild = pObj->GetChild();
						for (size_t i = 0; i < vecChild.size(); ++i)
						{
							queue.push_back(vecChild[i]);
						}
					}
				}
			}

			break;

		case EVENT_TYPE::ADD_CHILD:
			// lParam : Parent Object, wParam : Child Object
			{
				CGameObject* pParent = (CGameObject*)m_vecEvent[i].lParam;
				CGameObject* pChild  = (CGameObject*)m_vecEvent[i].wParam;

				int iParent_LayerIdx = pParent->GetLayerIndex();
				int iChild_LayerIdx  = pChild->GetLayerIndex();

				if (iParent_LayerIdx != iChild_LayerIdx)
				{
					CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
					pCurScene->GetLayer(iChild_LayerIdx)->DeregisterObject(pChild);
					pChild->RenewLayerIndex(iParent_LayerIdx);
				}

				pParent->AddChild(pChild);
				m_bObjEvn = true;
			}
			break;

		case EVENT_TYPE::DISCONNECT_PARENT:
			// lParam : Parent Object, wParam : Child Object
			{
				CGameObject* pObject = (CGameObject*)m_vecEvent[i].lParam;
				pObject->DisconnectBetweenParent();
				int     i         = pObject->GetLayerIndex();
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->GetLayer(i)->RegisterObjectAsRoot(pObject);

				m_bObjEvn = true;
			}
			break;


		case EVENT_TYPE::SET_CAMEAR_INDEX:
			{
				CCamera* cam        = (CCamera*)m_vecEvent[i].lParam;
				int      iChangeIdx = (int)m_vecEvent[i].wParam;

				CRenderMgr::GetInst()->SwapCameraIndex(cam, iChangeIdx);
			}


			break;

		case EVENT_TYPE::ACTIVATE_OBJECT:
			{
				CGameObject* pObject = (CGameObject*)m_vecEvent[i].lParam;
				pObject->m_bActive   = true;
				pObject->active();
			}


			break;

		case EVENT_TYPE::DEACTIVATE_OBJECT:
			{
				CGameObject* pObject = (CGameObject*)m_vecEvent[i].lParam;
				pObject->m_bActive   = false;
				pObject->deactive();
			}
			break;


		case EVENT_TYPE::ACTIVATE_COMPONENT:
			{
				CComponent* pCom = (CComponent*)m_vecEvent[i].lParam;
				pCom->active();
			}
			break;

		case EVENT_TYPE::DEACTIVATE_COMOPNENT:
			{
				CComponent* pCom = (CComponent*)m_vecEvent[i].lParam;
				pCom->deactive();
			}
			break;

		case EVENT_TYPE::SCENE_CHANGE:
			{
				m_bObjEvn = true;
			}
			break;


		case EVENT_TYPE::DELETE_RES:
			{
				// lParam : Res Adress
				CRes* pRes = (CRes*)m_vecEvent[i].lParam;

				CResMgr::GetInst()->DeleteRes(pRes->GetKey());

				m_bObjEvn = true;
			}
			break;
		case EVENT_TYPE::CHANGE_FSM_STATE:
			{
				// lParam : FSM Component
				// wParam : Next State Type Name

				CFSM*   pFSM       = (CFSM*)m_vecEvent[i].lParam;
				wstring sNextState = (const wchar_t*)m_vecEvent[i].wParam;


				const auto iter = pFSM->m_mapState.find(sNextState);
				if (iter == pFSM->m_mapState.end())
					break;

				// 기존 상태 Exit 
				CState* pPrevState = pFSM->GetCurState();
				if (pPrevState != nullptr)
					pPrevState->Exit();

				// 현재 상태 Enter 
				CState* pNextState = iter->second;
				pNextState->Enter();
				pFSM->SetCurState(pNextState);

				break;
			}
		case EVENT_TYPE::CHANGE_OBJ_LAYER_INDEX:
			{
				// lParam : GameObject*
				// wParam : LayerIndex		
				CGameObject* pObject      = (CGameObject*)m_vecEvent[i].lParam;
				UINT         iNewLayerIdx = (UINT)m_vecEvent[i].wParam;

				UINT    iCurLayerIdx = pObject->GetLayerIndex();
				CScene* pCurScene    = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->GetLayer(iCurLayerIdx)->DeregisterObject(pObject);
				pCurScene->AddObject(pObject, iNewLayerIdx);

				m_bObjEvn = true;
			}
			break;
		case EVENT_TYPE::SWAP_LAYER:
			{
				// lParam : LayerIndex
				// wParam : LayerIndex				
				UINT iLayerIdx_1 = (UINT)m_vecEvent[i].lParam;
				UINT iLayerIdx_2 = (UINT)m_vecEvent[i].wParam;

				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->SwapLayer(iLayerIdx_1, iLayerIdx_2);
				m_bObjEvn = true;
			}
			break;

		// 이벤트 중에  변경 이벤트가 있었다면,
		// 나머지 이벤트는 다 무시하고 종료
		/*if (bChangeStage)
		{
			break;
		}*/
		}
	}

	m_vecEvent.clear();

	//Custom Event
	if (false == CSingletonScript::s_vecFunc.empty())
	{
		CSingletonScript::FireScriptEvents();
		m_bObjEvn = true;
	}
}
