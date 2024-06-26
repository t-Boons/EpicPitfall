#include "pfpch.h"

namespace Pitfall
{
	void Renderable::BasicRender(GameObject* object)
	{
		const Renderable* rend = object->GetRenderable();

		VISDEBUGONLY(Renderer::WorldDrawRect(rend->m_SpriteRect, RgbColor(225, 70, 170)));
		Renderer::WorldDrawSprite(rend->m_SpriteRect, rend->m_Sprite);
	}

	bool Renderable::BasicShouldRender(const GameObject* object)
	{
		return Renderer::WorldInFrame(object->GetRenderable()->m_SpriteRect);
	}
}