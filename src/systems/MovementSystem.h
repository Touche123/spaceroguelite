#pragma once
#include "../entity.h"
#include "../movementBehavior.h"

class MovementSystem {
public:
	void update(EntitySystem entitySystem, float dt) {
		for (auto& e : entitySystem.GetEntities()) {
			auto velocityComponent = e.getComponent<VelocityComponent>("Velocity");
			auto postionComponent = e.getComponent<PositionComponent>("Position");
			auto movementBehavior = e.getComponent<BehaviorComponent>("MovementBehavior");

			if (velocityComponent && postionComponent && movementBehavior) {
				movementBehavior->behavior->update(e, dt);

				postionComponent->position += velocityComponent->velocity * dt;
			}
		}
	}
};