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

			if (spriteComponent && positionComponent) {
				spriteComponent->sprite.setPosition(positionComponent->position);

				window.draw(spriteComponent->sprite);
			}
		}
	}
};