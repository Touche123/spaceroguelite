#include "player.h"

void Player::update(sf::Window& window, float dt)
{
	sf::Vector2f direction(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
		direction.y -= 1.f;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
		direction.y += 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		direction.x -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		direction.x += 1.f;
	}
	
	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude != 0.f) {
		direction /= magnitude;
	}

	if (magnitude != 0.f)
	{
		velocity += direction * acceleration;

		float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		if (speed > max_speed)
			velocity = (velocity / speed) * max_speed;
	} else
	{
		velocity *= 0.999f;
	}

	position += velocity * dt;

	sprite.setPosition(position);
	mouse_local_position = sf::Mouse::getPosition(window);
	rectile.setPosition((float)mouse_local_position.x, (float)mouse_local_position.y);

	updateWeapons(dt);
}

void Player::updateWeapons(float dt)
{
	is_firing = fire_cooldown > 0.f;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (!is_firing) {
			sf::Vector2f direction(mouse_local_position.x - position.x, mouse_local_position.y - position.y);
			float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (magnitude != 0)
				direction /= magnitude;
			
			Projectiles.push_back(Projectile(position, direction));
			fire_cooldown = firerate;
		}
	}
	if (is_firing) {
		fire_cooldown -= dt;
		if (fire_cooldown < 0.f)
			fire_cooldown = 0.f;
	}

	for (auto &projectile : Projectiles) {
		projectile.update(dt);
	}

	std::erase_if(Projectiles, [](Projectile const& p) { return !p.is_alive; });
}

Player::Player()
{
	sprite = sf::RectangleShape(sf::Vector2f(50, 50));
	max_speed = 200.f;
	acceleration = 0.4f;

	rectile = sf::CircleShape(20.f);
	rectile.setFillColor(sf::Color::Green);
	rectile.setOrigin(20.f, 20.f);

	firerate = 0.2f;
}

