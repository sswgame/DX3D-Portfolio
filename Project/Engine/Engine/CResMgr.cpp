#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"

CResMgr::CResMgr() = default;

CResMgr::~CResMgr()
{
	for (size_t i = 0; i < std::size(m_Res); ++i)
	{
		Safe_Del_Map(m_Res[i]);
	}
}

void CResMgr::SaveChangedRes()
{
	const wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (size_t i = 0; i < std::size(m_Res); ++i)
	{
		for (const auto& [key, pResource] : m_Res[i])
		{
			if (pResource->IsChanged())
			{
				const std::wstring strFullPath = strContentPath + pResource->GetRelativePath();
				pResource->Save(strFullPath);
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

	pRes                = new CTexture{};
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	if (FAILED(pRes->Load(strFilePath, _iMapLevel)))
	{
		return nullptr;
	}

	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strRelativePath);
	m_Res[(UINT)RES_TYPE::TEXTURE].insert({_strKey, pRes});

	return pRes;
}


vector<Ptr<CMeshData>> CResMgr::LoadFBX(const wstring& _strPath)
{
	const std::wstring strFileName     = std::filesystem::path(_strPath).stem();
	const std::wstring strRelativePath = L"meshdata\\" + strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strRelativePath);
	//if (nullptr != pMeshData)
	//	return pMeshData;

	vector<Ptr<CMeshData>> vecResult{};

	const vector<CMeshData*> vecMeshData = CMeshData::LoadFromFBX(_strPath);
	for (size_t i = 0; i < vecMeshData.size(); ++i)
	{
		wstring meshFileName = std::filesystem::path(_strPath).stem();
		wstring strName      = L"meshdata\\" + meshFileName + std::to_wstring(i) + L".mdat";

		vecResult.push_back(vecMeshData[i]);
		vecResult[i]->SetKey(strName);
		vecResult[i]->SetRelativePath(strName);
		m_Res[static_cast<UINT>(RES_TYPE::MESHDATA)].insert({strName, vecResult[i].Get()});
	}

	return vecResult;
}

void CResMgr::DeleteRes(const wstring& _strKey)
{
	for (size_t i = 0; i < std::size(m_Res); ++i)
	{
		const auto iter = m_Res[i].find(_strKey);
		if (iter != m_Res[i].end())
		{
			SAFE_DELETE(iter->second);
			m_Res[i].erase(iter);

			return;
		}
	}

	assert(nullptr);
}
