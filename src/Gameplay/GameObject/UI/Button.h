#pragma once

// Frame numbers for the buttons. Spritesheets should also follow these frame numbers.
#define BUTTON_NORMAL 0
#define BUTTON_HOVER 1
#define BUTTON_PRESS 2


namespace Pitfall
{
	class Button : public GameObject
	{
	public:
		Button(const String& name, const RectInt& buttonRect, Sprite* sprite, int zIndex = 0);

		virtual void Tick(float) override;
		virtual void Render() override;
		virtual bool ShouldRender() const override { return true; }

		virtual void OnClick() {}
		virtual void OnHover() {}
		virtual void OnPressDown() {}

	private:
		RectInt m_ButtonPlacement;
		bool m_IsHovering;
		bool m_IsPressing;
	};
}