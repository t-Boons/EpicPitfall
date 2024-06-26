#include "pfpch.h"

namespace Pitfall
{
	BoxCollider::BoxCollider(const String& name, const RectFloat& rect)
		: GameObject(name)
	{
		SetPosition(rect.Pos);
		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB = Utils::RectToAABB(rect);
	}
}