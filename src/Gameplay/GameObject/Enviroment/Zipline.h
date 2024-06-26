#pragma once

#define ZIPLINE_RENDER_ITERATIONS 5

namespace Pitfall
{
	class Zipline : public GameObject
	{
	public:
		Zipline(const String& name, const float2& begin, const float2& end, float speed);

		// Zipline specific.
		void Attach();
		void Detach();
		bool Attached() const { return m_PlayerAttached; }

		// GameObject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		// Rendering.
		virtual void Render() override;
		virtual bool ShouldRender() const override;

	private:
		// Dynamic.
		float m_TransitionProgress = 0;
		Player* m_Player;
		float2 m_EndPoint;
		float2 m_DetachDirection;
		bool m_PlayerAttached = false;
		float m_MovementSpeed;
		AudioPlayer m_ZiplineSound;

		// Properties.
		const float2 m_AttachmentOffset = float2(0, -1);
	};
}