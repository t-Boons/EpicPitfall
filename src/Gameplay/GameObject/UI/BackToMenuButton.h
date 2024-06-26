#pragma once

namespace Pitfall
{
	class BackToMainmenuButton : public Button
	{
	public:
		BackToMainmenuButton(const String& name, const RectInt& buttonRect);

		virtual void OnClick() override;
	};
}