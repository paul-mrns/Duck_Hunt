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

            sf::Texture _backgroundText;
            sf::Sprite _backgroundSpr;

            void resizeSprite(sf::Sprite &sprite, const sf::Texture &texture, float width, float height)
            {
                sf::Vector2u textureSize = texture.getSize();
                sf::Vector2f scaleFactors;
                
                scaleFactors.x = width / static_cast<float>(textureSize.x);
                scaleFactors.y = height / static_cast<float>(textureSize.y);
                sprite.setScale(scaleFactors);
            }
    };
}