#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

class IMovementBehavior;

struct PositionComponent {
	sf::Vector2f position;
};

struct SpriteComponent {
	sf::Sprite sprite;
};

struct CollisionComponent {
	sf::FloatRect bounds;

	void updateBounds(const sf::Sprite& sprite) {
		bounds = sprite.getGlobalBounds();
	}
};

struct VelocityComponent {
	sf::Vector2f velocity; // movement speed and direction
};

struct InputComponent {
	sf::Vector2f movementDirection; // A vector to track input.
	bool isFiring = false;
};

struct BulletComponent {
	float lifeTime = 5.0f;
};

struct PlayerComponent {

};

struct ShootingComponent {
	float shootCooldown;
	float timeSinceLastShot;

	ShootingComponent(float cooldown)
		: shootCooldown(cooldown), timeSinceLastShot(0.0f) {}

	bool canShoot() const {
		return timeSinceLastShot >= shootCooldown;
	}

	void resetCooldown() {
		timeSinceLastShot = 0.0f;
	}

	void updateCooldown(float deltaTime) {
		if (timeSinceLastShot < shootCooldown) {
			timeSinceLastShot += deltaTime;
		}
	}
};

struct BehaviorComponent {
	std::shared_ptr<IMovementBehavior> behavior;

	BehaviorComponent(std::shared_ptr<IMovementBehavior> behavior)
		: behavior(behavior) { }
};

struct AudioComponent {
	std::map<std::string, sf::SoundBuffer> soundBuffers;
	std::map<std::string, sf::Sound> sounds;

	bool loadSound(const std::string& name, const std::string filename) {
		sf::SoundBuffer buffer;
		if (buffer.loadFromFile(filename)) {
			soundBuffers[name] = buffer;
			sounds[name] = sf::Sound();
			sounds[name].setBuffer(soundBuffers[name]);
			return true;
		}
		return false;
	}

	void play(const std::string& name) {
		if (sounds.find(name) != sounds.end()) {
			sounds[name].play();
		}
	}
};

struct HealthComponent {
	float health;
	float damageCooldown;
	float timeSinceLastHit;
	std::function<void()> onDeath;

	HealthComponent(float initialHealth, float cooldown) 
		: health(initialHealth), damageCooldown(cooldown), timeSinceLastHit(0.0f) {}

	void takeDamage(float amount) {
		health -= amount;
		
		if (health <= 0) {
			health = 0;
			if (onDeath)
				onDeath();
		}
		timeSinceLastHit = 0.0f;
	}

	bool canTakeDamage() const {
		return timeSinceLastHit >= damageCooldown;
	}

	bool isDead() const {
		return health <= 0;
	}

	void updateCooldown(float deltaTime) {
		if (timeSinceLastHit < damageCooldown)
			timeSinceLastHit += deltaTime;
	}
};

class Entity {
public:
	friend class EntitySystem;
	bool marked_for_removal = false;
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

	int getId() const { return id; }

private:
	int id;
};