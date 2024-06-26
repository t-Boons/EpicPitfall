#pragma once

namespace Pitfall
{
	class StartButton : public Button
	{
	public:
		StartButton(const String& name, const RectInt& buttonRect);

		virtual void OnClick() override;
	};
}