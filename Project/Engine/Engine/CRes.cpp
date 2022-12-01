#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _eType)
	: m_eResType(_eType)
	, m_iRefCount(0)
	, m_bEngineRes(false)
	, m_bChanged(false) {}

CRes::~CRes() = default;

bool CRes::CheckFail(HRESULT _hr)
{
	if (FAILED(_hr))
	{
		const int iErrorCode  = GetLastError();
		wchar_t   szText[256] = {};
		wsprintf(szText, L"에러코드 : %d", iErrorCode);
		MessageBox(nullptr, szText, L"텍스쳐 로딩 실패!!", MB_OK);
		return true;
	}
	return false;
}
