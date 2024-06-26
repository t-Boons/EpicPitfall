#pragma once

namespace Pitfall
{
	class Player;
	class ParralaxBackground;

	class Camera : public GameObject
	{
	public:
		Camera(const String& name);

		// GameObject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		void ObjectToFollow(const GameObject* toFollow) { m_ToFollow = toFollow; }

		void ShaderStart();
		void ShaderTick();
		float GetRenderSize() const { return m_RenderSize; }
		void SetRenderSize(float renderSize) { m_RenderSize = renderSize; }

		// Effects.
		void PlayPixelationEffect();
		void FadeIn(float seconds);
		void FadeOut(float seconds);

	private:
		const GameObject* m_ToFollow = nullptr;
		ParralaxBackground* m_Background = nullptr;
		float2 m_OffsetPosition = float2(0, 0);
		float m_LookDirection = 0.0f;
		float m_RenderSize = 1.0f;

		float m_Pixelation;
		float m_FadeBrightness;
		float m_FadeDelta;
	};
}