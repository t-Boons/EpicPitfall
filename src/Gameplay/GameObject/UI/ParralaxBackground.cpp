#include "pfpch.h"

namespace Pitfall
{
	ParralaxBackground::ParralaxBackground(const String& name)
		: GameObject(name)
	{
		m_Renderable = new Renderable();
		m_Renderable->m_ZIndex = -2;

		Sprite* backgroundSprite = SpriteLibrary::SpriteFromSurface("Parralax_Layer1");
		float2 backgroundSize = { backgroundSprite->GetWidth() * Renderer::RenderSize().x * 2.0f, backgroundSprite->GetHeight() * Renderer::RenderSize().y * 2.0f };
		backgroundSize *= 2;

		// Bake the surface.
		m_BakedSurface = new Surface(Cast<int>(backgroundSize.x), Cast<int>(backgroundSize.y));
		backgroundSprite->DrawScaled(0, 0, m_BakedSurface->width, m_BakedSurface->height, m_BakedSurface);
		m_BakedSurfaceSprite = new Sprite(m_BakedSurface, 1);
	}

	ParralaxBackground::~ParralaxBackground()
	{
		delete m_BakedSurfaceSprite;
		delete m_BakedSurface;
	}

	void ParralaxBackground::Render()
	{
		Surface* screen = Renderer::Screen();
		const int2 spriteSize = { m_BakedSurfaceSprite->GetWidth(), m_BakedSurfaceSprite->GetHeight() };

		// This new implementation makes use of the baked surface/sprite. This is way more optimized than rendering every scaled pixel every frame.
		// Instead because it is a fixed size it runs way faster reducing cpu overhead.

		for (int x = -spriteSize.x; x < screen->width + spriteSize.x; x += spriteSize.x) for (int y = -spriteSize.y; y < screen->height + spriteSize.y; y += spriteSize.y)
		{
			m_BakedSurfaceSprite->Draw(screen, x + m_BackgroundOffset.x % spriteSize.x, y + m_BackgroundOffset.y % spriteSize.y);
		}
	}
}
