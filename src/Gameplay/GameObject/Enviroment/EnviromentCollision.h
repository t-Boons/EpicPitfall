#pragma once

namespace Pitfall
{
	class EnviromentCollision : public GameObject
	{
	public:
		EnviromentCollision(const String& name, const TileChunk& collisionMap);
	};
}