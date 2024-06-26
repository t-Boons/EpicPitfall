#pragma once

namespace Pitfall
{
	class BoxCollider : public GameObject
	{
	public:
		BoxCollider(const String& name, const RectFloat& rect);
	};
}