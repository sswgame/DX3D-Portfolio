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
	: CRes(RES_TYPE::MESHDATA)
{
}

CMeshData::~CMeshData()
{
}


CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);

	pNewObj->MeshRender()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetSharedMaterial(m_vecMtrl[i], i);
	}

	// Animation 파트 추가
	if (false == m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}




CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// 메쉬 가져오기
	CMesh* pMesh = nullptr;
	pMesh = CMesh::CreateFromContainer(loader);


	// ResMgr 에 메쉬 등록
	wstring strMeshName = L"mesh\\";
	strMeshName += path(strFullPath).stem();
	strMeshName += L".mesh";

	// 메쉬 이름 설정
	pMesh->SetName(strMeshName);

	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

	vector<Ptr<CMaterial>> vecMtrl;

	// 메테리얼 가져오기
	for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
	{
		// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		vecMtrl.push_back(pMtrl);
	}

	CMeshData* pMeshData = new CMeshData;
	pMeshData->m_pMesh = pMesh;
	pMeshData->m_vecMtrl = vecMtrl;

	return pMeshData;
}



int CMeshData::Save(const wstring& _strFilePath)
{
	SetRelativePath(CPathMgr::GetInst()->GetRelativePath(_strFilePath));

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);

	// Mesh 를 파일로 저장
	wstring strMeshPath = CPathMgr::GetInst()->GetContentPath();
	strMeshPath += m_pMesh->GetRelativePath();
	m_pMesh->Save(strMeshPath);

	// Mesh Key 저장	
	// Mesh Data 저장
	SaveResPtr<CMesh>(m_pMesh, pFile);


	// material 정보 저장
	UINT iMtrlCount = (UINT)m_vecMtrl.size();
	fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	UINT i = 0;
	wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();

	for (; i < iMtrlCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
			continue;

		// Material 을 파일로 저장		
		m_vecMtrl[i]->Save(strMtrlPath + m_vecMtrl[i]->GetRelativePath());

		// Material 인덱스, Key, Path 저장
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveResPtr<CMaterial>(m_vecMtrl[i], pFile);
	}

	i = -1; // 마감 값
	fwrite(&i, sizeof(UINT), 1, pFile);

	fclose(pFile);

	return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	// Mesh Load
	LoadResPtr<CMesh>(m_pMesh, pFile);
	assert(m_pMesh.Get());

	// material 정보 읽기
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, pFile);

	m_vecMtrl.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		fread(&idx, 4, 1, pFile);
		if (idx == -1)
			break;

		wstring strKey, strPath;

		Ptr<CMaterial> pMtrl;
		LoadResPtr<CMaterial>(pMtrl, pFile);

		m_vecMtrl[i] = pMtrl;
	}

	fclose(pFile);

	return S_OK;
}
