#pragma once

#include "src/entity.h"

class HealthSystem {
public:
    HealthSystem(EventQueue& eventQueue, std::vector<Entity>& entities)
        : eventQueue(eventQueue), entities(entities)
    {
    }
    void processEvents()
    {
        auto damageEntityEvents = eventQueue.getEventsByType(EventType::DamageEntity);

        for (auto& eventRef : damageEntityEvents)
        {
            Event& event = eventRef.get();
            if (event.status == EventStatus::Pending)
            {
                if (auto* damageEntityData = std::get_if<DamageEntityEventData>(&event.data))
                {
                    

                    int id = damageEntityData->entityId;
                    auto it = std::find_if(entities.begin(), entities.end(),
                        [id](const Entity& entity) {
                            return entity.id == id;
                        });

                    if (it != entities.end())
                    {
                        it->getComponent<HealthComponent>("Health")->takeDamage(damageEntityData->damageAmount);
                    }
                    // Create and add a new bullet based on the event data
                    //bulletFactory.createPlayerBullets(fireBulletData->position, fireBulletData->direction, 400.f);

                    event.status = EventStatus::Processed;
                }
            }
        }
    }

    void update()
    {
        processEvents();
        for (auto it = entities.begin(); it != entities.end();) {
            auto healthComponent = it->getComponent<HealthComponent>("Health");
            if (healthComponent && healthComponent->isDead())
            {
                // Remove the dead enemy from the game
                it = entities.erase(it);
            } else
            {
                ++it;
            }
        }

        for (auto it = entities.begin(); it != entities.end();) {
            auto healthComponent = it->getComponent<HealthComponent>("Health");
            if (healthComponent && healthComponent->isDead())
            {
                // Remove the dead enemy from the game
                it = entities.erase(it);
            } else
            {
                ++it;
            }
        }
    }
private:
    EventQueue& eventQueue;
    std::vector<Entity>& entities;
};