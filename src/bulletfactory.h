#pragma once

#include <SFML/Graphics.hpp>
#include "entity.h"

class BulletFactory {
public:
    std::vector<Entity> enemy_bullets;
    std::vector<Entity> player_bullets;

    BulletFactory(sf::Texture& bullet_texture)
        : bullet_texture(bullet_texture)
    {
    }

    void createEnemyBullet(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
    {
        Entity bullet;

        // Create components for the bullet
        auto bullet_position = std::make_shared<PositionComponent>();
        bullet_position->position = position;  // Spawn bullet at player's position

        auto bullet_velocity = std::make_shared<VelocityComponent>();
        bullet_velocity->velocity = direction * speed;  // Set bullet velocity

        auto bullet_component = std::make_shared<BulletComponent>();

        auto sprite_component = std::make_shared<SpriteComponent>();
        sprite_component->sprite.setTexture(bullet_texture);  // Set bullet texture

        auto collision_component = std::make_shared<CollisionComponent>();
        collision_component->bounds = sprite_component->sprite.getGlobalBounds();

        // Add components to the bullet entity
        bullet.addComponent("Position", bullet_position);
        bullet.addComponent("Velocity", bullet_velocity);
        bullet.addComponent("Bullet", bullet_component);
        bullet.addComponent("Sprite", sprite_component);
        bullet.addComponent("Collision", collision_component);

        enemy_bullets.push_back(bullet);
    }

    void createPlayerBullets(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
    {
        Entity bullet;

        // Create components for the bullet
        auto bullet_position = std::make_shared<PositionComponent>();
        bullet_position->position = position;  // Spawn bullet at player's position

        auto bullet_velocity = std::make_shared<VelocityComponent>();
        bullet_velocity->velocity = direction * speed;  // Set bullet velocity

        auto bullet_component = std::make_shared<BulletComponent>();

        auto sprite_component = std::make_shared<SpriteComponent>();
        sprite_component->sprite.setTexture(bullet_texture);  // Set bullet texture

        auto collision_component = std::make_shared<CollisionComponent>();
        collision_component->bounds = sprite_component->sprite.getGlobalBounds();

        // Add components to the bullet entity
        bullet.addComponent("Position", bullet_position);
        bullet.addComponent("Velocity", bullet_velocity);
        bullet.addComponent("Bullet", bullet_component);
        bullet.addComponent("Sprite", sprite_component);
        bullet.addComponent("Collision", collision_component);

        player_bullets.push_back(bullet);
    }

private:
    sf::Texture& bullet_texture;
    float bulletSpeed;
};