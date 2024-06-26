#include "pfpch.h"

namespace Pitfall
{
	BackToMainmenuButton::BackToMainmenuButton(const String& name, const RectInt& buttonRect)
		: Button(name, buttonRect, SpriteLibrary::SpriteFromSurface("Button_BackToMainMenu"))
	{
	}

	void BackToMainmenuButton::OnClick()
	{
		Game::SetActiveWorldAfterSeconds(new TitleScreen(), FADE_SPEED);
	}
}