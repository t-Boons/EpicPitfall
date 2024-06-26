#pragma once

namespace Pitfall
{
	class Enviroment : public GameObject
	{
	public:
		Enviroment(const String& name, const TileChunk& chunk, Sprite* sampleSprite, int zIndex);

		virtual void Render() override;

		virtual bool ShouldRender() const override;
	private:
		TileChunk m_Tiles;
	};
}