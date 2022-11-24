#pragma once
#include <Engine/CScript.h>
#include <bitset>

class CUIBase;

enum class CLICK_TYPE
{
	ONE,
	DOUBLE,
	RELEASE,
	DRAG,

	END
};

enum class MOUSE_TYPE
{
	LEFT,
	MIDDLE,
	RIGHT,

	END
};

class ButtonScript final
	: public CScript
{
private:
	float                             m_clickTerm;
	std::bitset<(int)MOUSE_TYPE::END> m_prevClick;

	using ClickFunc = std::array<std::function<void()>, (int)CLICK_TYPE::END>;
	std::array<ClickFunc, (int)MOUSE_TYPE::END> m_arrCallback;

	CUIBase* m_pUIBase;

private:
	bool DoubleClick(MOUSE_TYPE mouseType, CLICK_TYPE clickType);
	bool OneClick(MOUSE_TYPE mouseType, CLICK_TYPE clickType);
	void HandleClickEvent();

	void HandleDragEvent();
	void HandleReleaseEvent();
public:
	void start() override;
	void lateupdate() override;
public:
	template <typename T>
	void SetCallback(T* pInstance, void (T::*func)(), MOUSE_TYPE mouseType, CLICK_TYPE clickType)
	{
		m_arrCallback[static_cast<int>(mouseType)][static_cast<int>(clickType)] = std::bind(func, pInstance);
	}

public:
	CLONE(ButtonScript);
	ButtonScript();
	virtual ~ButtonScript();
};