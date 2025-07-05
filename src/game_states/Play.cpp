/*
** Paul Mourens project
** Duck Hunt cpp
** Play.cpp
*/

#include "Core.hpp"

DuckHunt::Play::Play(int &score, Audio& audio)
    : _audio(audio), _score(score)
{
    _assets = std::make_unique<Assets::PlayAssets>(std::vector<int>{1, 3, 0, 0, 1000, 10000});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
    _blackScreen.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    _blackScreen.setFillColor(sf::Color::Black);
    _hitboxRect.setFillColor(sf::Color::White);
    _duckHits.push_back(Blinking);
    for (int i = 0; i < 9; i++)
        _duckHits.push_back(Missed);
}

void DuckHunt::Play::startNewRound()
{
    _gameHasStarted = false;
    _round++;
    _assets->setRound(_round);
    _currentDuckBlinking = true;
    _currentDuckBlinkTimer = 0.f;
    reload();
    _duckHits[0] = Blinking;
    for (size_t i = 1; i < _duckHits.size(); i++)
        _duckHits[i] = Missed;
}

bool DuckHunt::Play::canStartNewRound()
{
    int duckHits = 0;
    int required = 6;

    if (isPerfectRound())
        return true;
    for (size_t i = 0; i < _duckHits.size(); i++)
        if (_duckHits[i] == Hit)
            duckHits++;
    if (_round > 10)
        required++; // 7 ducks
    if (_round > 12)
        required++; // 8 ducks
    if (_round > 14)
        required++; // 9 ducks
    if (_round > 19)
        required++; // 10 ducks
    return duckHits >= required;
}

bool DuckHunt::Play::isPerfectRound()
{
    int duckHits = 0;

    for (size_t i = 0; i < _duckHits.size(); i++)
        if (_duckHits[i] == Hit)
            duckHits++;
    return duckHits == 10;
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
    window.draw(_assets->_requiredBar);
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
    if (_duckActive && _duck) {
        if (_duck && _duck->isFalling())
            window.draw(_assets->_hitScoreText);
        _duck->draw(window);
    }
    if (_showFlash) {
        window.draw(_blackScreen);
        window.draw(_hitboxRect);
        return;
    }
    if (_dog && (_dogCatching || _dogLaughing || _gameOverAnimationStarted))
        _dog->draw(window);
    if (!_gameHasStarted)
        drawIntro(window);
    else
        window.draw(_assets->_grassSpr);
    drawHUD(window);
    if (_pause)
        window.draw(_assets->_pauseSpr);
    if (_gameOverSequence) {
        window.draw(_assets->_gameoverBgSpr);
        window.draw(_assets->_gameoverText);
    }
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
    if (_duck && isDuckHit()) {
        sf::FloatRect duckBounds = _duck->getSprite().getGlobalBounds();
        float hitX = duckBounds.left + duckBounds.width / 2.f - 30.f;
        float hitY = duckBounds.top + duckBounds.height / 2.f - 20.f;
        _assets->setHitScore(_duck->getDuckScore(_round), {hitX, hitY});
        _score += _duck->getDuckScore(_round);
        _assets->setScore(_score);
        _duckHits[_duckIndex] = Hit;
    } else {
        if (_ammo == 0 && _duck) {
            _flyAway = true;
            _duck->flyAway();
        }
    }
    if (_duckIndex >= 9) {
        _pointsCounting = true;
        /*if (canStartNewRound()) {
            startNewRound();
            return;
        } else {
            _gameOverSequence = true;
        }*/
    }
} 

void DuckHunt::Play::handleInput(input &in, sf::Vector2i mousePos)
{
    if (in == leftClick && !_dogHasAppeared && !_pause) {
        if (_duck && !_duck->isFalling() && !_flyAway)
            shoot(mousePos);
        return;
    } else if ((in == quit || in == escape) && !_gameOverSequence) {
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
    _dog->initLaugh({WINDOW_WIDTH / 2.f - 40, 775.f});
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

void DuckHunt::Play::pointsCountingSequence(float dt)
{
    _pointsCountingTimer += dt;
    if (_pointsCountingTimer >= 0.25f && _pointsCountingIndex < _duckHits.size()) {
        _pointsCountingTimer = 0.f;
        for (size_t i = _pointsCountingIndex; i < _duckHits.size(); ++i) {
            if (_duckHits[i] == Hit) {
                std::swap(_duckHits[i], _duckHits[_pointsCountingIndex]);
                _audio.playSound(POINTS_SOUND);
                _pointsCountingIndex++;
                return;
            }
        }
        _pointsCounting = false;
        _pointsCountingIndex = 0;
        _endGame = true;
    }
}

void DuckHunt::Play::gameOverLaugh(float dt)
{
    if (!_gameOverAnimationStarted) {
        _audio.playMusic(GAMEOVER_MUSIC, false);
        _dog->initGameoverLaugh({WINDOW_WIDTH / 2.f - 40.f, 800.f});
        _dogHasAppeared = true;
        _gameOverAnimationStarted = true;
    } else {
        _dog->update(dt);
    }
    if (_dog->isAnimationDone())
        _gameOver = true;
}

void DuckHunt::Play::playLoop(float dt)
{
    if (!_duck && _duckIndex < 10) {
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

    if (_pointsCounting)
        if (!_duck && _dog && _dog->isAnimationDone())
            pointsCountingSequence(dt);

    if (_endGame) {
        if (!canStartNewRound())
            _gameOverSequence = true;
        else
            _newRound = true;
        _endGame = false;
    }
    
    if (_gameOverSequence)
        gameOverLaugh(dt);
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
