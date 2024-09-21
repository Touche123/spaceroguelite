#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

struct PositionComponent {
	sf::Vector2f position;
};

struct SpriteComponent {
	sf::Sprite sprite;
};

struct CollisionComponent {
	sf::FloatRect bounds;
};

struct VelocityComponent {
	sf::Vector2f velocity; // movement speed and direction
};

struct InputComponent {
	sf::Vector2f movementDirection; // A vector to track input.
	bool isFiring = false;
};

struct BulletComponent {
	sf::Vector2f velocity;
	float lifeTime = 5.0f;
};

class Entity {
public:
	int id;
	std::unordered_map<std::string, std::shared_ptr<void>> components;

	template<typename T>
	void addComponent(const std::string& name, std::shared_ptr<T> component) {
		components[name] = component;
	}

	template<typename T>
	std::shared_ptr<T> getComponent(const std::string& name) const {
		auto it = components.find(name);
		if (it != components.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}
		return nullptr;  // Component not found
	}
};