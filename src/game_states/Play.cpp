/*
** Paul Mourens project
** Duck Hunt cpp
** Play.cpp
*/

#include "Core.hpp"

DuckHunt::Play::Play(int &score, Audio& audio, int round)
    : _audio(audio), _round(round),  _score(score)
{
    int perfectRoundBonus = getPerfectRoundBonus(_round);
    _assets = std::make_unique<Assets::PlayAssets>(std::vector<int>{_round, 3, _score, 1000, perfectRoundBonus});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
    _blackScreen.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    _blackScreen.setFillColor(sf::Color::Black);
    _hitboxRect.setFillColor(sf::Color::White);
    _duckHits.push_back(Blinking);
    for (int i = 0; i < 9; i++)
        _duckHits.push_back(Missed);
}

int DuckHunt::Play::getPerfectRoundBonus(int round)
{
    if (round <= 10)
        return 10000;
    else if (round <= 12)
        return 15000;
    else if (round <= 14)
        return 20000;
    else if (round <= 19)
        return 25000;
    else
        return 30000;
}

bool DuckHunt::Play::canStartNewRound()
{
    int required = 6;

    if (isPerfectRound())
        return true;
    _ducksHit = 0;
    for (size_t i = 0; i < _duckHits.size(); i++)
        if (_duckHits[i] == Hit)
            _ducksHit++;
    if (_round > 10)
        required++; // 7 ducks
    if (_round > 12)
        required++; // 8 ducks
    if (_round > 14)
        required++; // 9 ducks
    if (_round > 19)
        required++; // 10 ducks
    return _ducksHit >= required;
}

bool DuckHunt::Play::isPerfectRound()
{
    for (size_t i = 0; i < _duckHits.size(); i++)
        if (_duckHits[i] == Hit)
            _ducksHit++;
    return _ducksHit == 10;
}

void DuckHunt::Play::drawIntro(sf::RenderWindow& window)
{
    window.draw(_assets->_roundBgSpr);
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
    if (!_flyAway)
        window.draw(_assets->_backgroundSpr);
    else
        window.draw(_assets->_flyAwayBackgroundSpr);
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
    if (_gameOverSequence) {
        window.draw(_assets->_textBgSpr);
        window.draw(_assets->_gameoverText);
    }
    if (_perfectBonusStarted) {
        window.draw(_assets->_textBgSpr);
        window.draw(_assets->_perfectRoundText);
        window.draw(_assets->_perfectRoundScoreText);
    }
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
    if (_jingleStarted) {
        if (!_ducksFlickering) {
            for (int i = 0; i < _ducksHit; i++)
                window.draw(redDucks[i]);
            for (int i = _ducksHit; i < 10; i++)
                window.draw(whiteDucks[i]);
        } else
            for (int i = 0; i < 10; i++)
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
    } else
        if (_ammo == 0 && _duck)
            _flyAway = true;
    if (_duckIndex >= 9)
        _pointsCounting = true;
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
        _duck = std::make_unique<Animation::Duck>(_assets->_spritesheet, _round);
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
        _duckActive = false;
    }
    _dog->update(dt);
}

void DuckHunt::Play::dogLaughing()
{
    _dog->initLaugh({WINDOW_WIDTH / 2.f - 40, 775.f});
    _audio.playMusic(LAUGH_MUSIC, false);
    _dogLaughing = true;
    _dogHasAppeared = true;
    if (_flyAway)
        _flyAway = false;
}

void DuckHunt::Play::duckHasBeenCaught()
{
    _duck.reset();
    reload();
    _duckIndex++;
    _duckHits[_duckIndex] = Blinking;
    _dogCatching = false;
    _dogHasAppeared = false;
}

void DuckHunt::Play::pointsCountingSequence(float dt)
{
    _pointsCountingTimer += dt;

    if (isPerfectRound()) {
        _pointsCounting = false;
        _pointsCountingIndex = 0;
        _endGame = true;
        _perfectRound = true;
        return;
    }

    if (_pointsCountingTimer >= 0.25f && _pointsCountingIndex < _duckHits.size()) {
        _pointsCountingTimer = 0.f;
        for (size_t i = _pointsCountingIndex; i < _duckHits.size(); ++i) {
            if (_duckHits[i] == Missed) {
                for (size_t j = _duckHits.size() - 1; j > i; --j) {
                    if (_duckHits[j] == Hit) {
                        std::swap(_duckHits[i], _duckHits[j]);
                        _audio.playSound(POINTS_SOUND);
                        _pointsCountingIndex = i + 1;
                        return;
                    }
                }
                _pointsCounting = false;
                _pointsCountingIndex = 0;
                _endGame = true;
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

void DuckHunt::Play::newRoundOutro(float dt)
{
    _jingleTimer += dt;

    if (!_jingleStarted) {
        _audio.playMusic(ROUND_END_MUSIC, false);
        _jingleStarted = true;
        _jingleTimer = 0.f;
        _ducksFlickering = true;
        return;
    }
    if (_jingleTimer < 4.25f) {
        if (!_perfectBonusStarted) {
            _flickerTimer += dt;
            if (_flickerTimer >= 0.25f) {
                _ducksFlickering = !_ducksFlickering;
                _flickerTimer = 0.f;
            }
        }
        return;
    }
    if (_perfectRound && !_perfectBonusStarted) {
        _score += getPerfectRoundBonus(_round);
        _assets->setScore(_score);
        _audio.playMusic(PERFECT_ROUND_MUSIC, false);
        _perfectBonusStarted = true;
        _perfectTimer = 0.f;
        return;
    }
    if (_perfectBonusStarted) {
        _perfectTimer += dt;
        if (_perfectTimer < 2.5f)
            return;
    }
    _newRoundCanStart = true;
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

    if (_duck && _duck->isOffScreen() && !_dogLaughing)
        dogLaughing();

    if (_dog && _dogLaughing) {
        _dog->update(dt);
        if (_dog->isAnimationDone()) {
            _dogHasAppeared = false;
            _dogLaughing = false;
            _duckHits[_duckIndex] = Missed;
            _duckIndex++;
            _duckHits[_duckIndex] = Blinking;
            reload();
            _duckActive = false;
            _duck.reset();
        }
    }

    if (_pointsCounting)
        if (!_duck && _dog && _dog->isAnimationDone())
            pointsCountingSequence(dt);

    if (_endGame) {
        if (canStartNewRound())
            _newRound = true;
        else
            _gameOverSequence = true;
        _endGame = false;
        return;
    }
    
    if (_gameOverSequence)
        gameOverLaugh(dt);

    if (_newRound)
        newRoundOutro(dt);
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
