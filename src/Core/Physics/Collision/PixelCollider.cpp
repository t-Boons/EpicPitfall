#include "pfpch.h"

namespace Pitfall
{
	void PixelCollider::BasicUpdateColliderPosition(GameObject* object)
	{
		object->GetPixelCollider()->m_Bounds.Set(object->GetPosition());
	}

	bool PixelCollider::Collides(const AABB& otherAABB) const
	{
		const AABB& aabb = m_Bounds;
		const Sprite* sp = m_Renderable->m_Sprite;

		int xIndStart = Math::FloorToInt((otherAABB.Min.x - aabb.Min.x) / m_StepSize.x);
		int xIndEnd = sp->GetWidth() - Math::FloorToInt((aabb.Max.x - otherAABB.Max.x) / m_StepSize.x);

		int yIndStart = Math::FloorToInt((aabb.Max.y - otherAABB.Max.y) / m_StepSize.y);
		int yIndEnd = sp->GetHeight() - Math::FloorToInt((otherAABB.Min.y - aabb.Min.y) / m_StepSize.y);

		xIndStart = Math::Clamp(xIndStart, 0, sp->GetWidth());
		xIndEnd = Math::Clamp(xIndEnd, 0, sp->GetWidth());
		yIndStart = Math::Clamp(yIndStart, 0, sp->GetHeight());
		yIndEnd = Math::Clamp(yIndEnd, 0, sp->GetHeight());

		for (int x = xIndStart; x < xIndEnd; x++)
		{
			for (int y = yIndStart; y < yIndEnd; y++)
			{
				const uint col = sp->ColorAtPixel(m_Flipped ? sp->GetWidth() - 1 - x : x, y);
				if (col >> 24)
				{
					return true;
				}
			}
		}

		return false;
	}

    bool PixelCollider::Collides(const PixelCollider* other) const
    {
		// Early out test.
		if (!m_Bounds.Collides(other->m_Bounds))
		{
			VISDEBUGONLY(Renderer::WorldDrawBox(m_Bounds.Min, m_Bounds.Max, RgbColor(225, 0, 0)));
			return false;
		}


		const Sprite* oSpr = other->m_Renderable->m_Sprite;

		VISDEBUGONLY(
			// Calculate AABB collision for every pixel that is not transparent.
			for (int y = 0; y < oSpr->GetHeight(); y++) for (int x = 0; x < oSpr->GetWidth(); x++)
			{
				// Check if the color on x y is transparent.
				if (oSpr->ColorAtPixel(other->m_Flipped ? oSpr->GetWidth() - 1 - x : x, y) >> 24)
				{
					// If it isn't calculate if the aabb at the pixel position collides with any of the other pixels.
					AABB aabb = other->AABBFromPixelIndex({ x, y });
					VISDEBUGONLY(Renderer::WorldDrawBox(aabb.Min, aabb.Max, RgbColor(225, 0, 225)));
					if (Collides(aabb))
					{
						VISDEBUGONLY(Renderer::WorldDrawBox(aabb.Min, aabb.Max, RgbColor(225, 0, 0)));
					}
				}
			}
		)

		// Big: O(n2)

		// Calculate AABB collision for every pixel that is not transparent.
		for (int y = 0; y < oSpr->GetHeight(); y++) for (int x = 0; x < oSpr->GetWidth(); x++) // O(n2)
		{
			// Check if the color on x y is transparent.
			if (oSpr->ColorAtPixel(other->m_Flipped ? oSpr->GetWidth() - 1 - x : x, y) >> 24)  // O(1)
			{
				// If it isn't calculate if the aabb at the pixel position collides with any of the other pixels.
				if (Collides(other->AABBFromPixelIndex({ x, y }))) // O(1)
				{
					return true;
				}
			}
		}
		return false;
	}

	void PixelCollider::Init(const Renderable* renderable, const AABB& bounds)
	{
		m_Renderable = renderable;
		m_Bounds = bounds;

		const Sprite* spr = m_Renderable->m_Sprite;
		m_StepSize.x = (bounds.Max.y - bounds.Min.y) / spr->GetHeight();
		m_StepSize.y = (bounds.Max.x - bounds.Min.x) / spr->GetWidth();
	}

	AABB PixelCollider::AABBFromPixelIndex(const int2& index) const
	{
		float2 pos
		{
			m_Bounds.Min.x + m_StepSize.x * Cast<float>(index.x),
			m_Bounds.Max.y - m_StepSize.y * (index.y + 1)
		};

		pos += m_Renderable->m_SpriteOffset;

		AABB out;
		out.Min = pos;
		out.Max = pos + m_StepSize;

		return out;
	}
}