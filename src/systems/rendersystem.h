#pragma once
#include <SFML/Graphics.hpp>
#include "src/entity.h"

class RenderSystem {
public:
	void update(sf::RenderWindow& window, const Entity& entity)
	{
		auto spriteComponent = entity.getComponent<SpriteComponent>("Sprite");
		auto positionComponent = entity.getComponent<PositionComponent>("Position");
		auto collisionComponent = entity.getComponent<CollisionComponent>("Collision");
		if (spriteComponent && positionComponent)
		{
			spriteComponent->sprite.setPosition(positionComponent->position);

			if (collisionComponent)
			{
				collisionComponent->updateBounds(spriteComponent->sprite);
			}
			window.draw(spriteComponent->sprite);
		}
	}

	void update(sf::RenderWindow& window, const std::vector<Entity>& entity)
	{
		SpriteComponent spr;
		for (const auto& entity : entity)
		{

			auto spriteComponent = entity.getComponent<SpriteComponent>("Sprite");
			auto positionComponent = entity.getComponent<PositionComponent>("Position");
			auto collisionComponent = entity.getComponent<CollisionComponent>("Collision");

			if (spriteComponent && positionComponent)
			{
				spriteComponent->sprite.setPosition(positionComponent->position);

				if (collisionComponent)
				{
					collisionComponent->updateBounds(spriteComponent->sprite);
				}
				window.draw(spriteComponent->sprite);
			}
		}
	}

	void update(sf::RenderWindow& window, const EntitySystem& entitySystem) {
		SpriteComponent spr;
		for (const auto& entity : entitySystem.GetEntities()) {
			
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