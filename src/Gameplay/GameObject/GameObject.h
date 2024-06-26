#pragma once

namespace Pitfall
{

	class GameObject
	{
	public:
		GameObject(const String& name);

		virtual ~GameObject();

		friend class World;

		/// <summary>
		/// Called when the world is started.
		/// </summary>
		virtual void Start() {}

		/// <summary>
		/// Called every frame.
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void Tick(float) {}

		/// <summary>
		/// Set GameObject position.
		/// </summary>
		/// <returns></returns>
		float2 GetPosition() const { return m_Position; }

		/// <summary>
		/// Get GameObject position.
		/// </summary>
		/// <param name="position"></param>
		void SetPosition(float2 position) { m_Position = position; }

		/// <summary>
		/// Get GameObject name.
		/// </summary>
		String Name() const { return m_Name; }

		// Component getters.
		Renderable* GetRenderable() { return m_Renderable; }
		AABBCollider* GetAABBCollider() { return m_AABBCollider; }
		PixelCollider* GetPixelCollider() { return m_PixelCollider; }
		TilemapCollider* GetTilemapCollider() { return m_TilemapCollider; }
		RigidBody* GetRigidBody() { return m_RigidBody; }
		const Renderable* GetRenderable() const { return m_Renderable; }
		const AABBCollider* GetAABBCollider() const { return m_AABBCollider; }
		const TilemapCollider* GetTilemapCollider() const { return m_TilemapCollider; }
		const PixelCollider* GetPixelCollider() const { return m_PixelCollider; }
		const RigidBody* GetRigidBody() const { return m_RigidBody; }

		// Renderable specific
		virtual void Render() { Renderable::BasicRender(this); }
		virtual bool ShouldRender() const { return Renderable::BasicShouldRender(this); }
		void UpdateSpriteRectPosition();

		// Physics specific.
		virtual void OnCollision(const CollisionResult& collision) { AABBCollider::BasicOnCollision(this, collision); }
		virtual bool ShouldCalculateCollisions() const { return true; }
		virtual void UpdateColliderPosition();
		virtual void PhysicsUpdate() { RigidBody::BasicPhysicsUpdate(this); }

#pragma region Utilityfunctions
		template<typename InType>
		inline static InType* FindByName(const String& name)
		{
			return Game::Get()->ActiveWorld()->FindObjectByName<InType>(name);
		}

		template<typename InType>
		inline static InType* FindByType()
		{
			return Game::Get()->ActiveWorld()->FindObjectByType<InType>();
		}

		template<typename InType>
		inline static DynamicList<InType*> FindAllByType()
		{
			return Game::Get()->ActiveWorld()->FindObjectsByType<InType>();
		}

		static void Destroy(GameObject* object);

		static void Instantiate(GameObject* object);
#pragma endregion Utilityfunctions

	private:
		// Properties.
		float2 m_Position;
		String m_Name;
		uint32_t m_ID;

	protected:
		// Components.
		Renderable* m_Renderable;
		AABBCollider* m_AABBCollider;
		TilemapCollider* m_TilemapCollider;
		PixelCollider* m_PixelCollider;
		RigidBody* m_RigidBody;
	};
}