#include "pch.h"
#include "UIImageScript.h"

UIImageScript::UIImageScript()
	: CUIBase{SCRIPT_TYPE::UIIMAGESCRIPT, UI_TYPE::IMAGE}
	, m_useInfo{0}
	, m_preserveRatio{false}
	, m_ratio{1.f}
{
}

UIImageScript::~UIImageScript() = default;

void UIImageScript::update()
{
	if (m_preserveRatio)
	{
		if (m_useInfo)
		{
			m_ratio           = m_currentInfo.imageSize.x / m_currentInfo.imageSize.y;
			Vec3 currentScale = Transform()->GetRelativeScale();
			currentScale.y    = currentScale.x / m_ratio;
			Transform()->SetRelativeScale(currentScale);
		}
		else
		{
			Ptr<CTexture> pTexture = MeshRender()->GetDynamicMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
			if (nullptr != pTexture)
			{
				m_ratio           = pTexture->Width() / pTexture->Height();
				Vec3 currentScale = Transform()->GetRelativeScale();
				currentScale.y    = currentScale.x / m_ratio;
				Transform()->SetRelativeScale(currentScale);
			}
		}
	}
	CUIBase::update();
}

void UIImageScript::lateupdate()
{
	CUIBase::lateupdate();

	const Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(m_textureName);
	assert(nullptr != pTexture);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	if (m_useInfo)
	{
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &m_useInfo);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_currentInfo);
	}
}

void UIImageScript::Clear()
{
	m_mapInfo.clear();
	m_useInfo = false;
}

void UIImageScript::AddImageInfo(const std::wstring& frameName, const Vec2& pos, const Vec2& size)
{
	const auto iter = FindImageInfo(frameName);
	assert(iter == m_mapInfo.end());
	m_mapInfo.insert({frameName, tImageInfo{pos, size}});
}

tImageInfo UIImageScript::GetImageInfo(const std::wstring& frameName)
{
	const auto iter = FindImageInfo(frameName);
	assert(iter != m_mapInfo.end());
	return iter->second;
}

void UIImageScript::SetImageInfo(const std::wstring& frameName)
{
	m_currentInfo = GetImageInfo(frameName);
	m_useInfo     = true;
}

void UIImageScript::SetPreserveRatio(bool enable)
{
	m_preserveRatio = enable;
}
