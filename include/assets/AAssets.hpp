/*
** Paul Mourens project
** Duck Hunt cpp
** Menu.cpp
** assets handling: Abstract Interface
*/

#pragma once
#include "IAssets.hpp"

namespace Assets
{
    class AAssets : public IAssets
    {
        public:
            AAssets() = default;
            ~AAssets() = default;
        protected:

            void resizeSprite(sf::Sprite &sprite, const sf::Texture &texture, float width, float height)
            {
                sf::Vector2u textureSize = texture.getSize();
                sf::Vector2f scaleFactors;
                
                scaleFactors.x = width / static_cast<float>(textureSize.x);
                scaleFactors.y = height / static_cast<float>(textureSize.y);
                sprite.setScale(scaleFactors);
            }

            void centerSpriteAxisX(sf::Sprite& sprite)
            {
                sf::FloatRect bounds = sprite.getLocalBounds();

                sprite.setOrigin(bounds.width / 2.f, sprite.getOrigin().y);
                sprite.setPosition(1920.f / 2.f, sprite.getPosition().y);
            }

            void centerSpriteAxisY(sf::Sprite& sprite)
            {
                sf::FloatRect bounds = sprite.getLocalBounds();

                sprite.setOrigin(sprite.getOrigin().x, bounds.height / 2.f);
                sprite.setPosition(sprite.getPosition().x, 1080.f / 2.f);
            }

            void centerSprite(sf::Sprite& sprite) {
                centerSpriteAxisX(sprite);
                centerSpriteAxisY(sprite);
            }

            void centerTextAxisX(sf::Text &text)
            {
                sf::FloatRect bounds = text.getLocalBounds();

                text.setOrigin(bounds.width / 2.f, text.getOrigin().y);
                text.setPosition(1920.f / 2.f, text.getPosition().y);
            }

            void centerTextAxisY(sf::Text& text)
            {
                sf::FloatRect bounds = text.getLocalBounds();

                text.setOrigin(text.getOrigin().x, bounds.height / 2.f);
                text.setPosition(text.getPosition().x, 1080.f / 2.f);
            }

            void centerText(sf::Text& text) {
                centerTextAxisX(text);
                centerTextAxisY(text);
            }


    };
}
