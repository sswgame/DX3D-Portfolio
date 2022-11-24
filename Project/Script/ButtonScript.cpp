#include "pch.h"
#include "ButtonScript.h"

#include "UIImageScript.h"

namespace BUTTON
{
	inline constexpr float DEFAULT_TERM = 0.3f;
}

ButtonScript::ButtonScript()
	: CScript{(int)SCRIPT_TYPE::BUTTONSCRIPT}
	, m_clickTerm{BUTTON::DEFAULT_TERM}
	, m_pUIBase{nullptr}
{
}

ButtonScript::~ButtonScript() = default;


void ButtonScript::start()
{
	m_pUIBase = GetOwner()->GetScript<CUIBase>();
	assert(m_pUIBase);
}

void ButtonScript::lateupdate()
{
	m_clickTerm -= DT;

	if (m_pUIBase->IsMouseHovered())
	{
		HandleClickEvent();
		HandleDragEvent();
		HandleReleaseEvent();
	}

	if (m_clickTerm <= 0.f)
	{
		m_prevClick.reset();
	}
}

bool ButtonScript::DoubleClick(MOUSE_TYPE mouseType, CLICK_TYPE clickType)
{
	if (m_prevClick[static_cast<int>(mouseType)]
	    && m_arrCallback[static_cast<int>(mouseType)][static_cast<int>(clickType)])
	{
		m_arrCallback[static_cast<int>(mouseType)][static_cast<int>(clickType)]();
		return true;
	}
	return false;
}

bool ButtonScript::OneClick(MOUSE_TYPE mouseType, CLICK_TYPE clickType)
{
	if (m_arrCallback[static_cast<int>(mouseType)][static_cast<int>(clickType)])
	{
		m_arrCallback[static_cast<int>(mouseType)][static_cast<int>(clickType)]();
		return true;
	}
	return false;
}

void ButtonScript::HandleClickEvent()
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (DoubleClick(MOUSE_TYPE::LEFT, CLICK_TYPE::DOUBLE))
		{
			return;
		}
		OneClick(MOUSE_TYPE::LEFT, CLICK_TYPE::ONE);

		m_prevClick.reset();
		m_prevClick[static_cast<int>(MOUSE_TYPE::LEFT)] = true;
		m_clickTerm                                     = BUTTON::DEFAULT_TERM;
	}
	else if (KEY_TAP(KEY::MBTN))
	{
		if (DoubleClick(MOUSE_TYPE::MIDDLE, CLICK_TYPE::DOUBLE))
		{
			return;
		}
		OneClick(MOUSE_TYPE::MIDDLE, CLICK_TYPE::ONE);

		m_prevClick.reset();
		m_prevClick[static_cast<int>(MOUSE_TYPE::MIDDLE)] = true;
		m_clickTerm                                       = BUTTON::DEFAULT_TERM;
	}
	else if (KEY_TAP(KEY::RBTN))
	{
		if (DoubleClick(MOUSE_TYPE::RIGHT, CLICK_TYPE::DOUBLE))
		{
			return;
		}
		OneClick(MOUSE_TYPE::RIGHT, CLICK_TYPE::ONE);

		m_prevClick.reset();
		m_prevClick[static_cast<int>(MOUSE_TYPE::RIGHT)] = true;
		m_clickTerm                                      = BUTTON::DEFAULT_TERM;
	}
}

void ButtonScript::HandleDragEvent()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::LEFT)][static_cast<int>(CLICK_TYPE::DRAG)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::LEFT)][static_cast<int>(CLICK_TYPE::DRAG)]();
		}
	}
	else if (KEY_PRESSED(KEY::MBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::MIDDLE)][static_cast<int>(CLICK_TYPE::DRAG)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::MIDDLE)][static_cast<int>(CLICK_TYPE::DRAG)]();
		}
	}
	else if (KEY_PRESSED(KEY::RBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::RIGHT)][static_cast<int>(CLICK_TYPE::DRAG)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::RIGHT)][static_cast<int>(CLICK_TYPE::DRAG)]();
		}
	}
}

void ButtonScript::HandleReleaseEvent()
{
	if (KEY_AWAY(KEY::LBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::LEFT)][static_cast<int>(CLICK_TYPE::RELEASE)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::LEFT)][static_cast<int>(CLICK_TYPE::RELEASE)]();
		}
	}
	else if (KEY_AWAY(KEY::MBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::MIDDLE)][static_cast<int>(CLICK_TYPE::RELEASE)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::MIDDLE)][static_cast<int>(CLICK_TYPE::RELEASE)]();
		}
	}
	else if (KEY_AWAY(KEY::RBTN))
	{
		if (m_arrCallback[static_cast<int>(MOUSE_TYPE::RIGHT)][static_cast<int>(CLICK_TYPE::RELEASE)])
		{
			m_arrCallback[static_cast<int>(MOUSE_TYPE::RIGHT)][static_cast<int>(CLICK_TYPE::RELEASE)]();
		}
	}
}
