#pragma once

namespace Pitfall
{
	struct RectInt
	{
	public:
		RectInt()
		{}

		RectInt(const int2& pos, const int2& size)
			: Pos(pos), Size(size)
		{}

		inline int2 Bottomleft() const { return Pos + int2(-Math::Abs(Size.x) / 2, -Math::Abs(Size.y) / 2); }
		inline int2 TopRight() const { return Pos + int2(Math::Abs(Size.x) / 2, Math::Abs(Size.y) / 2); }

		bool ContainsPoint(const int2& point);

		int2 Pos;
		int2 Size;
	};

	struct RectFloat
	{
	public:
		RectFloat()
		{}

		RectFloat(const float2& pos, const float2& size)
			: Pos(pos), Size(size)
		{}

		inline float2 Bottomleft() const { return Pos + float2(-Math::Abs(Size.x) * 0.5f, -Math::Abs(Size.y) * 0.5f); }
		inline float2 TopRight() const { return Pos + float2(Math::Abs(Size.x) * 0.5f, Math::Abs(Size.y) * 0.5f); }


		bool ContainsPoint(const float2& point) const;

		float2 Pos;
		float2 Size;
	};
}