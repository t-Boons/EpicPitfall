#include "pfpch.h"
#include "StartButton.h"

namespace Pitfall
{
	StartButton::StartButton(const String& name, const RectInt& buttonRect)
		: Button(name, buttonRect, SpriteLibrary::SpriteFromSurface("Button_Start"))
	{
	}

	void StartButton::OnClick()
	{
		Game::SetActiveWorldAfterSeconds(new Level01(), FADE_SPEED);
	}
}