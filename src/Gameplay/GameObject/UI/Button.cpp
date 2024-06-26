#include "pfpch.h"
#include "Button.h"

namespace Pitfall
{
	Button::Button(const String& name, const RectInt& buttonRect, Sprite* sprite, int zIndex)
		: GameObject(name), m_ButtonPlacement(buttonRect), m_IsHovering(false), m_IsPressing(false)
	{
		m_Renderable = new Renderable();
		m_Renderable->m_Sprite = sprite;
		m_Renderable->m_ZIndex = zIndex;
	}

	void Button::Tick(float)
	{
		m_IsHovering = m_ButtonPlacement.ContainsPoint(Game::MousePosition());

		m_IsPressing = (m_IsHovering && Game::IsMousePressed(GLFW_MOUSE_BUTTON_1));

		m_Renderable->m_Sprite->SetFrame(BUTTON_NORMAL);

		if (m_IsHovering)
		{
			m_Renderable->m_Sprite->SetFrame(BUTTON_HOVER);
			OnHover();
		}

		if (m_IsPressing)
		{
			m_Renderable->m_Sprite->SetFrame(BUTTON_PRESS);
			OnPressDown();
		}

		if (m_IsHovering && Game::IsMouseUp(GLFW_MOUSE_BUTTON_1))
		{
			OnClick();
		}
	}

	void Button::Render()
	{
		Renderer::ScreenDrawSprite(m_ButtonPlacement, m_Renderable->m_Sprite);
	}
}