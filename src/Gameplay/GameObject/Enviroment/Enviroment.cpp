#include "pfpch.h"

namespace Pitfall
{
    Enviroment::Enviroment(const String& name, const TileChunk& chunk, Sprite* sampleSprite, int zIndex)
        : GameObject(name)
    {
        SetPosition(chunk.Position);

        m_Renderable = new Renderable();
        m_Renderable->m_ZIndex = zIndex;
        m_Renderable->m_Sprite = sampleSprite;
        m_Tiles = chunk;
    }

    void Enviroment::Render()
    {
        const int2 screenSize = Renderer::ScreenSize();

        uint32_t i = 0;
		for (int y = 0; y < m_Tiles.Size.y; y++) for (int x = 0; x < m_Tiles.Size.x; x++)
		{
			if (m_Tiles.Tiles[i] != 0)
			{
				const float2 worldPos = float2(Cast<float>(x + GetPosition().x + 1), Cast<float>(m_Tiles.Size.y - y + GetPosition().y ));

				m_Renderable->m_Sprite->SetFrame(m_Tiles.Tiles[i] - 1);
                Renderer::WorldDrawSprite({ worldPos - float2(0.5f, 0.5f), {1, 1} }, m_Renderable->m_Sprite);
			}
			i++;
		}
	}

    bool Enviroment::ShouldRender() const
    {
        return Renderer::WorldInFrame(GetPosition(), GetPosition() + float2(Cast<float>(m_Tiles.Size.x), Cast<float>(m_Tiles.Size.y)));
    }
}