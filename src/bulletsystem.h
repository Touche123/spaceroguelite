#pragma once

#include "src/entity.h"
#include "bulletfactory.h"
#include "eventqueue.h"

class BulletSystem {
public:
    BulletSystem(BulletFactory& bulletFactory, EventQueue& eventQueue)
        : bulletFactory(bulletFactory), eventQueue(eventQueue)
    {
    }

    void update(float deltaTime, std::vector<Entity>& bullets)
    {
        processEvents(bullets);  // Handle bullet creation events

        for (auto it = bullets.begin(); it != bullets.end();)
        {
            auto positionComponent = it->getComponent<PositionComponent>("Position");
            auto bulletComponent = it->getComponent<BulletComponent>("Bullet");

            if (positionComponent && bulletComponent)
            {
                // Update bullet position based on velocity
                positionComponent->position += bulletComponent->velocity * deltaTime;

                // Remove bullet if out of bounds
                if (isOutOfBounds(positionComponent->position))
                {
                    it = bullets.erase(it);
                } else
                {
                    ++it;
                }
            }
        }
    }

private:
    BulletFactory& bulletFactory;
    EventQueue& eventQueue;

    void processEvents(std::vector<Entity>& bullets)
    {
        for (const auto& event : eventQueue.getEvents())
        {
            if (event.type == EventType::FireBullet)
            {
                // Create and add a new bullet based on the event data
                Entity bullet = bulletFactory.createBullet(event.position, event.direction);
                bullets.push_back(bullet);
            }
        }
        eventQueue.clear();  // Clear events after processing
    }

    bool isOutOfBounds(const sf::Vector2f& position)
    {
        return position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600;
    }
};