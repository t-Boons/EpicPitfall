#include "pfpch.h"

namespace Pitfall
{
	Image::Image(const String& name, const RectInt& imageRect, Sprite* sprite, int zIndex)
		: GameObject(name), m_ImagePlacement(imageRect)
	{
		m_Renderable = new Renderable();
		m_Renderable->m_Sprite = sprite;
		m_Renderable->m_ZIndex = zIndex;
	}

	void Image::Render()
	{
		Renderer::ScreenDrawSprite(m_ImagePlacement, m_Renderable->m_Sprite);
	}
}