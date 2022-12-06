#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"

CResMgr::CResMgr() {}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Del_Map(m_Res[i]);
	}
}

void CResMgr::SaveChangedRes()
{
	wstring strContent = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, CRes*>::iterator iter = m_Res[i].begin();
		for (; iter != m_Res[i].end(); ++iter)
		{
			if (iter->second->IsChanged())
			{
				iter->second->Save(strContent + iter->second->GetRelativePath());
			}
		}
	}
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey,
                                     UINT           _iWidth,
                                     UINT           _iHeight,
                                     DXGI_FORMAT    _format,
                                     UINT           _flag,
                                     bool           _bEngineRes)
{
	assert(nullptr == FindRes<CTexture>(_strKey));

	CTexture* pTexture = new CTexture;

	pTexture->Create(_iWidth, _iHeight, _format, _flag);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture, _bEngineRes);

	return pTexture;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D, bool _bEngineRes)
{
	assert(nullptr == FindRes<CTexture>(_strKey));


	CTexture* pTexture = new CTexture;

	pTexture->Create(_pTex2D);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture, _bEngineRes);

	return pTexture;
}

Ptr<CTexture> CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath, int _iMapLevel)
{
	CTexture* pRes = FindRes<CTexture>(_strKey).Get();
	if (nullptr != pRes)
	{
		return pRes;
	}

	pRes = new CTexture;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	if (FAILED(pRes->Load(strFilePath, _iMapLevel)))
	{
		return nullptr;
	}

	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strRelativePath);

	m_Res[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pRes));

	return pRes;
}

vector<Ptr<CMeshData>> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = std::filesystem::path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	//if (nullptr != pMeshData)
	//	return pMeshData;

	vector<CMeshData*>     vecMeshData;
	vector<Ptr<CMeshData>> ptrMeshData;

	vecMeshData = CMeshData::LoadFromFBX(_strPath);
	for (int i = 0; i < vecMeshData.size(); ++i)
	{
		ptrMeshData.push_back(vecMeshData[i]);
		wstring strName     = L"meshdata\\";
		wstring strFileName = std::filesystem::path(_strPath).stem();
		string  strNum      = std::to_string(i);
		strFileName += wstring(strNum.begin(), strNum.end());
		strName += strFileName + L".mdat";

		ptrMeshData[i]->SetKey(strName);
		ptrMeshData[i]->SetRelativePath(strName);
		m_Res[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, ptrMeshData[i].Get()));
	}
	//pMeshData->m_bEngineRes = true;


	return ptrMeshData;
}

void CResMgr::DeleteRes(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter;
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		iter = m_Res[i].find(_strKey);
		if (iter != m_Res[i].end())
		{
			SAFE_DELETE(iter->second);
			m_Res[i].erase(iter);
			return;
		}
	}

	assert(nullptr);
}
