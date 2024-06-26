#pragma once

namespace Pitfall
{
	struct Health
	{
	public:
		Health(int health = 0, int attackDamage = 0)
			: HP(health), ATK(attackDamage)
		{}

		int HP;
		int ATK;
	};
}