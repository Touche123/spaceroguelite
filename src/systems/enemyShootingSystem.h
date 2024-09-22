#pragma once
#include "../entity.h"
#include "../bulletfactory.h"

class EnemyShootingSystem {
public:
	void update(Entity& player, std::vector<Entity>& enemies, BulletFactory& bulletFactory, float deltaTime) {
		auto playerPosition = player.getComponent<PositionComponent>("Position");
		
		if (!playerPosition) return;

		

		for (auto& enemy : enemies) {
			auto playerComponent = enemy.getComponent<PlayerComponent>("Player");
			if (playerComponent)
				continue;
			auto enemyPosition = enemy.getComponent<PositionComponent>("Position");
			auto shootComponent = enemy.getComponent<ShootingComponent>("Shooting");
			auto audioComponent = enemy.getComponent<AudioComponent>("Audio");

			if (enemyPosition && shootComponent && shootComponent->canShoot()) {
				fireBulletAtPlayer(enemyPosition->position, playerPosition->position, bulletFactory);
				if (audioComponent)
					audioComponent->play("Shoot");
				shootComponent->resetCooldown();

				
			}

			shootComponent->updateCooldown(deltaTime);
		}
	}

private:
	void fireBulletAtPlayer(const sf::Vector2f& enemyPosition, const sf::Vector2f& playerPosition, BulletFactory& bulletFactory) {
		sf::Vector2f direction = playerPosition - enemyPosition;
		float magnitude = std::sqrt(direction.x * direction.x + direction.y + direction.y);

		if (magnitude != 0) {
			direction /= magnitude;
		}

		bulletFactory.createEnemyBullet(enemyPosition, direction, 200.f);
	}
};