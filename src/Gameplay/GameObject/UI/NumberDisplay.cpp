#include "pfpch.h"

namespace Pitfall
{
	NumberDisplay::NumberDisplay(const String& name, const int2& position)
		: GameObject(name)
	{
		m_Renderable = new Renderable();
		m_Renderable->m_ZIndex = 3;

		m_NumRenderer.SetPosition(position);
		m_NumRenderer.SetSize({64, 128});
		m_NumRenderer.SetSpacing(50);
		m_NumRenderer.SetNumberFont(SpriteLibrary::SpriteFromSurface("Font_Numbers"));
	}
	void NumberDisplay::Render()
	{
		m_NumRenderer.Render();
	}
}