#pragma once

namespace Pitfall
{
	class ParralaxBackground : public GameObject
	{
	public:
		ParralaxBackground(const String& name);
		~ParralaxBackground();

		// Rendering.
		virtual void Render() override;
		virtual bool ShouldRender() const override { return true; }

		void SetOffset(const int2& offset) { m_BackgroundOffset = offset; }

	private:
		int2 m_BackgroundOffset;

		Surface* m_BakedSurface;
		Sprite* m_BakedSurfaceSprite;
	};
}