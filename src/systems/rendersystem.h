#pragma once
#include <SFML/Graphics.hpp>
#include "src/entity.h"

class RenderSystem {
public:
	void update(sf::RenderWindow& window, const std::vector<Entity>& entities) {
		SpriteComponent spr;
		for (const auto& entity : entities) {
			
			auto spriteComponent = entity.getComponent<SpriteComponent>("Sprite");
			auto positionComponent = entity.getComponent<PositionComponent>("Position");
			auto collisionComponent = entity.getComponent<CollisionComponent>("Collision");

			if (spriteComponent && positionComponent) {
				spriteComponent->sprite.setPosition(positionComponent->position);

				if (collisionComponent) {
					collisionComponent->updateBounds(spriteComponent->sprite);
				}
				window.draw(spriteComponent->sprite);
			}
		}
	}

	void drawReticle(sf::RenderWindow& window, Entity& reticle)
	{
		auto spriteComponent = reticle.getComponent<SpriteComponent>("Sprite");
		if (spriteComponent)
		{
			window.draw(spriteComponent->sprite);
		}
	}
};