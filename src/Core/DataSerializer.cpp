#include "pfpch.h"

#define JsonToWorldPos(value) value / 32.0f

namespace Pitfall
{
	using jsonlist = nlohmann::json_abi_v3_11_2::json;

	namespace Utils
	{
		Object ReadObjectFromJson(const jsonlist& object)
		{

			Object o;

			{
				float oWidth, oHeight, oX, oY;
				object["width"].get_to(oWidth);
				object["height"].get_to(oHeight);
				object["x"].get_to(oX);
				object["y"].get_to(oY);


				std::string name;
				object["name"].get_to(name);

				o.Position = JsonToWorldPos(float2(oX, -oY));
				o.Size = JsonToWorldPos(float2(oWidth, oHeight));
				o.Name = name.c_str();
			}

			{
				if (object.contains("polyline"))
				{
					for (auto& vert : object["polyline"])
					{
						float oX = 0, oY = 0;

						vert["x"].get_to(oX);
						vert["y"].get_to(oY);

						o.PolyLines.Add(JsonToWorldPos(float2(oX, -oY)));
					}
				}
			}

			return o;
		}

		TileChunk ReadChunkFromJson(const jsonlist& chunk, int firstGid)
		{
			int cWidth, cHeight, cX, cY;
			chunk["width"].get_to(cWidth);
			chunk["height"].get_to(cHeight);
			chunk["x"].get_to(cX);
			chunk["y"].get_to(cY);


			std::vector<int> v(cWidth * cHeight);
			chunk["data"].get_to(v);

			// Adjust the tile index from the TilED format to a usable format.
			firstGid--;
			for (auto& vr : v)
			{
				if (vr != 0)
				{
					vr -= firstGid;
				}
			}

			TileChunk c;
			c.Position = { cX, -cY - 16};
			c.Size = { cWidth, cHeight };
			c.Tiles = DynamicList<int>(&v[0], Cast<uint32_t>(v.size()));
			c.Tiles = DynamicList<int>(&v[0], Cast<uint32_t>(v.size()));

			return c;
		}

		ObjectLayer GetObjectLayerByName(const jsonlist& json, const String& name)
		{
			ObjectLayer out;

			for (auto& layer : json["layers"])
			{
				std::string layerName;
				layer["name"].get_to(layerName);

				if (name != layerName.c_str())
				{
					continue;
				}

				DynamicList<Object> objects;

				for (auto& obj : layer["objects"])
				{
					objects.Add(Utils::ReadObjectFromJson(obj));
				}

				out.Objects = objects;
				out.Name = layerName.c_str();
				break;
			}

			return out;
		}

		TileLayer GetEnviromentLayerByName(const jsonlist& json, const String& name, int firstGid)
		{
			TileLayer out;

			for (auto& layer : json["layers"])
			{
				std::string layerName;
				layer["name"].get_to(layerName);

				if (name != layerName.c_str())
				{
					continue;
				}

				DynamicList<TileChunk> layerChunks;

				for (auto& chunk : layer["chunks"])
				{
					layerChunks.Add(Utils::ReadChunkFromJson(chunk, firstGid));
				}

				out.Chunks = layerChunks;
				out.Name = layerName.c_str();
				break;
			}

			return out;
		}
	}

	TileChunk DataSerializer::MapFromCsv(const String& source)
	{
		const String processedSource = source.ReplaceAll('\n', ',');

		const int size = processedSource.ContainsAmountOf(',');

		const uint32_t tilemapHeight = source.ContainsAmountOf('\n');
		const uint32_t tilemapWidth = size / tilemapHeight;

		DynamicList<int> out(size);

		uint32_t offset = 0;

		// Get every int in the string using ',' as its divider.
		uint32_t i = 0;
		while (offset < processedSource.Len())
		{
			const int index = processedSource.FindIndexOf(',', offset);
			const int length = index - offset;

			const String valueStr = processedSource.SubStr(length, offset);
			const int value = String::ToInt(valueStr);

			out[i] = value;
			i++;

			offset += length + 1;
		}

		TileChunk tm;
		tm.Size = int2(tilemapWidth, tilemapHeight);
		tm.Tiles = out;

		return tm;
	}

	DynamicList<GameObject*> DataSerializer::MapFromJson(const String& source)
	{
		using json = nlohmann::json;

		// Resource and Documentation of json library.
		// https://github.com/nlohmann/json

		DynamicList<GameObject*> objects;

		// Read file json.
		std::fstream f(source.Cstr(), fstream::in);
		nlohmann::json j = json::parse(f);
		f.close();

		// Handle tile layers.
		{
			// firstGid is required because of TilED's comperssion system. gID can be found in the exported .json files under tileSets.

			TileLayer collisionLayer = Utils::GetEnviromentLayerByName(j, "Interaction", 257); // 257 is the firstGid
			for (auto& chunk : collisionLayer.Chunks)
			{
				objects.Add(new EnviromentCollision("EnviromentCollisionChunk", chunk));
			}

			TileLayer visualLayer = Utils::GetEnviromentLayerByName(j, "Visual", 1); // 1 is the firstGid
			for (auto& chunk : visualLayer.Chunks)
			{
				objects.Add(new Enviroment("EnviromentVisualLayer", chunk, SpriteLibrary::SpriteFromSurface("Enviroment_Grass"), 1));
			}

			TileLayer visual2Layer = Utils::GetEnviromentLayerByName(j, "Visual2", 265); // 265 is the firstGid
			for (auto& chunk : visual2Layer.Chunks)
			{
				objects.Add(new Enviroment("EnviromentVisual2Layer", chunk, SpriteLibrary::SpriteFromSurface("Enviroment_Props"), 0));
			}
		}

		// Handle object layer.
		{
			ObjectLayer objectLayer = Utils::GetObjectLayerByName(j, "Objects");

			for (auto& obj : objectLayer.Objects)
			{
				if (obj.Name == "PlayerStart")
				{
					objects.Add(new Player("Player", obj.Position));
					continue;
				}

				if (obj.Name == "RollingPig")
				{
					objects.Add(new RollingPig("RollingPig", obj.Position + float2(obj.Size.x, 0), obj.Position.x , obj.Position.x + obj.Size.x));
					continue;
				}

				if (obj.Name == "Monkey")
				{
					objects.Add(new Monkey("Monkey", obj.Position + obj.Size.x, obj.Position.x, obj.Position.x + obj.Size.x));
					continue;
				}

				if (obj.Name == "Zipline")
				{
					objects.Add(new Zipline("Zipline", obj.Position + obj.PolyLines[0], obj.Position + obj.PolyLines[1], 1.0f));
					continue;
				}

				if (obj.Name == "SwingingRope")
				{
					objects.Add(new SwingingRope("SwingingRope", obj.Position + float2(obj.Size.x * 0.5f, -obj.Size .y* 0.5f), obj.Size.y * 0.5f));
					continue;
				}

				if (obj.Name == "ClimbRope")
				{
					objects.Add(new ClimbRope("ClimbRope", { Math::RoundToInt(obj.Position.x), Math::RoundToInt(obj.Position.y - obj.Size.y + 1) }, Cast<int>(obj.Size.y)));
					continue;
				}

				if (obj.Name == "Coin")
				{
					objects.Add(new Coin("Coin", obj.Position));
					continue;
				}

				if (obj.Name == "Finish")
				{
					objects.Add(new Finish("Finish", obj.Position));
					continue;
				}

				if (obj.Name == "Spiderweb")
				{
					objects.Add(new Spiderweb("Spiderweb", { Math::RoundToInt(obj.Position.x + obj.PolyLines[0].x), Math::RoundToInt(obj.Position.y + obj.PolyLines[0].y + 1)}, Math::Abs(obj.PolyLines[0].y + obj.PolyLines[1].y) * 7.5f));
					continue;
				}

				if (obj.Name.Contains("Checkpoint"))
				{
					const int checkpointIndex = String::ToInt(obj.Name.SubStr(obj.Name.Len() - 10, 10));
					objects.Add(new Checkpoint("Checkpoint", obj.Position, checkpointIndex));
					continue;
				}
			}
		}

		return objects;
	}
}