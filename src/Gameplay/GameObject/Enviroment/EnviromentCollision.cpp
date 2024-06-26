#include "pfpch.h"

namespace Pitfall
{
	EnviromentCollision::EnviromentCollision(const String& name, const TileChunk& collisionMap)
		: GameObject(name)
	{
		m_TilemapCollider = new TilemapCollider();
		m_TilemapCollider->m_TileCollisionOffset = collisionMap.Position;
		m_TilemapCollider->m_TileCollisionMap = collisionMap;
	}
}