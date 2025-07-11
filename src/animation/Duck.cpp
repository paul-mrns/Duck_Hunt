/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.cpp
*/

#include "animation/Duck.hpp"
#include "Core.hpp"

Animation::Duck::Duck(const sf::Texture& texture, int round)
    : _direction(1.f, -1.f), _state(DuckState::Flying)
{
    _speed = calculateSpeedFromRound(round);
    _color = rand() % 3;
    setTexture(texture);
    _sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
    initFrames();
    setSpeed(0.1f);
    randomiseValues();
    flipIfNeeded();
}

void Animation::Duck::randomiseValues()
{
    float scaledWidth = SPRITE_WIDTH * SPRITE_SCALE;
    float startX = static_cast<float>(rand() % static_cast<int>(WINDOW_WIDTH - scaledWidth));
    float startY = 850.f;
    setPosition({startX, startY});

    float angleDeg = static_cast<float>((rand() % 120 + 30));
    float angleRad = angleDeg * 3.14159f / 180.f;
    _direction = {std::cos(angleRad), -std::abs(std::sin(angleRad))};
}

float Animation::Duck::calculateSpeedFromRound(int round) const
{
    float baseSpeed = 400.f;
    float scaling = std::min(round * 14.f, 500.f);

    return baseSpeed + scaling;
}

void Animation::Duck::initFrames()
{
    _horizontalFlyFrames = getFlyFrames(_color, Horizontal);
    _diagonalFlyFrames = getFlyFrames(_color, Diagonal);
    _flyAwayFrames = getFlyFrames (_color, Up);
    _hitFrame = {260 * _color, 473, SPRITE_WIDTH, SPRITE_HEIGHT};
    _fallFrame = {80 + 260 * _color, 473, SPRITE_WIDTH, SPRITE_HEIGHT};
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
        Frame = {xStart, yStart, SPRITE_WIDTH, SPRITE_HEIGHT};
        xStart += 80;
        vector.emplace_back(Frame);
    }
    return vector;
}

void Animation::Duck::update(float dt)
{
    applyAnimationFrame(dt);

    switch (_state) {
        case DuckState::Flying: {
            _flightTime += dt;
            if (_flightTime >= 5.f)
                _canLeaveScreen = true;
            duckFlying(dt);
            break;
        }

        case DuckState::FlyAway: {
            _flyAwayElapsed += dt;
            float ascent = _flyAwaySpeed * dt;
            _sprite.move(0.f, -ascent);
            break;
        }

        case DuckState::Hit: {
            _direction = {0.f, 100.f};
            if (_hitClock.getElapsedTime().asSeconds() >= 0.15f)
                setFallFrames();
            break;
        }

        case DuckState::Falling: {
            _fallElapsed += dt;
            float distance = _fallSpeed * dt;
            _sprite.move(0.f, distance);
            int spin = static_cast<int>(_fallElapsed * 10) % 2;
            _sprite.setOrigin(34.f, 31.f);
            _sprite.setScale(spin == 0 ? 1.6f : -1.6f, 1.6f);
            if (_fallElapsed >= 1.5f)
                _state = DuckState::Caught;
            break;
        }

        case DuckState::Caught:
            break;
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
    _fallSpeed = ((850.f - startY) / 1.5f) + 100;
}

void Animation::Duck::Hit()
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
        _flyAwayElapsed = 0.f;
        _flyAwaySpeed = 1000.f;
    }
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

void Animation::Duck::flipIfNeeded()
{
    if (_direction.x < 0.f && !_isFlipped) {
        _sprite.setScale(-SPRITE_SCALE, SPRITE_SCALE);
        _isFlipped = true;
    } else if (_direction.x > 0.f && _isFlipped) {
        _sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
        _isFlipped = false;
    }
    _sprite.setOrigin(_sprite.getGlobalBounds().width / 2.f, _sprite.getGlobalBounds().height / 2.f);
}

void Animation::Duck::duckFlying(float dt)
{
    _sprite.move(_direction * _speed * dt);
    sf::FloatRect bounds = _sprite.getGlobalBounds();
    bool bounced = bounce(bounds);

    if (!_hasLeftGrass && bounds.top < 800.f)
        _hasLeftGrass = true;
    flipIfNeeded();
    if (bounced) {
        float absX = std::abs(_direction.x);
        float absY = std::abs(_direction.y);

        if (absX > absY)
            setFrames(_horizontalFlyFrames);
        else
            setFrames(_diagonalFlyFrames);
    }
}

bool Animation::Duck::bounce(sf::FloatRect bounds)
{
    bool bounced = false;
    float scaledWidth = SPRITE_WIDTH * SPRITE_SCALE;
    float scaledHeight = SPRITE_HEIGHT * SPRITE_SCALE;

    if (!_canLeaveScreen) {
        if (bounds.left <= 0.f) {
            _direction.x = std::abs(_direction.x);
            _sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
            _isFlipped = false;
            _sprite.setPosition(scaledWidth, _sprite.getPosition().y);
            bounced = true;
        } else if (bounds.left + bounds.width >= WINDOW_WIDTH) {
            _direction.x = -std::abs(_direction.x);
            _sprite.setScale(-SPRITE_SCALE, SPRITE_SCALE);
            _isFlipped = true;
            _sprite.setPosition(WINDOW_WIDTH - scaledWidth, bounds.top);
        }
        if (bounds.top <= 0.f) {
            _direction.y = std::abs(_direction.y);
            _sprite.setPosition(_sprite.getPosition().x, scaledHeight);
            bounced = true;
        }
    }
    if (_hasLeftGrass && bounds.top + bounds.height >= 850.f) {
        _direction.y = -std::abs(_direction.y);
        float newY = 900.f - bounds.height;
        _sprite.setPosition(_sprite.getPosition().x, newY);
        bounced = true;
    }
    return bounced;
}
