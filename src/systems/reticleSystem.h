#pragma once
#include "src/entity.h"

class ReticleSystem {
public:
    void update(sf::RenderWindow& window, Entity& reticle)
    {
        auto positionComponent = reticle.getComponent<PositionComponent>("Position");
        auto spriteComponent = reticle.getComponent<SpriteComponent>("Sprite");

        if (positionComponent && spriteComponent)
        {
            // Update the reticle's position to follow the mouse
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

            positionComponent->position = mouseWorldPos;
            spriteComponent->sprite.setPosition(mouseWorldPos);
        }
    }
};