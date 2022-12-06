#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CEventMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CSceneSaveLoad.h>

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	m_TreeUI->UseFrame(true);
	m_TreeUI->UseDragDropOuter(true);

	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&ResourceUI::ItemClicked);

	// DoubleClicked Deletage 등록
	m_TreeUI->SetDoubleClickedDelegate(this, (CLICKED)&ResourceUI::ItemDBClicked);

	Reset();
}

ResourceUI::~ResourceUI() {}

void ResourceUI::update()
{
	// 리소스 변경상태 저장
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
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
	// Content 폴더 밑의 리소스 로딩
	Reload();

	Renew();
}

void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey    = pNode->GetName();
	CRes*  pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource)
		return;

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetResource(pResource);
}

void ResourceUI::ItemDBClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey    = pNode->GetName();
	CRes*  pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource || pNode->GetParent()->GetName() != "SCENEFILE")
		return;

	// Scene 로딩
	// 현재 Scene 정지
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetSceneState(SCENE_STATE::STOP);

	// 로딩할 Scene 파일의 경로 계산
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += pResource->GetRelativePath();

	CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
	CSceneMgr::GetInst()->ChangeScene(pNewScene);
}


void ResourceUI::Reload()
{
	// Content 폴더 밑으로 모든 파일의 상대경로를 알아낸다.
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());

	// 키값 확인
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		// 1. 파일명을 통해서 리소스의 종류를 알아낸다.
		const RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);

		// 2. 상대경로를 키값으로 해서 리소스매니저에 로딩한다.
		switch (eType)
		{
		case RES_TYPE::PREFAB:
			{
				CResMgr::GetInst()->Load<CPrefab>(m_vecResPath[i], m_vecResPath[i]);
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

	// 3. 로딩되어있는 리소스들의 실제 파일 존재 확인
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);
		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			// File Exist 체크
			if (false == std::filesystem::exists(strContentPath + pair.second->GetRelativePath()))
			{
				// 4. 없으면 리소스매니저에서 메모리 해제
				MessageBox(nullptr, L"원본파일 삭제 됨", L"파일변경 감지", MB_OK);

				if (0 == pair.second->GetRefCount())
				{
					// 삭제
					tEventInfo info{};
					info.eType  = EVENT_TYPE::DELETE_RES;
					info.lParam = (DWORD_PTR)pair.second;
					CEventMgr::GetInst()->AddEvent(info);

					MessageBox(nullptr, L"리소스 메모리 해제", L"파일변경 감지", MB_OK);
				}
				else
				{
					MessageBox(nullptr, L"사용 중인 리소스\n 메모리 해제 실패", L"파일변경 감지", MB_OK);
				}
			}
		}
	}
}

void ResourceUI::Renew()
{
	m_TreeUI->Clear();

	// 리소스를 Tree 에 표시
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		// TreeUI 가 DummyRoot 를 사용하기 때문에, 리소스 항목 노드들은 더미 자식으로 들어감
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString((RES_TYPE)i));
		pResNode->SetNodeType(NODE_TYPE::ENGINE_RESOURCE);

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			// 각 리소스 노드들은 해당 리소스 항목 자식으로 들어감
			m_TreeUI->AddTreeNode(pResNode, ToString(pair.first), (DWORD_PTR)pair.second);
		}
	}
}

void ResourceUI::FindFileName(const wstring& _strFolderPath)
{
	//강사님 방식으로 로딩하는 것.
	/*wstring strContent = _strFolderPath + L"*.*";

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

	FindClose(hFind);*/

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

	return RES_TYPE::END;
}
