#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CResMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CFBXLoader.h"


CMeshData::CMeshData()
	: CRes(RES_TYPE::MESHDATA) {}

CMeshData::~CMeshData() {}


CGameObject* CMeshData::Instantiate()
{
	auto pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);

	pNewObj->MeshRender()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetSharedMaterial(m_vecMtrl[i], i);
	}

	// Animation ��Ʈ �߰�
	if (false == m_pMesh->IsAnimMesh())
	{
		return pNewObj;
	}

	auto pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	// todo 
	int MaxFrameIdx = pAnimator->GetAnimClip(0).iFrameLength - 1;
	pAnimator->CreateAnimByFrame(L"test", 0, 0, MaxFrameIdx);

	return pNewObj;
}


vector<CMeshData*> CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	vector<CMeshData*> vecMeshData;

	// �޽� ��������
	int ContainerCnt = loader.GetContainerCount();
	for (int ContainerNum = 0; ContainerNum < ContainerCnt; ++ContainerNum)
	{
		// �޽� ��������
		CMesh* pMesh = nullptr;
		pMesh        = CMesh::CreateFromContainer(loader, ContainerNum);


		// ResMgr �� �޽� ���
		wstring strMeshName = L"mesh\\";
		strMeshName += std::filesystem::path(strFullPath).stem();
		string strNum = std::to_string(ContainerNum);
		strMeshName += wstring(strNum.begin(), strNum.end());
		strMeshName += L".mesh";


		// �޽� �̸� ����
		pMesh->SetName(strMeshName);

		CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

		vector<Ptr<CMaterial>> vecMtrl;

		// ���׸��� ��������
		for (UINT i = 0; i < loader.GetContainer(ContainerNum).vecMtrl.size(); ++i)
		{
			// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<
				CMaterial>(loader.GetContainer(ContainerNum).vecMtrl[i].strMtrlName);
			vecMtrl.push_back(pMtrl);
		}

		CMeshData* pMeshData = new CMeshData;
		pMeshData->m_pMesh   = pMesh;
		pMeshData->m_vecMtrl = vecMtrl;

		vecMeshData.push_back(pMeshData);
	}
	//���� ���� ��ġ�� ����� �ű� ��
	std::filesystem::path fmbFolderPath = std::filesystem::path{strFullPath}.replace_extension(L".fbm");
	remove_all(fmbFolderPath);
	return vecMeshData;
}

int CMeshData::Save(const wstring& _strFilePath)
{
	SetRelativePath(CPathMgr::GetInst()->GetRelativePath(_strFilePath));

	FILE*   pFile = nullptr;
	errno_t err   = _wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);

	// Mesh �� ���Ϸ� ����
	wstring strMeshPath = CPathMgr::GetInst()->GetContentPath();
	strMeshPath += m_pMesh->GetRelativePath();
	m_pMesh->Save(strMeshPath);

	// Mesh Key ����	
	// Mesh Data ����
	SaveResPtr<CMesh>(m_pMesh, pFile);


	// material ���� ����
	UINT iMtrlCount = static_cast<UINT>(m_vecMtrl.size());
	fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	UINT    i           = 0;
	wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();

	for (; i < iMtrlCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
		{
			continue;
		}

		// Material �� ���Ϸ� ����		
		m_vecMtrl[i]->Save(strMtrlPath + m_vecMtrl[i]->GetRelativePath());

		// Material �ε���, Key, Path ����
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveResPtr<CMaterial>(m_vecMtrl[i], pFile);
	}

	i = -1; // ���� ��
	fwrite(&i, sizeof(UINT), 1, pFile);

	fclose(pFile);

	return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	// Mesh Load
	LoadResPtr<CMesh>(m_pMesh, pFile);
	assert(m_pMesh.Get());

	// material ���� �б�
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, pFile);

	m_vecMtrl.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		fread(&idx, 4, 1, pFile);
		if (idx == -1)
		{
			break;
		}

		wstring strKey, strPath;

		Ptr<CMaterial> pMtrl;
		LoadResPtr<CMaterial>(pMtrl, pFile);

		m_vecMtrl[i] = pMtrl;
	}

	fclose(pFile);

	return S_OK;
}
