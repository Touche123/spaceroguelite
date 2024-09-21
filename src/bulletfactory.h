#pragma once

#include <SFML/Graphics.hpp>
#include "src/entity.h"

class BulletFactory {
public:
    std::vector<Entity> bullets;

    BulletFactory(sf::Texture& bulletTexture)
        : bulletTexture(bulletTexture)
    {
    }

    void createBullet(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
    {
        Entity bullet;

        // Create components for the bullet
        auto bulletPosition = std::make_shared<PositionComponent>();
        bulletPosition->position = position;  // Spawn bullet at player's position

        auto bulletVelocity = std::make_shared<VelocityComponent>();
        bulletVelocity->velocity = direction * speed;  // Set bullet velocity

        auto bulletComponent = std::make_shared<BulletComponent>();

        auto spriteComponent = std::make_shared<SpriteComponent>();
        spriteComponent->sprite.setTexture(bulletTexture);  // Set bullet texture

        auto collisionComponent = std::make_shared<CollisionComponent>();
        collisionComponent->bounds = spriteComponent->sprite.getGlobalBounds();

        // Add components to the bullet entity
        bullet.addComponent("Position", bulletPosition);
        bullet.addComponent("Velocity", bulletVelocity);
        bullet.addComponent("Bullet", bulletComponent);
        bullet.addComponent("Sprite", spriteComponent);
        bullet.addComponent("Collision", collisionComponent);

        bullets.push_back(bullet);
    }

private:
    sf::Texture& bulletTexture;
    float bulletSpeed;
};