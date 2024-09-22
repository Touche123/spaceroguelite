#pragma once
#include <vector>
#include "../entity.h"
#include <queue>

class EntitySystem {
public:
	Entity* getEntityById(int id) {
		auto it = std::find_if(entities.begin(), entities.end(),
			[id](const Entity& entity) {
				return entity.getId() == id;
			});

		if (it != entities.end())
			return &(*it);

		return nullptr;
	}

	void AddEntity(Entity& e)
	{
		e.id = generateUniqueId();
		entities.push_back(e);
	}

	std::vector<Entity>& GetEntities() {
		return entities;
	}

	std::vector<Entity> GetEntities() const
	{
		return entities;
	}

	int GetEntitiesCount()
	{
		return entities.size();
	}

	void markForRemoval(int id) {
		auto it = std::find_if(entities.begin(), entities.end(),
			[id](const Entity& entity) {
				return entity.getId() == id;
			});

		if (it != entities.end()) {
			it->marked_for_removal = true;
		}
	}

	void remove_marked_for_removal()
	{
		// Use a loop to ensure all marked entities are removed
		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const Entity& entity) {
				return entity.marked_for_removal; // Check if marked for removal
			}), entities.end());

		// Reset the marked_for_removal flag for all entities
		for (auto& entity : entities)
		{
			entity.marked_for_removal = false; // Reset after removal
		}
	}
	

private:
	std::vector<Entity> entities;

	std::queue<int> freeIds; // Queue to manage reusable IDs
	int nextId = 0; // Next ID to assign if freeIds is empty

	int generateUniqueId()
	{
		if (!freeIds.empty())
		{
			int id = freeIds.front();
			freeIds.pop();
			return id;
		}
		return nextId++;
	}

	void removeEntityById(int id)
	{
		auto it = std::remove_if(entities.begin(), entities.end(),
			[id](const Entity& entity) {
				return entity.getId() == id;
			});

		if (it != entities.end())
		{
			entities.erase(it, entities.end());
			std::cout << "Entity removed: " << id << std::endl;
		} else
		{
			std::cout << "Entity not found: " << id << std::endl;
		}
	}
};