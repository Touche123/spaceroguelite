#pragma once

#include "src/entity.h"

class HealthSystem {
public:
    void update(std::vector<Entity>& entities, std::vector<Entity>& enemies)
    {
        for (auto it = enemies.begin(); it != enemies.end();) {
            auto healthComponent = it->getComponent<HealthComponent>("Health");
            if (healthComponent && healthComponent->isDead())
            {
                // Remove the dead enemy from the game
                it = enemies.erase(it);
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
};