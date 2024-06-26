#pragma once

#define MAX_RENDERABLES 256
#define PIXELS_PER_METER 32
#define CIRCLE_RENDERING_SEGMENTS 32

namespace Pitfall
{
	class Camera;
	class Renderable;

	struct RendererProperties
	{
	public:
		Shader* MainShader;
		Surface* Screen;
	};

	class Renderer
	{
	public:
		static void Init(const RendererProperties& properties);

		static void SetActiveCamera(Camera* camera) { s_Instance->m_Camera = camera; }

		static void Render();

		static void Clear(const uint32_t color);

		static void Add(GameObject* renderable);
		static void Remove(GameObject* renderable);
		static void Reset();

		static void Sort();

		static int2 WorldToScreenSpace(const float2& position);
		static int2 WorldToScreenSize(const float2& size);

		static int WorldToScreenSize(const float size);
		static RectInt WorldRectToScreenRect(const RectFloat& rect);

		static void ShaderStart();
		static void ShaderTick();

		static void	WorldDrawBox(const float2& bottomLeft, const float2& topRight, const uint32_t color);
		static void	WorldDrawRect(const RectFloat& rect, const uint32_t color);
		static void	WorldDrawSprite(const RectFloat& rect, const Sprite* sprite);
		static void	WorldDrawBoxSolid(const float2& bottomLeft, const float2& topRight, const uint32_t color);
		static void	WorldDrawLine(const float2& a, const float2& b, const uint32_t color);
		static void	WorldDrawCircle(const float2& point, const float radius, uint32_t color);

		static void ScreenDrawBox(const int2& bottomLeft, const int2& topRight, const uint32_t color);
		static void ScreenDrawRect(const RectInt& rect, const uint32_t color);
		static void ScreenDrawSprite(const RectInt& rect, const Sprite* sprite);
		static void ScreenDrawBoxSolid(const int2& bottomLeft, const int2& topRight, const uint32_t color);
		static void ScreenDrawLine(const int2& a, const int2& b, const uint32_t color);
		static void ScreenDrawCircle(const int2& point, const uint32_t radius, const uint32_t color);

		inline static float2 RenderSize() { return s_Instance->m_RenderScale; }
		inline static RendererProperties GetProperties() { return s_Instance->m_Properties; }

		inline static Surface* Screen() { return s_Instance->m_Properties.Screen; }
		inline static int2 ScreenSize() { return s_Instance->m_ScreenSize; }
		inline static float2 HalfScreenSize() { return s_Instance->m_HalfScreenSize; }
		inline static float2 HalfWorldSize() { return s_Instance->m_HalfWorldSize; }
		inline static float2 WorldViewPosition();
		inline static int2 FlipVerticalScreenSpace(const int2& value);

		inline static int2 ScreenPercentage(const float2& percentage) { return int2(Cast<int>(Cast<float>(ScreenSize().x / 1.0f) * percentage.x), Cast<int>(Cast<float>(ScreenSize().y / 1.0f) * percentage.y)); }

		static bool WorldInFrame(const float2& bottomLeft, const float2& topRight);
		static bool ScreenInFrame(const int2& bottomLeft, const int2& topRight);
		static bool WorldInFrame(const RectFloat& rect);
		static bool ScreenInFrame(const RectInt& rect);

		static bool WorldFullyInFrame(const float2& bottomLeft, const float2& topRight);
		static bool ScreenFullyInFrame(const int2& bottomLeft, const int2& topRight);
		static bool WorldFullyInFrame(const RectFloat& rect);
		static bool ScreenFullyInFrame(const RectInt& rect);

	private:
		static Renderer* s_Instance;

		RendererProperties m_Properties;
		float2 m_HalfScreenSize, m_HalfWorldSize;
		int2 m_ScreenSize;
		float m_RenderScale, m_ResolutionMultiplier;

		Camera* m_Camera;

		StaticList<GameObject*, MAX_RENDERABLES> m_RenderList;
	};
}