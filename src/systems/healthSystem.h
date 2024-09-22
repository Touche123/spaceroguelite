#pragma once

#include "src/entity.h"

class HealthSystem {
public:
    HealthSystem(EventQueue& eventQueue, EntitySystem& entitySystem)
        : eventQueue(eventQueue), entitySystem(entitySystem)
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
                    Entity* e = entitySystem.getEntityById(id);

                    if (e != nullptr)
                        e->getComponent<HealthComponent>("Health")->takeDamage(damageEntityData->damageAmount);
                    
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
        for (auto& entity : entitySystem.GetEntities()) {
            auto healthComponent = entity.getComponent<HealthComponent>("Health");
            if (healthComponent && healthComponent->isDead())
            {
                // Remove the dead enemy from the game
                entitySystem.markForRemoval(entity.getId());
            }
        }
    }
private:
    EventQueue& eventQueue;
    EntitySystem& entitySystem;
};