#pragma once
#include "src/entity.h"

class CollisionSystem {
public:
	void update(std::vector<Entity>& entities) {
		for (size_t i = 0; i < entities.size(); ++i) {
			auto collider1 = entities[i].getComponent<CollisionComponent>("Collision");
			if (!collider1) continue;

			for (size_t j = i + 1; j < entities.size(); ++j) {
				auto collider2 = entities[j].getComponent<CollisionComponent>("Collision");
				if (!collider2) continue;

				// Check if the bounds intersect
				if (collider1->bounds.intersects(collider2->bounds)) {
					handleCollision(entities[i], entities[j]);
				}
			}
		}
	}

private:
	void handleCollision(Entity& entity1, Entity& entity2) {
		// Add collision logic (bounce, damage, etc.)
	}
};