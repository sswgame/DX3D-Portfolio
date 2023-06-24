#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr() = default;

CPathMgr::~CPathMgr() = default;

void CPathMgr::init()
{
	const std::filesystem::path currentDirectory = std::filesystem::current_path();
	const std::filesystem::path parentDirectory  = currentDirectory.parent_path();
	m_strContentPath                             = parentDirectory.wstring() + L"\\bin\\content\\";
	LOG_TRACE("CONTENT PATH : %s", ToString(m_strContentPath).c_str());
}

std::wstring CPathMgr::GetRelativePath(const wstring& _strFilePath) const
{
	const size_t iPos = _strFilePath.rfind(m_strContentPath);
	if (iPos != _strFilePath.npos)
	{
		//iPos�� 0�̾���Ѵ�.(������ ���� ��θ� �������� ã�� ����
		assert(iPos==0);
		return _strFilePath.substr(m_strContentPath.length());
	}
	assert(nullptr && "NOT CONTAIN RELATIVE PATH");
	return std::wstring{};
}
