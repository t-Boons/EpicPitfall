#pragma once

namespace Pitfall
{
	class Enemy : public GameObject
	{
	public:
			Enemy(const String& name)
			: GameObject(name), m_Player(nullptr)
		{}

		virtual void Hit(const Health& entity) = 0;

	protected:
		Player* m_Player;
	};
}