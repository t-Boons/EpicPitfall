#pragma once

#define MAX_ENTITY_COUNT 256

namespace Pitfall
{
	class World
	{
	public:
		/// <summary>
		/// Runs when loading the world.
		/// </summary>
		virtual void Load();

		void Add(GameObject* object);

		void Remove(GameObject* object);

		/// <summary>
		/// Runs every frame.
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void Tick(float deltaTime);

		/// <summary>
		/// Runs when world is unloaded.
		/// </summary>
		virtual void Unload();

		template <typename InType>
		InType* FindObjectByName(const String& name) const
		{
			for (GameObject* obj : m_Objects)
			{
				if (obj && obj->Name() == name)
				{
					return TryCast<InType*>(obj);
				}
			}

			Print("Object with name can not be found in level");
			return nullptr;
		}

		template <typename InType>
		InType* FindObjectByType() const
		{
			for (GameObject* obj : m_Objects)
			{
				if (obj)
				{
					InType* out = TryCast<InType*>(obj);

					if (out)
					{
						return out;
					}
				}
			}

			Print("Object of type can not be found in level");
			return nullptr;
		}

		template <typename InType>
		DynamicList<InType*> FindObjectsByType() const
		{
			DynamicList<InType*> list;
			for (GameObject* obj : m_Objects)
			{
				if (obj)
				{
					InType* out = TryCast<InType*>(obj);

					if (out)
					{
						list.Add(out);
					}
				}
			}

			return list;
		}

	private:
		void AddToSystems(GameObject* object);

		void RemoveFromSystems(GameObject* object);

	protected:
		StaticList<GameObject*, MAX_ENTITY_COUNT> m_Objects{};
		StaticList<GameObject*, MAX_ENTITY_COUNT> m_InstantiationQueue{};
		StaticList<GameObject*, MAX_ENTITY_COUNT> m_DeletionQueue{};
		uint32_t m_ObjectIndex;
		uint32_t m_InstantiableObjectCount, m_DeletableObjectCount;
		bool m_Killed;
	};
}