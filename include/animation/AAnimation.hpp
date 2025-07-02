/*
** Paul Mourens project
** Duck Hunt cpp
** IAnimation.hpp
** Animation abstract class
*/

#pragma once
#include "IAnimation.hpp"

namespace Animation {
    class AAnimation : public IAnimation {
        public:
            AAnimation() = default;

            virtual void update(float dt) override;
            virtual void draw(sf::RenderWindow& window) override;
            virtual sf::Vector2f getPosition() const override;
            virtual sf::Sprite getSprite() const override;

            virtual void setPosition(sf::Vector2f pos) override;
            void setTexture(const sf::Texture& texture);
            void setFrames(const std::vector<sf::IntRect>& frames);
            void setSpeed(float speed);

        protected:
            sf::Sprite _sprite;
            std::vector<sf::IntRect> _frames;
            float _speed;
            float _elapsed = 0.f;
            std::size_t _currentFrame = 0;
        };
}