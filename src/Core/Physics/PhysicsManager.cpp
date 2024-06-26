#include "pfpch.h"

namespace Pitfall
{
	PhysicsManager* PhysicsManager::s_Instance = new PhysicsManager();

	void PhysicsManager::Add(GameObject* object)
	{
		for (uint32_t i = 0; i < s_Instance->m_PhysicsObjectCount; i++)
		{
			if (s_Instance->m_PhysicsObjects[i] == object)
			{
				Print("Duplicate object added to PhysicaManager");
				return;
			}
		}

		s_Instance->m_PhysicsObjects[s_Instance->m_PhysicsObjectCount] = object;
		s_Instance->m_PhysicsObjectCount++;

		Assert(s_Instance->m_PhysicsObjectCount < MAX_COLLIDERS, "Physics objects list overflow.");
	}

	void PhysicsManager::Remove(GameObject* object)
	{
		for (uint32_t i = 0; i < s_Instance->m_PhysicsObjectCount; i++)
		{
			if (object == s_Instance->m_PhysicsObjects[i])
			{
				// This was Jacco's 1 on 1 feedback tip. Take the last object in the array and put it in the position of the removed object. 
				s_Instance->m_PhysicsObjects[i] = s_Instance->m_PhysicsObjects[s_Instance->m_PhysicsObjectCount - 1];
				s_Instance->m_PhysicsObjects[s_Instance->m_PhysicsObjectCount - 1] = nullptr;

				s_Instance->m_PhysicsObjectCount--;
				return;
			}
		}

		Print("Physics object could not be found");
	}

	void PhysicsManager::Reset()
	{
		s_Instance->m_PhysicsObjectCount = 0;
	}

	void PhysicsManager::ResolveCollisions()
	{
		auto& pList = s_Instance->m_PhysicsObjects;
		auto pSize = s_Instance->m_PhysicsObjectCount;

		// Tile > AABB collision.
		for (uint32_t i = 0; i < pSize; i++)
		{

			GameObject* receiver = pList[i];
			if (!receiver) continue;

			AABBCollider* receiverCol = pList[i]->GetAABBCollider();
			if (!receiverCol || receiverCol->m_IsStatic || !receiver->ShouldCalculateCollisions()) continue;

			// Check against all tilemaps that have collision enabled.
			for (uint32_t j = 0; j < pSize; j++)
			{
				// Don't check AABB collision for the receiver itself.
				if (i == j)
				{
					continue;
				}

				GameObject* comparer = pList[j];
				if (!comparer) continue;

				TilemapCollider* comparerCol = pList[j]->GetTilemapCollider();

				if (!comparer || !comparerCol || !comparer->ShouldCalculateCollisions() ||
					Utils::HasMatchingBit(receiverCol->GetCollissionIgnoreLayers(), TILEMAP_COLLISION_LAYER))
				{
					continue;
				}

				receiver->UpdateColliderPosition();

				CollisionResult result;
				for (uint32_t k = 0; k < MAX_COLLISION_SOLVE_ATTEMPTS; k++)
				{
					if (CompareAABBToTile(receiverCol, comparerCol, result))
					{
						result.Object = pList[j];
						receiver->OnCollision(result);

						// GameObject position might be corrected so we have to update it again.
						receiver->UpdateColliderPosition();

						if (receiverCol->m_IsTrigger)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}

		// AABB -> AABB collision.
		for (uint32_t i = 0; i < pSize; i++)
		{
			GameObject* receiver = pList[i];
			if (!receiver) continue;

			AABBCollider* receiverCol = pList[i]->GetAABBCollider();
			if (!receiverCol || receiverCol->m_IsStatic || !receiver->ShouldCalculateCollisions()) continue;

			for (uint32_t j = 0; j < pSize; j++)
			{
				// Don't check AABB collision for the receiver itself.
				if (i == j)
				{
					continue;
				}

				GameObject* comparer = pList[j];
				if (!comparer) continue;

				AABBCollider* comparerCol = pList[j]->GetAABBCollider();

				// Check if both objects are obligated to process collision.
				if (!comparerCol || !comparer->ShouldCalculateCollisions() ||
					Utils::HasMatchingBit(receiverCol->GetCollissionIgnoreLayers(), comparerCol->GetCollisionLayer()) ||
					Utils::HasMatchingBit(comparerCol->GetCollissionIgnoreLayers(), receiverCol->GetCollisionLayer()))
				{
					continue;
				}

				receiver->UpdateColliderPosition();
				comparer->UpdateColliderPosition();

				for (uint32_t k = 0; k < MAX_COLLISION_SOLVE_ATTEMPTS; k++)
				{
					CollisionResult result;
					if (CompareAABBAABB(receiverCol, comparerCol, result))
					{
						result.Object = pList[j];
						receiver->OnCollision(result);

						// Don't bother updating collision positions if it is a trigger the object collided with.
						if (result.IsTrigger || receiverCol->m_IsTrigger)
						{
							break;
						}

						// GameObject position might be corrected so we have to update it again.
						receiver->UpdateColliderPosition();
						comparer->UpdateColliderPosition();
					}
					else
					{
						break;
					}
				}
			}
		}

		// Pixel -> Pixel collision.
		for (uint32_t i = 0; i < pSize; i++)
		{
			GameObject* receiver = pList[i];
			if (!receiver) continue;

			PixelCollider* receiverCol = pList[i]->GetPixelCollider();
			if (!receiverCol) continue;

			for (uint32_t j = 0; j < pSize; j++)
			{
				// Don't check collision for the receiver itself.
				if (i == j)
				{
					continue;
				}

				GameObject* comparer = pList[j];
				if (!comparer) continue;

				PixelCollider* comparerCol = pList[j]->GetPixelCollider();

				if (!comparerCol)
				{
					continue;
				}

				receiver->UpdateColliderPosition();
				comparer->UpdateColliderPosition();

				CollisionResult result;
				if (ComparePixelPixel(receiverCol, comparerCol, result))
				{
					result.Object = pList[j];
					receiver->OnCollision(result);
					result.Object = pList[i];
					comparer->OnCollision(result);
				}
			}
		}
	}

	void PhysicsManager::ResolveVelocities()
	{
		for (uint32_t i = 0; i < s_Instance->m_PhysicsObjectCount; i++)
		{
			if (s_Instance->m_PhysicsObjects[i]->GetRigidBody())
			{
				s_Instance->m_PhysicsObjects[i]->PhysicsUpdate();
			}
		}
	}

	bool PhysicsManager::CompareAABBToTile(const AABBCollider* square, const TilemapCollider* tile, CollisionResult& result)
	{
		VISDEBUGONLY(Renderer::WorldDrawBox(square->m_AABB.Min, square->m_AABB.Max, RgbColor(0, 0, 225)));

		int2 tileIndex;
		if (tile->Collides(square, tileIndex))
		{
			const float2 tileMin = { Cast<float>(tileIndex.x), Cast<float>(tileIndex.y) };
			const float2 tileMax = { Cast<float>(tileIndex.x + 1), Cast<float>(tileIndex.y + 1) };

			float2 overlap;
			if (tile->Overlap(square, tileIndex, overlap))
			{
				result.IsTrigger = false;
				result.Overlap = overlap;
				result.Normal = -Math::Normalize(overlap);
				result.CollisionType = CollisionType::Tile;
				result.TileIndex = tile->m_TileCollisionMap.IndexAt(tileIndex);

				return true;
			}
		}

		return false;
	}

	bool PhysicsManager::ComparePixelPixel(const PixelCollider* pixel1, const PixelCollider* pixel2, CollisionResult& result)
	{
		if (pixel1->Collides(pixel2))
		{
			result.IsTrigger = true;
			result.CollisionType = CollisionType::Pixel;
			return true;
		}

		return false;
	}

	bool PhysicsManager::CompareAABBAABB(const AABBCollider* square1, const AABBCollider* square2, CollisionResult& result)
	{
		VISDEBUGONLY(Renderer::WorldDrawBox(square1->m_AABB.Min, square1->m_AABB.Max, RgbColor(0, 225, 225)));
		VISDEBUGONLY(Renderer::WorldDrawBox(square2->m_AABB.Min, square2->m_AABB.Max, RgbColor(0, 225, 225)));

		if (square1->m_AABB.Collides(square2->m_AABB))
		{
			float2 overlap;
			if (square1->Overlap(square2, overlap))
			{
				result.IsTrigger = square2->m_IsTrigger;
				result.Overlap = overlap;
				result.Normal = -Math::Normalize(overlap);
				result.CollisionType = CollisionType::AABB;

				return true;
			}
		}

		return false;
	}
}