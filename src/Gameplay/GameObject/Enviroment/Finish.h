#pragma once

namespace Pitfall
{
	class Finish : public GameObject
	{
	public:
		Finish(const String& name, const float2& position);

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		bool m_Finished;
	};
}