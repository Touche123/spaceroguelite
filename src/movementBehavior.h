#pragma once
#include "entity.h"

class IMovementBehavior {
public:
	virtual ~IMovementBehavior() = default;
	virtual void update(Entity& entity, float deltaTime) = 0;
};

class StraighLineMovement : public IMovementBehavior {
public:
	void update(Entity& e, float dt) override {
		auto velocityComponent = e.getComponent<VelocityComponent>("Velocity");
		if (velocityComponent) {
			velocityComponent->velocity.y += 100.0f * dt;
		}
	}
};

class FollowPlayerMovement : public IMovementBehavior {
private:
	Entity& player;

public:
	FollowPlayerMovement(Entity& playerEntity) : player(playerEntity) {}

	void update(Entity& e, float dt) override {
		auto velocityComponent = e.getComponent<VelocityComponent>("Velocity");
		auto playerPosition = player.getComponent<PositionComponent>("Position");
		auto enemyPosition = e.getComponent<PositionComponent>("Position");

		if (velocityComponent && playerPosition && enemyPosition) {
			sf::Vector2f direction = playerPosition->position - enemyPosition->position;
			float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (magnitude != 0)
				direction /= magnitude;

			velocityComponent->velocity = direction * 50.f;
		}
	}
};