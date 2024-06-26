#pragma once

namespace Pitfall
{
	class GameObject;

	struct TileChunk
	{
		inline int IndexAt(int2 coordinate) const
		{
			return Tiles[Size.x * (Size.y - coordinate.y) - Size.x + coordinate.x];
		}

		int2 Position;
		int2 Size;
		DynamicList<int> Tiles;
		String Name;
	};

	struct TileLayer
	{
		DynamicList<TileChunk> Chunks;
		String Name;
	};

	struct Object
	{
		float2 Position;
		float2 Size;
		String Name;
		DynamicList<float2> PolyLines;
	};

	struct ObjectLayer
	{
		DynamicList<Object> Objects;
		String Name;
	};

	class DataSerializer
	{
	public:
		static TileChunk MapFromCsv(const String& source);

		static DynamicList<GameObject*> MapFromJson(const String& source);
	};
}