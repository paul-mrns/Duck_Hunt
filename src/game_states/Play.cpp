/*
** Paul Mourens project
** Duck Hunt cpp
** Play.cpp
*/

#include "Core.hpp"

DuckHunt::Play::Play(int &score, Audio& audio)
    : _audio(audio), _score(score)
{
    _assets = std::make_unique<Assets::PlayAssets>(std::vector<int>{1, 3, 0, 0});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
    _blackScreen.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    _blackScreen.setFillColor(sf::Color::Black);
    _hitboxRect.setFillColor(sf::Color::White);
    _duckHits.push_back(Blinking);
    for (int i = 0; i < 9; i++)
        _duckHits.push_back(Missed);

}

void DuckHunt::Play::drawIntro(sf::RenderWindow& window)
{
    window.draw(_assets->_roundBg);
    window.draw(_assets->_roundText);
    if (_dog) {
        if (_dog->isFalling()) {
            _dog->draw(window);
            window.draw(_assets->_grassSpr);
        } else {
            window.draw(_assets->_grassSpr);
            _dog->draw(window);
        }
    }
}

void DuckHunt::Play::drawHUD(sf::RenderWindow& window)
{
    window.draw(_assets->_greenRoundBg);
    window.draw(_assets->_greenRoundText);
    window.draw(_assets->_ammoBg);
    window.draw(_assets->_ammoText);
    window.draw(_assets->_hitBg);
    window.draw(_assets->_hitText);
    window.draw(_assets->_scoreBg);
    window.draw(_assets->_scoreText);
    drawAmmo(window, _assets->_bullets);
    drawHits(window, _assets->_redDucks, _assets->_whiteDucks);
    if (_flyAway)
        window.draw(_assets->_flyAwaySpr);
}

void DuckHunt::Play::draw(sf::RenderWindow& window)
{
    window.draw(_assets->_backgroundSpr);
    if (_duckActive && _duck)
        _duck->draw(window);
    if (_showFlash) {
        window.draw(_blackScreen);
        window.draw(_hitboxRect);
        return;
    }
    if (_dog && _dogCatching)
        _dog->draw(window);
    if (_dog && _dogLaughing)
        _dog->draw(window);
    if (!_gameHasStarted)
        drawIntro(window);
    else
        window.draw(_assets->_grassSpr);
    drawHUD(window);
    if (_pause)
        window.draw(_assets->_pauseSpr);
}

void DuckHunt::Play::drawAmmo(sf::RenderWindow &window, std::vector<sf::Sprite> bullets)
{
    for (int i = 0; i < _ammo; i++)
        window.draw(bullets[i]);
}

void DuckHunt::Play::drawHits(sf::RenderWindow &window, std::vector<sf::Sprite> redDucks,
    std::vector<sf::Sprite> whiteDucks)
{
    for (int i = 0; i < 10; i++) {
        if (_duckHits[i] == Missed)
            window.draw(whiteDucks[i]);
        if (_duckHits[i] == Hit)
            window.draw(redDucks[i]);
        if (_duckHits[i] == Blinking)
            if (_currentDuckBlinking)
                window.draw(whiteDucks[i]);
    }
}

void DuckHunt::Play::reload()
{
    _ammo = 3;
    _assets->setAmmo(_ammo);
}

void DuckHunt::Play::handleHit(sf::Vector2i mousePos)
{
    _showFlash = true;
    _lastMouseShot = sf::Vector2f(mousePos);
    _duckBounds = _duck->getSprite().getGlobalBounds();
    _hitboxRect.setPosition(_duckBounds.left, _duckBounds.top);
    _hitboxRect.setSize({_duckBounds.width, _duckBounds.height});
    _flashClock.restart();
}

void DuckHunt::Play::shoot(sf::Vector2i mousePos)
{
    _audio.playSound(SHOT_SOUND);
    _ammo--;
    _assets->setAmmo(_ammo);
    handleHit(mousePos);
    if (isDuckHit()) {
        _score += _duck->getDuckScore(_round);
        //display duck->getDuckScore(_round) on top of the duck;
        _assets->setScore(_score);
        _duckHits[_duckIndex] = Hit;
    } else {
        if (_ammo == 0 && _duck) {
            _flyAway = true;
            _duck->flyAway();
        }
    }
} 

void DuckHunt::Play::handleInput(input &in, sf::Vector2i mousePos)
{
    if (in == left_click && !_dogHasAppeared && !_pause) {
        if (_duck && !_duck->isFalling())
            shoot(mousePos);
        return;
    } else if (in == quit || in == escape) {
        if (_pause) {
            _pause = false;
            _audio.resumeCurrentMusic();
        } else {
            _pause = true;
            _audio.pauseCurrentMusic();
            _audio.playSound(PAUSE_SOUND);
        }
    } else
        return;
}

void DuckHunt::Play::spawnDuck()
{
    if (!_duckActive) {
        _duck = std::make_unique<Animation::Duck>(_assets->_spritesheet);
        _duck->setDirection({ (rand() % 2 ? 1.f : -1.f), -1.f });
        _duckActive = true;
    }
}

void DuckHunt::Play::duckHasBeenHit()
{
    _audio.stopMusic(FLAP_MUSIC);
    _duck->isHit();
    _audio.playMusic(FALL_MUSIC, false);
}

bool DuckHunt::Play::isDuckHit()
{
    if (_duckBounds.contains(_lastMouseShot))
        return true;
    return false;
}

void DuckHunt::Play::dogCatchingDuck(float dt)
{
    if (!_dogHasAppeared) {
        sf::Vector2f duckPos = _duck->getSprite().getPosition();
        _dog->initHappy1({ duckPos.x - 125.f, 775.f });
        _audio.stopMusic(FALL_MUSIC);
        _audio.playMusic(CAUGHT_MUSIC, false);
        _dogCatching = true;
        _dogHasAppeared = true;
    }
    _dog->update(dt);
}

void DuckHunt::Play::dogLaughing()
{
    _dog->initLaugh({WINDOW_WIDTH / 2.f, 775.f});
    _audio.playMusic(LAUGH_MUSIC, false);
    _dogLaughing = true;
    _dogHasAppeared = true;
    _flyAway = false;
}

void DuckHunt::Play::duckHasBeenCaught()
{
    _duck.reset();
    reload();
    _duckIndex++;
    _duckHits[_duckIndex] = Blinking;
    _duckActive = false;
    _dogCatching = false;
    _dogHasAppeared = false;
}

void DuckHunt::Play::playLoop(float dt)
{
    if (!_duck) {
        spawnDuck();
        _audio.playMusic(FLAP_MUSIC, true);
        return;
    }

    if (_duckActive && _duck) {
        _duck->update(dt);
        _currentDuckBlinkTimer += dt;
        if (_currentDuckBlinkTimer >= 0.5f) {
            _currentDuckBlinking = !_currentDuckBlinking;
            _currentDuckBlinkTimer = 0.f;
        }
    }
    
    if (_showFlash) {
        if (_flashClock.getElapsedTime().asSeconds() >= 0.05f) {
            _showFlash = false;
            if (isDuckHit())
                duckHasBeenHit();
        }
    }

    if (_duck && _duck->isCaught())
        dogCatchingDuck(dt);

    if (_dog && _dogCatching && _dog->isAnimationDone())
        duckHasBeenCaught();

    if (_duck && _flyAway)
        _duck->flyAway();

    if (_duck && _duck->isOffScreen() && _flyAway)
        dogLaughing();

    if (_dog && _dogLaughing) {
        _dog->update(dt);
        if (_dog->isAnimationDone()) {
            _dogHasAppeared = false;
            _dogLaughing = false;
            _duck.reset();
            _duckActive = false;
            _duckHits[_duckIndex] = Missed;
            _duckIndex++;
            _duckHits[_duckIndex] = Blinking;
            reload();
        }
    }
}

void DuckHunt::Play::update()
{
    float dt = _clock.restart().asSeconds();

    if (_pause)
        return;
    if (!_gameHasStarted) {
        if (_dog) {
            _dog->update(dt);
            if (_dog->isAnimationDone())
                _gameHasStarted = true;
        }
        return;
    } else
        playLoop(dt);
}
