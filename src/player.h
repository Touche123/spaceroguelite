#pragma once
#include <SFML/Graphics.hpp>
#include "projectile.h"
#include <vector>

class Player {
private:
	sf::Vector2i mouse_local_position;
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	
	float acceleration;
	float max_speed;

	float fire_cooldown;
	float firerate;
	bool is_firing;

	float speed;

	sf::RectangleShape sprite;
	sf::CircleShape rectile;
	
	std::vector<Projectile> projectiles;

	Player();
	void update(sf::Window& window, float dt);
	void updateWeapons(float dt);
	void draw(sf::RenderWindow& window);
};