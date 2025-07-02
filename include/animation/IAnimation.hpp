/*
** Paul Mourens project
** Duck Hunt cpp
** IAnimation.hpp
** Animation interface
*/

#pragma once
#include <SFML/Graphics.hpp>

namespace Animation {
    class IAnimation {
        public:
            virtual ~IAnimation() = default;
            virtual void update(float dt) = 0;
            virtual void draw(sf::RenderWindow& window) = 0;
            virtual void setPosition(sf::Vector2f pos) = 0;
            virtual sf::Vector2f getPosition() const = 0;
            virtual sf::Sprite getSprite() const = 0;
    };
}