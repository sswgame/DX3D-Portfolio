#pragma once
#include <Engine/CScript.h>

class UIImageScript;

enum class PROGRESS_DIR
{
	RIGHT_TO_LEFT,
	LEFT_TO_RIGHT,
	TOP_TO_BOTTOM,
	BOTTOM_TO_TOP,

	END
};

class UIProgressBarScript final
	: public CScript
{
private:
	float        m_percentage;
	PROGRESS_DIR m_dir;

	UIImageScript* m_pImageScript;
	std::wstring   m_textureName;
public:
	float GetPercent() const { return m_percentage; }
	void  SetPercent(float value);
	void  SetDir(PROGRESS_DIR dir) { m_dir = dir; }
	void  SetTexture(const std::wstring& texturePath) { m_textureName = texturePath; }
public:
	void start() override;
	void lateupdate() override;
public:
	CLONE(UIProgressBarScript);
	UIProgressBarScript();
	virtual ~UIProgressBarScript();
};
