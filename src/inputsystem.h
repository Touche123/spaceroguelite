#pragma once

#include <SFML/Graphics.hpp>
#include "src/entity.h"
#include "eventqueue.h"

class InputSystem {
public:
	InputSystem(EventQueue& eventQueue) : eventQueue(eventQueue) {}
	void update(sf::RenderWindow& window, std::vector<Entity>& entities, std::vector<Entity>& bullets) {
		for (auto& entity : entities) {
			auto inputComponent = entity.getComponent<InputComponent>("Input");
			auto velocityComponent = entity.getComponent<VelocityComponent>("Velocity");
			auto positionComponent = entity.getComponent<PositionComponent>("Position");

			if (inputComponent && velocityComponent && positionComponent) {
				// Reset movement direction before processing input
				inputComponent->movementDirection = sf::Vector2f(0, 0);

				// Handle keyboard and mouse input
				handleInput(inputComponent);

				// Apply movement direction to velocity
				velocityComponent->velocity = inputComponent->movementDirection * speed;

				// Check if the player is firing
				if (inputComponent->isFiring) {
					// Create a new bullet entity
					fireBullet(window, positionComponent->position);
				}
			}
		}
	}

private:
	EventQueue& eventQueue;

	float speed = 200.0f;

	void handleInput(std::shared_ptr<InputComponent>& inputComponent) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			inputComponent->movementDirection.y = -1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			inputComponent->movementDirection.y = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			inputComponent->movementDirection.x = -1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			inputComponent->movementDirection.x = 1;
		}

		inputComponent->isFiring = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	void fireBullet(sf::RenderWindow& window, const sf::Vector2f& playerPosition)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

		sf::Vector2f direction = mouseWorldPos - playerPosition;
		float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (magnitude != 0)
		{
			direction /= magnitude;
		}

		// Post a fire bullet event to the event queue
		Event bulletEvent = { EventType::FireBullet, playerPosition, direction };
		eventQueue.addEvent(bulletEvent);
	}

	Entity createBullet(const Entity& player, sf::RenderWindow& window) {
		Entity bullet;

		// Retrieve the player's position to spawn the bullet from there
		auto positionComponent = player.getComponent<PositionComponent>("Position");

		// Retrieve the mouse position relative to the game window
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f mosueWorldPos = window.mapPixelToCoords(mousePosition);

		// Calculate the direction vector from player to mouse
		sf::Vector2f direction = mosueWorldPos - positionComponent->position;

		// Normalize the direction vector
		float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (magnitude != 0) {
			direction /= magnitude; // Normalize direction
		}

		// Create components for the bullet
		auto bulletPosition = std::make_shared<PositionComponent>();
		bulletPosition->position = positionComponent->position; // Spawn the bullet at player's position

		auto bulletVelocity = std::make_shared<BulletComponent>();
		bulletVelocity->velocity = direction * 400.f; // Set bullet velocity in the direction of the mouse

		sf::Texture bulletTexture;
		bulletTexture.loadFromFile("assets/textures/player.png");
		auto spriteComponent = std::make_shared<SpriteComponent>();
		spriteComponent->sprite.setTexture(bulletTexture);

		bullet.addComponent("Position", bulletPosition);
		bullet.addComponent("Bullet", bulletVelocity);
		bullet.addComponent("Sprite", spriteComponent);

		return bullet;
	}
};