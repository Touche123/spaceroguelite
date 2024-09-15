#include "projectile.h"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction)
{
	sprite = sf::RectangleShape(sf::Vector2f(20, 20));
	sprite.setFillColor(sf::Color::Blue);
	max_speed = 400.f;
	acceleration = 400.0f;
	deceleration = 100.f;
	this->position = position;
	this->direction = direction;
}

void Projectile::update(float dt)
{
	current_lifetime += dt;
	is_alive = !(current_lifetime >= max_lifetime);
	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude != 0.f)
		direction /= magnitude;

	if (magnitude != 0.f) {
		velocity += direction * acceleration;

		float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		if (speed > max_speed)
			velocity = (velocity / speed) * max_speed;
	}

	position += velocity * dt;
	
	sprite.setPosition(position);
}