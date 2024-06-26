#include "pfpch.h"

namespace Pitfall
{
	bool RectInt::ContainsPoint(const int2& point)
	{
		const int2 bottomLeft = Bottomleft();
		const int2 topRight = TopRight();

		return !(bottomLeft.x > point.x || topRight.x < point.x ||
			bottomLeft.y > point.y || topRight.y < point.y);
	}

	bool RectFloat::ContainsPoint(const float2& point) const
	{
		const float2 bottomLeft = Bottomleft();
		const float2 topRight = TopRight();

		return!(bottomLeft.x > point.x ||
			bottomLeft.y < point.y ||
			topRight.x < point.x ||
			topRight.y > point.y);
	}
}