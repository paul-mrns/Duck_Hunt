/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.cpp
*/

#include "animation/Duck.hpp"
#include "Core.hpp"

Animation::Duck::Duck(const sf::Texture& texture)
    : _direction(1.f, -1.f),
      _state(DuckState::Flying)
{
    _speed = 160.0f;
    setTexture(texture);
    _color = rand() % 3;
    initFrames();
    setSpeed(0.12f);
    setPosition({1400, 900});
    flipIfNeeded();
}

void Animation::Duck::initFrames()
{
    _horizontalFlyFrames = getFlyFrames(_color, Horizontal);
    _diagonalFlyFrames = getFlyFrames(_color, Diagonal);
    _diagonalFlyFrames = getFlyFrames (_color, Up);
    _hitFrame = {_color * (SPRITE_WIDTH * 3 + 40), 455, SPRITE_WIDTH, SPRITE_HEIGTH};
    _fallFrame = {(_color * (SPRITE_WIDTH * 3 + 40)) + SPRITE_WIDTH, 455, SPRITE_WIDTH, SPRITE_HEIGTH};
    setFrames(_diagonalFlyFrames);
}

std::vector<sf::IntRect> Animation::Duck::getFlyFrames(int color, DuckFlight orientation)
{
    std::vector<sf::IntRect> vector;
    sf::IntRect Frame;
    int xStart = color * (SPRITE_WIDTH * 3 + 40);
    int yStart;

    if (orientation == Horizontal)
        yStart = 230;
    else if (orientation == Diagonal)
        yStart = 305;
    else
        yStart = 380;
    for (int i = 0; i < 3; i++) {
        Frame = {xStart, yStart, SPRITE_WIDTH, SPRITE_HEIGTH};
        xStart += SPRITE_WIDTH;
        vector.emplace_back(Frame);
    }
    return vector;
}

void Animation::Duck::update(float dt)
{
    applyAnimationFrame(dt);
    auto bounds = _sprite.getGlobalBounds();

    switch (_state) {
        case DuckState::Flying:
            _sprite.move(_direction * _speed * dt);
            if (bounds.left <= 0 || bounds.left + bounds.width >= WINDOW_WIDTH)
                _direction.x *= -1.f;
            if (bounds.top <= 0 || bounds.top + bounds.height >= WINDOW_HEIGHT)
                _direction.y *= -1.f;
            flipIfNeeded();
            break;

        case DuckState::FlyAway:
            _sprite.move(0.f, -_speed * 1.5f * dt);
            break;

        case DuckState::Hit:
            _direction = {0.f, 100.f};
            if (_hitClock.getElapsedTime().asSeconds() >= 0.15f)
                setFallFrames();
            break;

        case DuckState::Falling: {
            _sprite.move(0.f, _speed * 2.5f * dt);
            if (_sprite.getPosition().y >= 800.f)
                _state = Caught;
            break;
        }

        case DuckState::Caught: {
            sf::Vector2f pos = _sprite.getPosition();
            pos.y = 800.f;
            _sprite.setPosition(pos);
            _speed = 0.f;
        }
    }
}

void Animation::Duck::applyAnimationFrame(float dt)
{
    if (_state == DuckState::Flying || _state == DuckState::FlyAway)
        AAnimation::update(dt);
}

void Animation::Duck::setDirection(sf::Vector2f dir)
{
    _direction = dir;
    flipIfNeeded();
}

void Animation::Duck::setFlyAwayFrames()
{
    setFrames(_flyAwayFrames);
    setSpeed(0.15f);
    _state = DuckState::FlyAway;
    _direction = {0.f, -1.f};
}

void Animation::Duck::setFallFrames()
{
    _sprite.setTextureRect(_fallFrame);
    _state = DuckState::Falling;
    setSpeed(0.f);
}

void Animation::Duck::isHit()
{
    if (_state == DuckState::Flying) {
        _sprite.setTextureRect(_hitFrame);
        _state = DuckState::Hit;
        _hitClock.restart();
    }
}

bool Animation::Duck::isCaught() const
{
    return _state == Caught;
}

DuckState Animation::Duck::getState() const
{
    return _state;
}

bool Animation::Duck::isOffScreen(const sf::RenderWindow& window) const
{
    auto b = _sprite.getGlobalBounds();
    auto s = window.getSize();
    return b.top > s.y || b.left + b.width < 0 || b.left > s.x;
}

bool Animation::Duck::isFalling() const
{
    return _state == DuckState::Falling;
}

void Animation::Duck::flipIfNeeded()
{
    if (_direction.x < 0.f && !_isFlipped) {
        _sprite.setScale(-1.f, 1.f);
        _isFlipped = true;
    } else if (_direction.x > 0.f && _isFlipped) {
        _sprite.setScale(1.f, 1.f);
        _isFlipped = false;
    }
}
