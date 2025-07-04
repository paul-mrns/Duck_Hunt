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
    _speed = 160.f;
    setTexture(texture);
    _color = rand() % 3;
    initFrames();
    setSpeed(0.15f);
    setPosition({static_cast<float>(rand() % 1920), 800});
    _sprite.setScale(1.4f, 1.4f);
    flipIfNeeded();
}

void Animation::Duck::initFrames()
{
    _horizontalFlyFrames = getFlyFrames(_color, Horizontal);
    _diagonalFlyFrames = getFlyFrames(_color, Diagonal);
    _flyAwayFrames = getFlyFrames (_color, Up);
    _hitFrame = {260 * _color, 472, 67, 63};
    _fallFrame = {94 + 260 * _color, 472, 39, 63};
    setFrames(_diagonalFlyFrames);
}

std::vector<sf::IntRect> Animation::Duck::getFlyFrames(int color, DuckFlight orientation)
{
    std::vector<sf::IntRect> vector;
    sf::IntRect Frame;
    int xStart = color * 260;
    int yStart;

    if (orientation == Horizontal)
        yStart = 235;
    else if (orientation == Diagonal)
        yStart = 310;
    else
        yStart = 390;
    for (int i = 0; i < 3; i++) {
        Frame = {xStart, yStart, 68, 68};
        xStart += 80;
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
            _sprite.move(0.f, -_speed * 4.f * dt);
            break;

        case DuckState::Hit:
            _direction = {0.f, 100.f};
            if (_hitClock.getElapsedTime().asSeconds() >= 0.15f)
                setFallFrames();
            break;

        case DuckState::Falling: {
            _fallElapsed += dt;
            float distance = _fallSpeed * dt;
            _sprite.move(0.f, distance);
            int spin = static_cast<int>(_fallElapsed * 10) % 2;
            _sprite.setScale(spin == 0 ? 1.4f : -1.4f, 1.4f);
            if (_fallElapsed >= 1.5f) {
                _sprite.setPosition(_sprite.getPosition().x, 800.f);
                _state = DuckState::Caught;
            }
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

void Animation::Duck::setFallFrames()
{
    _sprite.setTextureRect(_fallFrame);
    _state = DuckState::Falling;
    setSpeed(0.f);
    float startY = _sprite.getPosition().y;
    _fallElapsed = 0.f;
    _fallSpeed = ((800.f - startY) / 1.5f) + 100;
}

void Animation::Duck::isHit()
{
    if (_state == DuckState::Flying) {
        _sprite.setTextureRect(_hitFrame);
        _state = DuckState::Hit;
        _hitClock.restart();
    }
}

void Animation::Duck::flyAway()
{
    if (_state == DuckState::Flying) {
        setFrames(_flyAwayFrames);
        setSpeed(0.10f);
        _state = DuckState::FlyAway;
        _direction = {0.f, -1.f};
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

int Animation::Duck::getDuckScore(int round) const
{
    int baseScore = 0;
    int roundBonus = 0;

    switch (_color) {
        case BLACK_DUCK: baseScore = 500; break;
        case BLUE_DUCK:  baseScore = 1000; break;
        case RED_DUCK:   baseScore = 1500; break;
        default:         baseScore = 500; break;
    }
    if (round >= 6 && round <= 10)
        roundBonus = baseScore * 0.6f;
    else if (round >= 11)
        roundBonus = baseScore;
    return baseScore + roundBonus;
}

bool Animation::Duck::isOffScreen() const
{
    const sf::FloatRect bounds = _sprite.getGlobalBounds();

    return ( 
        bounds.top + bounds.height < 0 || bounds.left + bounds.width < 0 ||
        bounds.top > WINDOW_HEIGHT || bounds.left > WINDOW_WIDTH
    );
}


bool Animation::Duck::isFalling() const
{
    return _state == DuckState::Falling;
}

void Animation::Duck::flipIfNeeded()
{
    if (_direction.x < 0.f && !_isFlipped) {
        _sprite.setScale(-1.4f, 1.4f);
        _isFlipped = true;
    } else if (_direction.x > 0.f && _isFlipped) {
        _sprite.setScale(1.4f, 1.4f);
        _isFlipped = false;
    }
}
