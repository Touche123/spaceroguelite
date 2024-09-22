#pragma once

#include <SFML/Graphics.hpp>
#include "src/entity.h"
#include "../eventqueue.h"
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

		inputComponent->isFiring = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}
};