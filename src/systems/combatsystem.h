#pragma once
#include "src/eventqueue.h"

class CombatSystem {
public:
    CombatSystem(EventQueue& eventQueue)
        : eventQueue(eventQueue)
    {
    }

    void fireBullet(const sf::RenderWindow& window, const sf::Vector2f& playerPosition)
    {
        // Calculate direction towards the mouse
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

        sf::Vector2f direction = mouseWorldPos - playerPosition;
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude != 0)
        {
            direction /= magnitude;
        }

        // Post a fire bullet event
        Event bulletEvent = { EventType::FireBullet, playerPosition, direction };
        eventQueue.addEvent(bulletEvent);
    }

private:
    EventQueue& eventQueue;
};