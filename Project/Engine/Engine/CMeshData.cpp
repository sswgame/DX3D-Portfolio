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

CMeshData::~CMeshData() = default;

CGameObject* CMeshData::Instantiate()
{
	const auto pNewGameObject = new CGameObject{};
	pNewGameObject->AddComponent(new CTransform);
	pNewGameObject->AddComponent(new CMeshRender);

	pNewGameObject->MeshRender()->SetMesh(m_pMesh);
	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewGameObject->MeshRender()->SetSharedMaterial(m_vecMtrl[i], i);
	}

	// Animation ��Ʈ �߰�
	if (false == m_pMesh->IsAnimMesh())
	{
		return pNewGameObject;
	}

	const auto pAnimator3D = new CAnimator3D{};
	pNewGameObject->AddComponent(pAnimator3D);
	pAnimator3D->SetBones(m_pMesh->GetBones());
	pAnimator3D->SetAnimClip(m_pMesh->GetAnimClip());

	const int MaxFrameIdx = pAnimator3D->GetAnimClip(0).iFrameLength - 1;
	pAnimator3D->CreateAnimByFrame(L"test", 0, 0, MaxFrameIdx);

	return pNewGameObject;
}

vector<CMeshData*> CMeshData::LoadFromFBX(const wstring& _strPath)
{
	const wstring strFullPath = CPathMgr::GetInst()->GetContentPath() + _strPath;

	CFBXLoader loader{};
	loader.init();
	loader.LoadFbx(strFullPath);

	vector<CMeshData*> vecMeshData{};
	// �޽� ��������
	const int containerCount = loader.GetContainerCount();
	for (int index = 0; index < containerCount; ++index)
	{
		CMesh* pMesh = CMesh::CreateFromContainer(loader, index);

		// ResMgr �� �޽� ���
		wstring strMeshName = L"mesh\\"
		                      + std::filesystem::path(strFullPath).stem().wstring()
		                      + std::to_wstring(index) + L".mesh";

		// �޽� �̸� ����
		pMesh->SetName(strMeshName);
		CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

		vector<Ptr<CMaterial>> vecMtrl{};
		// ���׸��� ��������
		for (auto& fbxMaterial : loader.GetContainer(index).vecMtrl)
		{
			// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(fbxMaterial.strMtrlName);
			if (nullptr != pMtrl)
			{
				vecMtrl.push_back(pMtrl);
			}
		}

		CMeshData* pMeshData = new CMeshData{};
		pMeshData->m_pMesh   = pMesh;
		pMeshData->m_vecMtrl = vecMtrl;

		vecMeshData.push_back(pMeshData);
	}

	//���� ���� ��ġ�� ����� �ű� ��
	const std::filesystem::path fbmFolderPath = std::filesystem::path{strFullPath}.replace_extension(L".fbm");
	remove_all(fbmFolderPath);
	return vecMeshData;
}

int CMeshData::Save(const wstring& _strFilePath)
{
	SetRelativePath(CPathMgr::GetInst()->GetRelativePath(_strFilePath));

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	LOG_ASSERT(pFile, "FILE OPEN FAILED");

	// Mesh �� ���Ϸ� ����
	const wstring meshPath = CPathMgr::GetInst()->GetContentPath() + m_pMesh->GetRelativePath();
	m_pMesh->Save(meshPath);

	// Mesh Key ����	,Mesh Data ����
	SaveResPtr<CMesh>(m_pMesh, pFile);

	// material ���� ����
	const UINT materialCount = static_cast<UINT>(m_vecMtrl.size());
	fwrite(&materialCount, sizeof(UINT), 1, pFile);

	UINT          i            = 0;
	const wstring materialPath = CPathMgr::GetInst()->GetContentPath();
	for (; i < materialCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
		{
			continue;
		}

		// Material �� ���Ϸ� ����		
		m_vecMtrl[i]->Save(materialPath + m_vecMtrl[i]->GetRelativePath());

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
	LOG_ASSERT(pFile, "FILE OPEN FAILED");

	// Mesh Load
	LoadResPtr<CMesh>(m_pMesh, pFile);
	LOG_ASSERT(m_pMesh.Get(), "MESH NOT FOUND");

	// material ���� �б�
	UINT iMtrlCount{};
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

		Ptr<CMaterial> pMtrl;
		LoadResPtr<CMaterial>(pMtrl, pFile);
		m_vecMtrl[i] = pMtrl;
	}
	fclose(pFile);

	return S_OK;
}
