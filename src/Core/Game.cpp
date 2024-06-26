// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023	

#include "pfpch.h"

namespace Pitfall
{
	Game::Game()
		: m_MousePos(int2(0,0)), m_MainShader(nullptr), m_ActiveWorld(nullptr), m_Time(0), m_CacheAfterSeconds(nullptr)
	{
		s_Instance = this;
	}

	void Game::Init()
	{
		RendererProperties props;
		props.MainShader = m_MainShader;
		props.Screen = screen;
		Renderer::Init(props);

		// Load all surfaces in to memory.

		// Enviroment.
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enviroment/Enviroment_Tileset_Grass_F256.png"), "Enviroment_Grass", 256);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enviroment/Enviroment_Tileset_Props_F1024.png"), "Enviroment_Props", 1024);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enviroment/Background_F1.png"), "Parralax_Layer1", 1);

		// Player animations.
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Run_F6.png"), "Player_Run", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Walk_F6.png"), "Player_Walk", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Climb_F6.png"), "Player_Climb", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Idle_F4.png"), "Player_Idle", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Jump_F4.png"), "Player_Jump", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Fall_F2.png"), "Player_Fall", 2);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Attack1_F6.png"), "Player_Attack1", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Player/Player_Hurt_F3.png"), "Player_Hurt", 3);

		// Rollingpig animations.
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/RollingPig_Walk_F6.png"), "RollingPig_Walk", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/RollingPig_Attack_F4.png"), "RollingPig_Attack", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/RollingPig_Idle_F4.png"), "RollingPig_Idle", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/RollingPig_Hurt_F2.png"), "RollingPig_Hurt", 2);

		// Monkey animations.
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/Monkey_Walk_F6.png"), "Monkey_Walk", 6);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/Monkey_Attack_F4.png"), "Monkey_Attack", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/Monkey_Idle_F4.png"), "Monkey_Idle", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Enemies/Monkey_Hurt_F2.png"), "Monkey_Hurt", 2);

		// Misc gameplay
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Throwable_Stick_F5.png"), "Throwable_Stick", 5);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Coin_F4.png"), "Coin", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Rock_F4.png"), "Rock", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Vine_F2.png"), "Vine", 2);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Spiderweb_Bounce_F4.png"), "Spiderweb_Bounce", 4);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Spiderweb_Idle_F1.png"), "Spiderweb_Idle", 1);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Checkpoint_F4.png"), "Checkpoint", 4);

		// Fonts
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/Font/Font_Numbers_F10.png"), "Font_Numbers", 10);
		
		// UI
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/UI/Button_Start_F3.png"), "Button_Start", 3);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/UI/Button_BackToMainMenu_F3.png"), "Button_BackToMainMenu", 3);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/UI/Background_TitleScreen_F1.png"), "Background_TitleScreen", 1);
		SpriteLibrary::AddSurface(new Surface("Assets/Visuals/UI/Background_EndScreen_F1.png"), "Background_EndScreen", 1);


		SetActiveWorld(new TitleScreen());
	}

	void Game::Tick(float deltaTime)
	{
		// Update frame timing stuff.
		m_FrameTime = deltaTime;
		const float convertedDt = Math::Min(deltaTime * 0.001f, 0.2f);
		m_DeltaTime = convertedDt;
		m_Time += m_DeltaTime;

		// World stuff.
		// Clearing is not neccesary when using a parralax background.
		//Renderer::Clear(RgbColor(10, 10, 10));

		// Check if new world should be loaded.
		if (m_CacheAfterSeconds && m_CacheAfterSecondsTimer.elapsed() > m_CacheWaitTime)
		{
			m_CachedWorld = m_CacheAfterSeconds;
			m_CacheAfterSeconds = nullptr;
		}


		// Reload world if there is a new world cached.
		if (m_ActiveWorld != m_CachedWorld)
		{
			UnloadAndLoadNewWorld(m_CachedWorld);

			// Fade in camera.
			Camera* c = s_Instance->ActiveWorld()->FindObjectByType<Camera>(); if (c) c->FadeIn(FADE_SPEED);
		}

		m_ActiveWorld->Tick(m_DeltaTime);


		PhysicsManager::ResolveVelocities();
		PhysicsManager::ResolveCollisions();
		Renderer::Render();


		FlushInput();


		// Renderer fps with minifont.
		//char buffer[20];
		//sprintf_s(buffer, "%f", FrameRate());
		//Renderer::Screen()->PrintText(buffer, 2, 2, RgbColor(225, 225, 225));
	}

	void Game::Shutdown()
	{
		m_ActiveWorld->Unload();
		delete m_ActiveWorld;
	}

	// Resource for setting individual bits.
	// Reason for using bits instead of booleans for checking input is that a boolean cost more memory.
	// https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit#:~:text=To%20toggle%20a%20bit%2C%20we,want%20to%20toggle%20and%201.
	// https://stackoverflow.com/questions/13054401/how-does-condition-statement-work-with-bit-wise-operators 
	// Char codes found at:
	// https://www.learncpp.com/cpp-tutorial/chars/

	void Game::KeyUp(int key)
	{
		m_KeyStates[key / 8] ^= (1 << (key % 8));
		m_KeyUpStates[key / 8] ^= (1 << (key % 8));
	}

	void Game::KeyDown(int key)
	{
		m_KeyStates[key / 8] ^= (1 << (key % 8));
		m_KeyDownStates[key / 8] ^= (1 << (key % 8));
	}

	void Game::MouseUp(int button)
	{
		m_MouseStates ^= (1 << button);
		m_MouseUpStates ^= (1 << button);
	}

	void Game::MouseDown(int button)
	{
		m_MouseStates ^= (1 << button);
		m_MouseDownStates ^= (1 << button);
	}

	void Game::FlushInput()
	{
		memset(m_KeyUpStates, 0x0, sizeof(m_KeyStates));
		memset(m_KeyDownStates, 0x0, sizeof(m_KeyStates));
		m_MouseDownStates = 0;
		m_MouseUpStates = 0;
	}

	bool Game::IsKeyPressed(int key)
	{
		return s_Instance->m_KeyStates[key / 8] & 1 << (key % 8);
	}

	bool Game::IsKeyDown(int key)
	{
		return s_Instance->m_KeyDownStates[key / 8] & 1 << (key % 8);
	}

	bool Game::IsKeyUp(int key)
	{
		return s_Instance->m_KeyUpStates[key / 8] & 1 << (key % 8);
	}

	bool Game::IsMousePressed(int button)
	{
		return s_Instance->m_MouseStates & (1 << button);
	}

	bool Game::IsMouseDown(int button)
	{
		return s_Instance->m_MouseDownStates & (1 << button);
	}

	bool Game::IsMouseUp(int button)
	{
		return s_Instance->m_MouseUpStates & (1 << button);
	}

	/* pressed
	0011 keys
	0010 input

	0010 &

	*/

	/* not pressed
	0001 keys
	0010 input

	0000 &
	*/

	Shader* Game::InitializeShader()
	{
		m_MainShader = new Shader(TextFileRead("Assets/Visuals/Shaders/MainShaderVertex.glsl").c_str(), TextFileRead("Assets/Visuals/Shaders/MainShaderFragment.glsl").c_str(), true);
		return m_MainShader;
	}

	void Game::StartShader()
	{
		Renderer::ShaderStart();
	}

	void Game::TickShader()
	{
		Renderer::ShaderTick();
	}

	void Game::SetActiveWorldAfterSeconds(World* newWorld, float seconds)
	{
		s_Instance->m_CacheAfterSeconds = newWorld;
		s_Instance->m_CacheAfterSecondsTimer.reset();
		s_Instance->m_CacheWaitTime = seconds;

		// Fade out camera.
		Camera* c = s_Instance->ActiveWorld()->FindObjectByType<Camera>(); if (c) c->FadeOut(FADE_SPEED);
	}

	void Game::UnloadAndLoadNewWorld(World* newWorld)
	{
		if (s_Instance->m_ActiveWorld)
		{
			s_Instance->m_ActiveWorld->Unload();
			delete s_Instance->m_ActiveWorld;
			s_Instance->m_ActiveWorld = nullptr;
		}

		s_Instance->m_ActiveWorld = newWorld;
		s_Instance->m_ActiveWorld->Load();
		Renderer::SetActiveCamera(s_Instance->m_ActiveWorld->FindObjectByType<Camera>());
	}
}