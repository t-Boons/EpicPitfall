#include "pfpch.h"

namespace Pitfall
{
	void EndScreen::Load()
	{
		m_Objects[0] = new Camera("Camera");
		m_Objects[1] = new BackToMainmenuButton("BackToMainmenuButton", { Renderer::ScreenPercentage({0.5f, 0.75f}), Renderer::ScreenPercentage({0.2f, 0.1f}) });
		m_Objects[2] = new Image("Background_EndScreen", { Renderer::ScreenPercentage({0.5f, 0.5f}), Renderer::ScreenSize() }, SpriteLibrary::SpriteFromSurface("Background_EndScreen"));
		
		// Reset checkpoints.
		if (File::Exists(CHECKPOINT_FILEPATH)) File::Write({ CHECKPOINT_FILEPATH , "0" });
		World::Load();
	}
}