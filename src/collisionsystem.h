#pragma once
#include "entity.h"
#include <iostream>

class CollisionSystem {
public:
	void checkPlayerEnemyCollisions(Entity& player, std::vector<Entity>& enemies, float deltaTime)
	{
		auto playerCollision = player.getComponent<CollisionComponent>("Collision");
		auto healthComponent = player.getComponent<HealthComponent>("Health");

		if (!playerCollision || !healthComponent)
		{
			return;
		}

		healthComponent->updateCooldown(deltaTime); // Update the player's cooldown timer

		for (const auto& enemy : enemies)
		{
			auto enemyCollision = enemy.getComponent<CollisionComponent>("Collision");

			if (enemyCollision && playerCollision->bounds.intersects(enemyCollision->bounds))
			{
				if (healthComponent->canTakeDamage())
				{
					healthComponent->takeDamage(10.0f); // Damage amount
				}
			}
		}
	}

	void update(std::vector<Entity>& bullets, std::vector<Entity>& enemies) {
		for (auto it = bullets.begin(); it != bullets.end();) {
			auto bulletPosition = it->getComponent<PositionComponent>("Position");
			if (!bulletPosition) {
				++it;
				continue;
			}

			bool bulletHit = false;

			for (auto& enemy : enemies) {
				auto enemyPosition = enemy.getComponent<PositionComponent>("Position");
				auto enemySprite = enemy.getComponent<SpriteComponent>("Sprite");

				if (enemyPosition && enemySprite && checkCollision(bulletPosition->position, enemyPosition->position, enemySprite->sprite.getGlobalBounds())) {
					auto healthComponent = enemy.getComponent<HealthComponent>("Health");

					if (healthComponent)
					{
						healthComponent->takeDamage(2.0f);
					}

					bulletHit = true;
					break;
				}
			}

			if (bulletHit) {
				it = bullets.erase(it);
			} else {
				++it;
			}
		}
	}

private:
	void handleCollision(Entity& entity1, Entity& entity2) {
		// Add collision logic (bounce, damage, etc.)
		auto healthComponent = entity1.getComponent<HealthComponent>("Health");
		if (healthComponent)
		{
			healthComponent->takeDamage(10.f);
			std::cout << "Health: " << healthComponent->health << std::endl;
		}

	}

	bool checkCollision(const sf::Vector2f& bulletPosition, const sf::Vector2f& enemyPosition, const sf::FloatRect& enemyBounds)
	{
		return enemyBounds.contains(bulletPosition);
	}
};