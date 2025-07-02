/*
** Paul Mourens project
** Duck Hunt cpp
** AAnimation.cpp
** abstract class functions declarations
*/

#include "animation/AAnimation.hpp"

void Animation::AAnimation::setTexture(const sf::Texture& texture)
{
    _sprite.setTexture(texture);
}

void Animation::AAnimation::setFrames(const std::vector<sf::IntRect>& frames)
{
    _frames = frames;
    _currentFrame = 0;
    _elapsed = 0.f;
    if (!_frames.empty())
        _sprite.setTextureRect(_frames[0]);
}

void Animation::AAnimation::setSpeed(float speed)
{
    _speed = speed;
}

void Animation::AAnimation::update(float dt)
{
    _elapsed += dt;
    if (_elapsed >= _speed && !_frames.empty()) {
        _elapsed = 0.f;
        _currentFrame = (_currentFrame + 1) % _frames.size();
        _sprite.setTextureRect(_frames[_currentFrame]);
    }
}

void Animation::AAnimation::draw(sf::RenderWindow& window)
{
    window.draw(_sprite);
}

void Animation::AAnimation::setPosition(sf::Vector2f pos)
{
    _sprite.setPosition(pos);
}

sf::Vector2f Animation::AAnimation::getPosition() const
{
    return _sprite.getPosition();
}

sf::Sprite Animation::AAnimation::getSprite() const
{
    return _sprite;
}

