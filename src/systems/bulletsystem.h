#pragma once

#include "src/entity.h"
#include "../bulletfactory.h"
#include "../eventqueue.h"

class BulletSystem {
public:
    BulletSystem(BulletFactory& bulletFactory, EventQueue& eventQueue)
        : bulletFactory(bulletFactory), eventQueue(eventQueue)
    {
    }

    void update(float deltaTime)
    {
        processEvents();  // Handle bullet creation events

        for (auto it = bulletFactory.playerBullets.begin(); it != bulletFactory.playerBullets.end();)
        {
            auto positionComponent = it->getComponent<PositionComponent>("Position");
            auto velocityComponent = it->getComponent<VelocityComponent>("Velocity");

            if (positionComponent && velocityComponent)
            {
                // Update bullet position based on velocity
                positionComponent->position += velocityComponent->velocity * deltaTime;

                // Remove bullet if out of bounds
                if (isOutOfBounds(positionComponent->position))
                {
                    it = bulletFactory.playerBullets.erase(it);
                } else
                {
                    ++it;
                }
            }
        }

        for (auto it = bulletFactory.enemyBullets.begin(); it != bulletFactory.enemyBullets.end();)
        {
            auto positionComponent = it->getComponent<PositionComponent>("Position");
            auto velocityComponent = it->getComponent<VelocityComponent>("Velocity");

            if (positionComponent && velocityComponent)
            {
                // Update bullet position based on velocity
                positionComponent->position += velocityComponent->velocity * deltaTime;

                // Remove bullet if out of bounds
                if (isOutOfBounds(positionComponent->position))
                {
                    it = bulletFactory.enemyBullets.erase(it);
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

    void processEvents()
    {
        auto fireBulletEvents = eventQueue.getEventsByType(EventType::FireBullet);

        for (auto& eventRef : fireBulletEvents) {
            Event& event = eventRef.get();
            if (event.status == EventStatus::Pending) {
                if (auto* fireBulletData = std::get_if<FireBulletEventData>(&event.data)) {
                    // Create and add a new bullet based on the event data
                    bulletFactory.createPlayerBullets(fireBulletData->position, fireBulletData->direction, 400.f);
                    event.status = EventStatus::Processed;
                }
            }
        }
    }

    bool isOutOfBounds(const sf::Vector2f& position)
    {
        return position.x < 0 || position.x > 1600 || position.y < 0 || position.y > 900;
    }
};