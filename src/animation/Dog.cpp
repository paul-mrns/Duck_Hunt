/*
** Paul Mourens project
** Duck Hunt cpp
** Dog.cpp
*/

#include "animation/Dog.hpp"
#include "Core.hpp"
#include <cmath>

Animation::Dog::Dog(const sf::Texture& texture)
{
    initFramesRects();
    setTexture(texture);
    setFrames(_sniffFrames);
    setSpeed(0.2f);
    setPosition({400.f, 800.f});
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::initFramesRects()
{
    _jumpFrames = {
        {000, 110, 120, 110},
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
    _laughingFrames = {
        {388, 125, 68, 80},
        {509, 125, 68, 80}
    };
    _happy1Frame = {638, 20, 112, 100};
    _happy2Frame = {638, 126, 112, 100};
    _sniffOrder = {0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 4, 3, 4};
}

void Animation::Dog::initHappy(sf::Vector2f pos, int ducksCaught)
{
    if (ducksCaught == 0)
        return;
    if (ducksCaught == 1)
        initHappy1(pos);
    else if (ducksCaught == 2)
        initHappy2(pos);
}

void Animation::Dog::initHappy1(sf::Vector2f pos)
{
    _state = DogState::Happy1;
    _catchPhase = CatchPhase::Rising;
    _catchTimer = 0.f;
    _elapsed = 0.f;
    _speed = 0.15f;
    _sprite.setTextureRect(_happy1Frame);
    _sprite.setPosition(pos);
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::initHappy2(sf::Vector2f pos)
{
    _state = DogState::Happy2;
    _catchPhase = CatchPhase::Rising;
    _catchTimer = 0.f;
    _elapsed = 0.f;
    _speed = 0.15f;
    _sprite.setTextureRect(_happy2Frame);
    _sprite.setPosition(pos);
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::initLaugh(sf::Vector2f pos)
{
    _state = DogState::Laughing;
    _laughPhase = LaughPhase::Rising;
    _laughTimer = 0.f;
    _elapsed = 0.f;
    _speed = 0.15f;
    setFrames(_laughingFrames);
    _sprite.setPosition(pos);
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::initGameoverLaugh(sf::Vector2f pos)
{
    _state = DogState::GoLaughing;
    _goLaughPhase = GoLaughPhase::Waiting;
    _laughTimer = 0.f;
    _elapsed = 0.f;
    _speed = 0.15f;
    setFrames(_laughingFrames);
    _sprite.setPosition(pos);
    _sprite.setScale(1.5f, 1.5f);
}

void Animation::Dog::happy(float dt)
{
    sf::Vector2f pos = _sprite.getPosition();

    _catchTimer += dt;
    if (_catchPhase == CatchPhase::Rising) {
        float progress = _catchTimer / 0.5f;
        pos.y = 775.f - (95.f * std::min(progress, 1.f));
        if (_catchTimer >= 0.5f) {
            pos.y = 680.f;
            _catchPhase = CatchPhase::Holding;
            _catchTimer = 0.f;
        }
        _sprite.setPosition(pos);
    }

    if (_catchPhase == CatchPhase::Holding) {
        if (_catchTimer >= 0.5f) {
            _catchPhase = CatchPhase::Falling;
            _catchTimer = 0.f;
        }
    }
    
    if (_catchPhase == CatchPhase::Falling) {
        float progress = _catchTimer / 0.5f;
        pos.y = 680.f + (95.f * std::min(progress, 1.f));
        if (_catchTimer >= 0.5f) {
            pos.y = 775.f;
            _state = DogState::Finished;
        }
        _sprite.setPosition(pos);
    }
}

void Animation::Dog::laugh(float dt)
{
    sf::Vector2f pos = _sprite.getPosition();

    _laughTimer += dt;
    int frameIndex = static_cast<int>(_laughTimer * 17) % 2;
    _sprite.setTextureRect(_laughingFrames[frameIndex]);
    if (_laughPhase == LaughPhase::Rising) {
        float progress = _laughTimer / 0.5f;
        pos.y = 800.f - (105.f * std::min(progress, 1.f));
        _sprite.setPosition(pos);
        if (_laughTimer >= 0.5f) {
            pos.y = 695.f;
            _sprite.setPosition(pos);
            _laughPhase = LaughPhase::Standing;
            _laughTimer = 0.f;
        }
    }

    if (_laughPhase == LaughPhase::Standing) {
        if (_laughTimer >= 1.f) {
            _laughPhase = LaughPhase::Falling;
            _laughTimer = 0.f;
        }
    }

    if (_laughPhase == LaughPhase::Falling) {
        float progress = _laughTimer / 0.5f;
        pos.y = 695.f + (125.f * std::min(progress, 1.f));
        _sprite.setPosition(pos);
        if (_laughTimer >= 1.0f) {
            pos.y = 810.f;
            _sprite.setPosition(pos);
            _state = DogState::Finished;
        }
    }
}

void Animation::Dog::gameoverLaugh(float dt)
{
    sf::Vector2f pos = _sprite.getPosition();

    _goLaughTimer += dt;
    int frameIndex = static_cast<int>(_goLaughTimer * 13) % 2;
    _sprite.setTextureRect(_laughingFrames[frameIndex]);
    
    if (_goLaughPhase == GoLaughPhase::Waiting) {
        if (_goLaughTimer >= 1.5f) {
            _goLaughPhase = GoLaughPhase::Rising;
            _goLaughTimer = 0.f;
        }
    }
    
    if (_goLaughPhase == GoLaughPhase::Rising) {
        float progress = _goLaughTimer / 0.5f;
        pos.y = 800.f - (105.f * std::min(progress, 1.f));
        _sprite.setPosition(pos);
        if (_goLaughTimer >= 0.5f) {
            pos.y = 695.f;
            _sprite.setPosition(pos);
            _goLaughPhase = GoLaughPhase::Standing;
            _goLaughTimer = 0.f;
        }
    }

    if (_goLaughPhase == GoLaughPhase::Standing)
        if (_goLaughTimer >= 3.25f) {
            _goLaughTimer = 0.f;
            _state = DogState::Finished;
        }
}

void Animation::Dog::jump(float dt)
{
    _jumpTimer += dt;
    float prepDuration = 0.4f;  
    float riseDuration = 0.7f;    
    float fallDuration = 0.7f;

    if (_jumpPhase == 0) {
        _sprite.setTextureRect(_jumpFrames[0]);
        if (_jumpTimer >= prepDuration) {
            _jumpPhase = 1;
            _jumpTimer = 0.f;
        }
    } else if (_jumpPhase == 1) {
        _sprite.setTextureRect(_jumpFrames[1]);
        float t = std::min(_jumpTimer / riseDuration, 1.f);
        float dx = 100.f * t;
        float dy = -280.f * std::sin(t * 1.5708f);
        _sprite.setPosition(_jumpStartPos.x + dx, _jumpStartPos.y + dy);
        if (_jumpTimer >= riseDuration) {
            _jumpPhase = 2;
            _jumpTimer = 0.f;
            _jumpStartPos = _sprite.getPosition();
        }
    } else if (_jumpPhase == 2) {
        _sprite.setTextureRect(_jumpFrames[2]);
        float t = std::min(_jumpTimer / fallDuration, 1.f);
        float dx = 100.f * t;
        float dy = 280.f * std::pow(t, 2);
        _sprite.setPosition(_jumpStartPos.x + dx, _jumpStartPos.y + dy);
        if (_jumpTimer >= fallDuration)
            _state = DogState::Finished;
    }
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
            _sprite.setTextureRect(_jumpFrames[0]);
            _jumpStartPos = _sprite.getPosition();
            return;
        }
    } else if (_state == DogState::Jump)
        jump(dt);
    else if (_state == DogState::Happy1 || _state == DogState::Happy2)
        happy(dt);
    else if (_state == DogState::Laughing)
        laugh(dt);
    else if (_state == DogState::GoLaughing)
        gameoverLaugh(dt);
}

