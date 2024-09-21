#pragma once
#include "src/entity.h"

class PhysicsSystem {
public:
	void update(float deltaTime, std::vector<Entity>& entities) {
		for (auto& entity : entities) {
			auto positionComponent = entity.getComponent<PositionComponent>("Position");
			auto velocityComponent = entity.getComponent<VelocityComponent>("Velocity");

			if (positionComponent && velocityComponent) {
				positionComponent->position += velocityComponent->velocity * deltaTime;
			}
		}
	}
};