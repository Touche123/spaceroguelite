#pragma once

#include <SFML/Graphics.hpp>
#include "src/entity.h"

class BulletFactory {
public:
    BulletFactory(sf::Texture& bulletTexture, float bulletSpeed)
        : bulletTexture(bulletTexture), bulletSpeed(bulletSpeed)
    {
    }

    Entity createBullet(const sf::Vector2f& position, const sf::Vector2f& direction)
    {
        Entity bullet;

        // Create components for the bullet
        auto bulletPosition = std::make_shared<PositionComponent>();
        bulletPosition->position = position;  // Spawn bullet at player's position

        auto bulletVelocity = std::make_shared<BulletComponent>();
        bulletVelocity->velocity = direction * bulletSpeed;  // Set bullet velocity

        auto spriteComponent = std::make_shared<SpriteComponent>();
        spriteComponent->sprite.setTexture(bulletTexture);  // Set bullet texture

        // Add components to the bullet entity
        bullet.addComponent("Position", bulletPosition);
        bullet.addComponent("Bullet", bulletVelocity);
        bullet.addComponent("Sprite", spriteComponent);

        return bullet;
    }

private:
    sf::Texture& bulletTexture;
    float bulletSpeed;
};