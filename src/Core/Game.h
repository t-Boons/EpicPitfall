// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

namespace Pitfall
{
	class World;

	class Game : public TheApp
	{
	public:
		Game();

		void Init();
		void Tick(float deltaTime);
		void Shutdown();

		void MouseUp(int);
		void MouseDown(int);
		void MouseMove(int x, int y) { m_MousePos.x = x, m_MousePos.y = y; }
		void MouseWheel(float) {}
		void KeyUp(int key);
		void KeyDown(int key);

		void FlushInput();

		static Game* Get() { return s_Instance; }

		static bool IsKeyPressed(int key);
		static bool IsKeyDown(int key);
		static bool IsKeyUp(int key);
		static int2 MousePosition() { return s_Instance->m_MousePos; }
		static bool IsMousePressed(int button);
		static bool IsMouseDown(int button);
		static bool IsMouseUp(int button);

		Shader* InitializeShader();
		void StartShader();
		void TickShader();

		static World* ActiveWorld() { return s_Instance->m_ActiveWorld; }
		static void SetActiveWorld(World* newWorld) { s_Instance->m_CachedWorld = newWorld; }
		static void SetActiveWorldAfterSeconds(World* newWorld, float seconds);

		static float FrameRate() { return 1.0f / s_Instance->m_DeltaTime; }
			
		static float FrameTime() { return s_Instance->m_FrameTime; }
		static float DeltaTime() { return s_Instance->m_DeltaTime; }
		static float Time() { return s_Instance->m_Time; }

		static void UnloadAndLoadNewWorld(World* newWorld);

	private:
		inline static Game* s_Instance;

		int2 m_MousePos;

		// Input handling.
		uint8_t m_MouseStates = 0;
		uint8_t m_MouseDownStates = 0;
		uint8_t m_MouseUpStates = 0;

		// Time.
		float m_DeltaTime;
		float m_Time;
		float m_FrameTime;

		Shader* m_MainShader;

		// World.
		World* m_ActiveWorld;
		World* m_CachedWorld;
		World* m_CacheAfterSeconds;
		Timer m_CacheAfterSecondsTimer;
		float m_CacheWaitTime;


		uint8_t m_KeyStates[GLFW_KEY_LAST / 8 + 1]{ 0 };
		uint8_t m_KeyDownStates[GLFW_KEY_LAST / 8 + 1]{ 0 };
		uint8_t m_KeyUpStates[GLFW_KEY_LAST / 8 + 1]{ 0 };
	};
}