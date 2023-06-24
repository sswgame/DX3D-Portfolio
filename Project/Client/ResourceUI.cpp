#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CThreadPool.h>

#include <Script/CSceneSaveLoad.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	m_TreeUI->UseFrame(true);
	m_TreeUI->UseDragDropOuter(true);
	AddChild(m_TreeUI);

	// Clicked Delegate ���
	m_TreeUI->SetClickedDelegate(this, static_cast<CLICKED>(&ResourceUI::ItemClicked));
	// DoubleClicked Deletage ���
	m_TreeUI->SetDoubleClickedDelegate(this, static_cast<CLICKED>(&ResourceUI::ItemDBClicked));

	Reset();
}

ResourceUI::~ResourceUI() = default;

void ResourceUI::update()
{
	// ���ҽ� ������� ����
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		LOG_INFO("RESOURCE HAS BEEN SAVED");
		CResMgr::GetInst()->SaveChangedRes();
	}
	UI::update();
}

void ResourceUI::render_update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Renew();
	}
}

void ResourceUI::Reset()
{
	// Content ���� ���� ���ҽ� �ε�
	Reload();
	Renew();
}


void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	const auto pNode     = reinterpret_cast<TreeNode*>(_dwNode);
	const auto pResource = reinterpret_cast<CRes*>(pNode->GetData());
	// ������ ��尡 ���ȴٸ� �ƹ��ϵ� ����.
	if (nullptr == pResource)
	{
		return;
	}
	const auto pInspectorUI = static_cast<InspectorUI*>(CImGuiMgr::GetInst()->FindUI("Inspector"));
	pInspectorUI->SetTargetResource(pResource);
}

void ResourceUI::ItemDBClicked(DWORD_PTR _dwNode)
{
	const auto  pNode     = reinterpret_cast<TreeNode*>(_dwNode);
	const CRes* pResource = reinterpret_cast<CRes*>(pNode->GetData());
	// ������ ��尡 ���ȴٸ� �ƹ��ϵ� ����.
	if (nullptr == pResource || pNode->GetParent()->GetName() != "SCENEFILE")
	{
		return;
	}
	// Scene �ε�
	// ���� Scene ����
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetSceneState(SCENE_STATE::STOP);

	// �ε��� Scene ������ ��� ���
	const wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pResource->GetRelativePath();
	CScene*       pNewScene   = CSceneSaveLoad::LoadScene(strFilePath);
	CSceneMgr::GetInst()->ChangeScene(pNewScene);
}

void ResourceUI::Reload()
{
	UsingMultiThread();
	//WithOutMultiThead();

	const wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 3. �ε��Ǿ��ִ� ���ҽ����� ���� ���� ���� Ȯ��
	for (UINT i = 0; i < static_cast<UINT>(RES_TYPE::END); ++i)
	{
		const auto& mapResource = CResMgr::GetInst()->GetResList(static_cast<RES_TYPE>(i));
		for (const auto& [resourceKey, pResource] : mapResource)
		{
			if (pResource->IsEngineRes())
			{
				continue;
			}
			// File Exist üũ
			if (false == std::filesystem::exists(strContentPath + pResource->GetRelativePath()))
			{
				// 4. ������ ���ҽ��Ŵ������� �޸� ����
				//MessageBox(nullptr, L"�������� ���� ��", L"���Ϻ��� ����", MB_OK);
				LOG_INFO("RESOURCE DELETED: %s", ToString(pResource->GetKey()).c_str());

				if (0 == pResource->GetRefCount())
				{
					// ����
					tEventInfo info{};
					info.eType  = EVENT_TYPE::DELETE_RES;
					info.lParam = reinterpret_cast<DWORD_PTR>(pResource);
					CEventMgr::GetInst()->AddEvent(info);
					//MessageBox(nullptr, L"���ҽ� �޸� ����", L"���Ϻ��� ����", MB_OK);
					LOG_INFO("MEMORY RELEASED SUCCESSFULLY");
				}
				else
				{
					//MessageBox(nullptr, L"��� ���� ���ҽ�\n �޸� ���� ����", L"���Ϻ��� ����", MB_OK);
					LOG_WARN("CURRENTLY USING,RELEASE MEMORY FAILED");
				}
			}
		}
	}
}

void ResourceUI::Renew()
{
	m_TreeUI->Clear();

	// ���ҽ��� Tree �� ǥ��
	for (int i = 0; i < static_cast<int>(RES_TYPE::END); ++i)
	{
		// TreeUI �� DummyRoot �� ����ϱ� ������, ���ҽ� �׸� ������ ���� �ڽ����� ��
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString(static_cast<RES_TYPE>(i)));
		pResNode->SetNodeType(NODE_TYPE::ENGINE_RESOURCE);

		const auto& mapResource = CResMgr::GetInst()->GetResList(static_cast<RES_TYPE>(i));
		for (const auto& [resourceKey, pResource] : mapResource)
		{
			// �� ���ҽ� ������ �ش� ���ҽ� �׸� �ڽ����� ��
			m_TreeUI->AddTreeNode(pResNode, ToString(resourceKey), reinterpret_cast<DWORD_PTR>(pResource));
		}
	}
}

void ResourceUI::FindFileName(const wstring& _strFolderPath)
{
	//����� ������� �ε��ϴ� ��.
	/*
	wstring strContent = _strFolderPath + L"*.*";

	WIN32_FIND_DATA FindFileData = {};

	HANDLE hFind = nullptr;

	hFind = FindFirstFile(strContent.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindFileData))
	{
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (0 == wcscmp(FindFileData.cFileName, L".."))
				continue;

			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
			continue;
		}

		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
		strRelativePath         = CPathMgr::GetInst()->GetRelativePath(strRelativePath);

		m_vecResPath.push_back(strRelativePath);
	}

	FindClose(hFind);
	*/

	for (const auto& entry : std::filesystem::recursive_directory_iterator{_strFolderPath})
	{
		if (false == entry.is_directory())
		{
			std::wstring relativePath = CPathMgr::GetInst()->GetRelativePath(entry.path());
			m_vecResPath.push_back(relativePath);
		}
	}
}


RES_TYPE ResourceUI::GetResTypeFromExt(const wstring& _strExt)
{
	const std::filesystem::path extension = std::filesystem::path{_strExt}.extension();

	if (L".mtrl" == extension)
	{
		return RES_TYPE::MATERIAL;
	}

	if (L".png" == extension
	    || L".jpeg" == extension
	    || L".bmp" == extension
	    || L".jpg" == extension
	    || L".tga" == extension
	    || L".dds" == extension)
	{
		return RES_TYPE::TEXTURE;
	}

	if (L".mp3" == extension || L".wav" == extension || L".ogg" == extension)
	{
		return RES_TYPE::SOUND;
	}

	if (L".pref" == extension)
	{
		return RES_TYPE::PREFAB;
	}

	if (L".mesh" == extension)
	{
		return RES_TYPE::MESH;
	}

	if (L".scene" == extension)
	{
		return RES_TYPE::SCENEFILE;
	}

	if (L".map" == extension)
	{
		return RES_TYPE::NAVIMAPDATA;
	}

	if (L".mdat" == extension)
	{
		return RES_TYPE::MESHDATA;
	}

	return RES_TYPE::END;
}

void ResourceUI::UsingMultiThread()
{
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());
	LOG_TRACE("TOTAL FILE COUNT : %d", m_vecResPath.size());

	TIME_MEASURE("LOADING RESOURCE WITH MULTI-THREAD");
	std::vector<std::wstring> vecData[static_cast<UINT>(RES_TYPE::END)]{};

	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		const RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);
		if (RES_TYPE::END != eType)
		{
			vecData[static_cast<UINT>(eType)].push_back(m_vecResPath[i]);
		}
	}

	LOG_TRACE("PREFAB : %d", vecData[(UINT)RES_TYPE::PREFAB].size());
	LOG_TRACE("MESHDATA : %d", vecData[(UINT)RES_TYPE::MESHDATA].size());
	LOG_TRACE("MATERIAL : %d", vecData[(UINT)RES_TYPE::MATERIAL].size());
	LOG_TRACE("MESH : %d", vecData[(UINT)RES_TYPE::MESH].size());
	LOG_TRACE("TEXTURE : %d", vecData[(UINT)RES_TYPE::TEXTURE].size());
	LOG_TRACE("SOUND : %d", vecData[(UINT)RES_TYPE::SOUND].size());
	LOG_TRACE("SCENEFILE : %d", vecData[(UINT)RES_TYPE::SCENEFILE].size());
	LOG_TRACE("NAVIMAPDATA : %d", vecData[(UINT)RES_TYPE::NAVIMAPDATA].size());

	//TEXTURE
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::TEXTURE)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CTexture>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("TEXTURE THREAD JOIN SUCCEDED");

	//MESH
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::MESH)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CMesh>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("MESH THREAD JOIN SUCCEDED");

	//MATERIAL
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::MATERIAL)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CMaterial>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("MATERIAL THREAD JOIN SUCCEDED");

	//MESHDATA
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::MESHDATA)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CMeshData>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("MESH-DATA THREAD JOIN SUCCEDED");

	//PREFAB
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::PREFAB)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CPrefab>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("PREFAB THREAD JOIN SUCCEDED");

	//SCENEFILE
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::SCENEFILE)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CSceneFile>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("SCENE-FILE THREAD JOIN SUCCEDED");

	//NaviMapDat
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::NAVIMAPDATA)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CSceneFile>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("NAVIMAPDATA THREAD JOIN SUCCEDED");

	//NAVMESHDATA
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::NAVIMAPDATA)])
	{
		CThreadPool::GetInst()->EnqueueJob(&ResourceUI::LoadResource<CNaviMapData>, this, resourceKey);
	}
	CThreadPool::GetInst()->Start();
	CThreadPool::GetInst()->Join();
	LOG_TRACE("NAVMESHDATA THREAD JOIN SUCCEDED");

	//SOUND
	for (auto& resourceKey : vecData[static_cast<UINT>(RES_TYPE::SOUND)])
	{
		LoadResource<CSound>(resourceKey);
	}
	LOG_TRACE("SOUND THREAD JOIN SUCCEDED");
}

void ResourceUI::WithOutMultiThead()
{
	// Content ���� ������ ��� ������ ����θ� �˾Ƴ���.
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());

	TIME_MEASURE("LOADING RESOURCE WITH SINGLE-THREAD");
	// Ű�� Ȯ��
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		// 1. ���ϸ��� ���ؼ� ���ҽ��� ������ �˾Ƴ���.
		const RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);
		// 2. ����θ� Ű������ �ؼ� ���ҽ��Ŵ����� �ε��Ѵ�.
		switch (eType)
		{
		case RES_TYPE::PREFAB:
			{
				LoadResource<CPrefab>(m_vecResPath[i]);
				break;
			}
		case RES_TYPE::MESHDATA:
			{
				CResMgr::GetInst()->Load<CMeshData>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::MATERIAL:
			{
				CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::MESH:
			{
				CResMgr::GetInst()->Load<CMesh>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::TEXTURE:
			{
				CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::SOUND:
			{
				CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::SCENEFILE:
			{
				CResMgr::GetInst()->Load<CSceneFile>(m_vecResPath[i], m_vecResPath[i]);
				break;
			}
		case RES_TYPE::NAVIMAPDATA:
			break;
		}
	}
}
