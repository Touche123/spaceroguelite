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
	sf::Vector2f movement_direction; // A vector to track input.
	bool isFiring = false;
};

struct BulletComponent {
	float lifeTime = 5.0f;
};

struct PlayerComponent {

};

struct ShootingComponent {
	float shoot_cooldown;
	float time_since_last_shot;

	ShootingComponent(float cooldown)
		: shoot_cooldown(cooldown), time_since_last_shot(0.0f) {}

	bool canShoot() const {
		return time_since_last_shot >= shoot_cooldown;
	}

	void resetCooldown() {
		time_since_last_shot = 0.0f;
	}

	void updateCooldown(float deltaTime) {
		if (time_since_last_shot < shoot_cooldown) {
			time_since_last_shot += deltaTime;
		}
	}
};

struct BehaviorComponent {
	std::shared_ptr<IMovementBehavior> behavior;

	BehaviorComponent(std::shared_ptr<IMovementBehavior> behavior)
		: behavior(behavior) { }
};

struct AudioComponent {
	std::map<std::string, sf::SoundBuffer> sound_buffers;
	std::map<std::string, sf::Sound> sounds;

	bool loadSound(const std::string& name, const std::string filename) {
		sf::SoundBuffer buffer;
		if (buffer.loadFromFile(filename)) {
			sound_buffers[name] = buffer;
			sounds[name] = sf::Sound();
			sounds[name].setBuffer(sound_buffers[name]);
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
	float damage_cooldown;
	float time_since_last_hit;
	std::function<void()> onDeath;

	HealthComponent(float initialHealth, float cooldown) 
		: health(initialHealth), damage_cooldown(cooldown), time_since_last_hit(0.0f) {}

	void takeDamage(float amount) {
		health -= amount;
		
		if (health <= 0) {
			health = 0;
			if (onDeath)
				onDeath();
		}
		time_since_last_hit = 0.0f;
	}

	bool canTakeDamage() const {
		return time_since_last_hit >= damage_cooldown;
	}

	bool isDead() const {
		return health <= 0;
	}

	void updateCooldown(float deltaTime) {
		if (time_since_last_hit < damage_cooldown)
			time_since_last_hit += deltaTime;
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