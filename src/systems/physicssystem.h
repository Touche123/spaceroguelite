#pragma once
#include "src/entity.h"

class PhysicsSystem {
public:
	void update(float deltaTime, EntitySystem entitySystem) {
		for (auto& entity : entitySystem.GetEntities()) {
			auto positionComponent = entity.getComponent<PositionComponent>("Position");
			auto velocityComponent = entity.getComponent<VelocityComponent>("Velocity");

			if (positionComponent && velocityComponent) {
				positionComponent->position += velocityComponent->velocity * deltaTime;
			}
		}
	}
};