#include "pfpch.h"

namespace Pitfall
{
	void World::Load() // Big: O(n)
	{
		m_Killed = false;

		m_ObjectIndex = 0;
		m_InstantiableObjectCount = 0;
		m_DeletableObjectCount = 0;

		// Add all objects Interfaces to the systems and call load on the gameobjects.
		for (uint32_t i = 0; i < m_Objects.Size(); i++) // O(n)
		{
			if (m_Objects[i])
			{
				m_ObjectIndex++;
				AddToSystems(m_Objects[i]);
				m_Objects[i]->m_ID = m_ObjectIndex;
			}
		}

		Renderer::Sort();

		// Call start on all objects.
		for (uint32_t i = 0; i < m_ObjectIndex; i++) // O(n)
		{
			m_Objects[i]->Start();
		}
	}

	void World::Add(GameObject* object) // Big: O(1)
	{
		m_InstantiationQueue[m_InstantiableObjectCount++] = object; // O(1)
	}

	void World::Remove(GameObject* object)  // Big: O(1) / O(2)
	{
		// Check if object is not in the deletion queue already.
		for (uint32_t i = 0; i < m_DeletableObjectCount; i++) // O(n)
		{
			if (m_DeletionQueue[i] == object)
			{
				return;
			}
		}

		// Add object to deletion queue.
		m_DeletionQueue[m_DeletableObjectCount++] = object; // O(1)
	}

	void World::AddToSystems(GameObject* object)
	{
		Renderer::Add(object);
		PhysicsManager::Add(object);
	}

	void World::RemoveFromSystems(GameObject* object)
	{
		Renderer::Remove(object);
		PhysicsManager::Remove(object);
	}

	void World::Tick(float deltaTime)
	{

		// Big: O(1)

		// Delete old objects.
		while (m_DeletableObjectCount > 0)
		{
			GameObject* toFree = m_DeletionQueue[--m_DeletableObjectCount];	 // O(1)
			RemoveFromSystems(toFree);						// O(1)

			GameObject* last = m_Objects[m_ObjectIndex - 1];

			// Fill in an existing gameObject in its place.
			m_Objects[toFree->m_ID - 1] = last;				// O(1)
			last->m_ID = toFree->m_ID;				        // O(1) 

			m_Objects[m_ObjectIndex - 1] = nullptr;

			m_ObjectIndex--;								// O(1)

			delete toFree;
		}



		// Big: O(1)

		// Instantiate new objects.
		bool updatedObjects = m_InstantiableObjectCount > 0;
		while (m_InstantiableObjectCount > 0)
		{
			GameObject* toAdd = m_InstantiationQueue[--m_InstantiableObjectCount];  // O(1)
			AddToSystems(toAdd); // O(1)

			// Add the object and set its ID.
			m_Objects[m_ObjectIndex] = toAdd;				   // O(1)
			toAdd->m_ID = m_ObjectIndex + 1;				   // O(1)

			m_ObjectIndex++;								   // O(1)

			toAdd->Start();
		}

		// Sort every added object by Z-Index if any are added.
		if(updatedObjects) 	Renderer::Sort();


		// Big: O(n)

		// Call tick on all objects if the scene isnt being unloaded.
		for (uint32_t i = 0; i < m_ObjectIndex; i++) // O(n)
		{
			if (m_Killed)
			{
				return;
			}

			m_Objects[i]->Tick(deltaTime);
		}
	}

	// OLD IMMPLEMENTATION //
	//void World::Tick(float deltaTime)
	//{
	//
	//	// Big: O(nm)
	//
	//	// Delete old objects.
	//	if (m_ContainsDeletableObjects)
	//	{
	//		m_ContainsDeletableObjects = false;
	//
	//		// Loop through the deletionqueue.
	//		for (uint32_t i = 0; i < m_DeletionQueue.Size(); i++)   // O(nm)
	//		{
	//			if (m_DeletionQueue[i])
	//			{
	//				RemoveFromSystems(m_DeletionQueue[i]);		    // O(1)
	//
	//
	//				// Remove from object list.
	//				for (uint32_t j = 0; j < m_Objects.Size(); j++)
	//				{
	//					if (m_DeletionQueue[i] == m_Objects[j])
	//					{
	//						m_Objects[j] = nullptr;				    // O(1)
	//						break;
	//					}
	//				}
	//
	//				delete m_DeletionQueue[i];
	//				m_DeletionQueue[i] = nullptr;
	//			}
	//		}
	//	}
	//
	//	// Big: O(nm)
	//
	//	// Instantiate new objects.
	//	if (m_ContainsInstantiableObjects)
	//	{
	//		m_ContainsInstantiableObjects = true;
	//
	//		// Loop through the instantiationqueue.
	//		for (uint32_t i = 0; i < m_InstantiationQueue.Size(); i++) // O(nm)
	//		{
	//			if (m_InstantiationQueue[i])
	//			{
	//				AddToSystems(m_InstantiationQueue[i]);			   // O(1)
	//
	//				for (uint32_t j = 0; j < m_Objects.Size(); j++)
	//				{
	//					if (!m_Objects[j])
	//					{
	//						m_Objects[j] = m_InstantiationQueue[i];	   // O(1)
	//						break;
	//					}
	//				}
	//
	//				m_InstantiationQueue[i]->Start();
	//
	//				m_InstantiationQueue[i] = nullptr;				   // O(1)
	//			}
	//		}
	//
	//		// Sort z-index after all objects are added.
	//		Renderer::Sort();
	//	}
	//
	//
	//	// Big: O(n)
	//
	//	// Call tick on all objects if the scene isnt being unloaded.
	//	for (uint32_t i = 0; i < m_Objects.Size(); i++) // O(n)
	//	{
	//		if (m_Killed)
	//		{
	//			return;
	//		}
	//
	//		if (m_Objects[i])
	//		{
	//			m_Objects[i]->Tick(deltaTime);
	//		}
	//	}
	//}

	void World::Unload() // Big; O(n)
	{
		m_Killed = true;

		Renderer::Reset();
		PhysicsManager::Reset();

		// This used to be a foreach loop. But because I was changing its contents mid loop I channged it.
		for (uint32_t i = 0; i < m_ObjectIndex; i++) // O(n)
		{
			if (m_Objects[i])
			{
				delete m_Objects[i];
				m_Objects[i] = nullptr;
			}
		}
	}
}