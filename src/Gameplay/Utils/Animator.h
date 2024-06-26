#pragma once

/// The reason for using DyamicList instead of StaticList is that animations are only added during object initialization. And having 2 big arrays that are mostly empty is a waste of memory.
/// Especially since the performance overhead of reallocating the dynamic arrays during initialization is not that severe.

namespace Pitfall
{
	class Animator
	{
	public:
		Animator()
			: m_FrameIndex(0), m_Speed(0), m_OneShot(false),
			  m_Looping(false), m_ActiveAnimation(""), m_AnimationRenderer(nullptr)
		{}

		void SetSubject(Renderable* animRenderer);
		void SetAnimationDirection(int direction);
		void SetSpeed(float speed);
		void Tick();
		void AddAnimation(const String& name, Sprite* sprite);
		void SetAnimation(const String& name, bool looping = true);
		void SetLooping(bool looping) { m_Looping = looping; }
		void OneShot() {m_OneShot = true; }
		void Clear() { m_OneShot = false; }

	private:
		// References.
		DynamicList<String> m_AnimationNames;
		DynamicList<Sprite*> m_AnimationSprites;
		Renderable* m_AnimationRenderer;

		// Dynamic.
		String m_ActiveAnimation;
		Timer m_AnimTimer;
		uint32_t m_FrameIndex;


		// Properties.
		float m_Speed;
		bool m_Looping;
		bool m_OneShot;
	};
}