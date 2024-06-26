#pragma once

namespace Pitfall
{
	class Image : public GameObject
	{
	public:

		Image(const String& name, const RectInt& imageRect, Sprite* sprite, int zIndex = -1);

		virtual void Render() override;

	private:
		RectInt m_ImagePlacement;
	};
}