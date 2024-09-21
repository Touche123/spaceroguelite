#pragma once

#include <SFML/Graphics.hpp>
#include "src/entity.h"
#include "eventqueue.h"
#include "combatsystem.h"

class InputSystem {
public:
	InputSystem(EventQueue& eventQueue, CombatSystem& combatSystem) 
		: eventQueue(eventQueue), combatSystem(combatSystem) {}
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
					combatSystem.fireBullet(window, positionComponent->position);
				}
			}
		}
	}

private:
	EventQueue& eventQueue;
	CombatSystem& combatSystem;

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

		//inputComponent->isFiring = sf::Mouse::isButtonPressed(sf::Mouse::Left);
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
};