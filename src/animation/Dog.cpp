/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.cpp
*/

#include "animation/Dog.hpp"
#include "Core.hpp"

void Animation::Dog::sniff(float dt)
{
    update(dt);
}

void Animation::Dog::initFramesRects()
{
    _jumpFrames = {
        {0, 110, 120, 110},
        {120, 110, 120, 110},
        {240, 110, 120, 110},
    };
    _sniffFrames = {
        {000, 000, 120, 110},
        {120, 000, 120, 110},
        {240, 000, 120, 110},
        {360, 000, 120, 110},
        {480, 000, 120, 110}
    };
    _sniffOrder = {0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 4, 3, 4};
    _jumpOrder  = {0, 1, 2};
}


Animation::Dog::Dog(const sf::Texture& texture)
{
    initFramesRects();
    setTexture(texture);
    setFrames(_sniffFrames);
    setSpeed(0.2f);
    setPosition({400.0f, 800.f});
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::update(float dt)
{
    _elapsed += dt;
    _totalElapsed += dt;

    if (_state == DogState::Sniffing) {
        if (_sniffOrder[_frameIndex] < 3)
            _sprite.move(60.f * dt, 0.f);
        if (_elapsed >= _speed) {
            _elapsed = 0.f;
            _sprite.setTextureRect(_sniffFrames[_sniffOrder[_frameIndex]]);
            _frameIndex = (_frameIndex + 1) % _sniffOrder.size();
        }

        if (_totalElapsed >= _jumpDelay) {
            _state = DogState::Jump;
            _frameIndex = 0;
            _elapsed = 0.f;
            _sprite.setTextureRect(_jumpFrames[_jumpOrder[0]]);
            _jumpStartPos = _sprite.getPosition();
            return;
        }
    } else if (_state == DogState::Jump) {
        _elapsed += dt;
        _jumpTime += dt;

        if (_frameIndex < _jumpFrameDurations.size() && _elapsed >= _jumpFrameDurations[_frameIndex]) {
            _elapsed = 0.f;
            _frameIndex++;

            if (_frameIndex >= _jumpOrder.size()) {
                _state = DogState::Finished;
                return;
            }

            _sprite.setTextureRect(_jumpFrames[_jumpOrder[_frameIndex]]);
        }
        float yOffset = _jumpHeights[_frameIndex];
        sf::Vector2f base = _jumpStartPos;
        _sprite.setPosition(base.x, base.y + yOffset);
    }
}

