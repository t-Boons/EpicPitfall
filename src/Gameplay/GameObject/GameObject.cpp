#include "pfpch.h"

namespace Pitfall
{
	GameObject::GameObject(const String& name)
		: m_Name(name), m_Position(float2(0, 0)),
		m_Renderable(nullptr), m_TilemapCollider(nullptr), m_RigidBody(nullptr), m_AABBCollider(nullptr), m_PixelCollider(nullptr)
	{
		Print((String("Created object: ") + m_Name).Cstr());
	}

	GameObject::~GameObject()
	{
		Print((String("Destroyed object: ") + m_Name).Cstr());

		// Free all object components memory.
		delete m_Renderable;
		delete m_AABBCollider;
		delete m_PixelCollider;
		m_PixelCollider = nullptr;
		delete m_TilemapCollider;
		delete m_RigidBody;
	}

	void GameObject::UpdateSpriteRectPosition()
	{
		m_Renderable->m_SpriteRect.Pos = m_Position + m_Renderable->m_SpriteOffset;
	}

	void GameObject::UpdateColliderPosition()
	{
		if(m_AABBCollider)
			AABBCollider::BasicUpdateColliderPosition(this);

		if (m_PixelCollider)
			PixelCollider::BasicUpdateColliderPosition(this);

	}

	void GameObject::Destroy(GameObject* object)
	{
		return Game::Get()->ActiveWorld()->Remove(object);
	}

	void GameObject::Instantiate(GameObject* object)
	{
		return Game::Get()->ActiveWorld()->Add(object);
	}
}