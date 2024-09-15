#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
private:
	float current_lifetime = 0.f;
	float max_lifetime = 10.f;
public:
	float acceleration;
	float max_speed;
	float deceleration;
	float speed;
	float damage = 1.f;
	bool is_alive = true;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::RectangleShape sprite;

	Projectile(sf::Vector2f position, sf::Vector2f direction);
	void update(float dt);
	void draw(sf::RenderWindow& window);
};