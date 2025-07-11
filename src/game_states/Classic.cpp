/*
** Paul Mourens project
** Duck Hunt cpp
** Classic.cpp
*/

#include "Core.hpp"

DuckHunt::Classic::Classic(int &score, Audio& audio, int round, int gamemode)
    : _audio(audio), _round(round),  _score(score), _maxDucksOnScreen(gamemode)
{
    int perfectRoundBonus = getPerfectRoundBonus(_round);
    _assets = std::make_unique<Assets::ClassicAssets>(std::vector<int>{_round, 3, _score, 1000, perfectRoundBonus});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
    _blackScreen.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    _blackScreen.setFillColor(sf::Color::Black);
    for (int i = 0; i < 10; i++)
        _duckHits.push_back(Missed);
}

int DuckHunt::Classic::getPerfectRoundBonus(int round)
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

bool DuckHunt::Classic::canStartNewRound()
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

bool DuckHunt::Classic::isPerfectRound()
{
    for (size_t i = 0; i < _duckHits.size(); i++)
        if (_duckHits[i] == Hit)
            _ducksHit++;
    return _ducksHit == 10;
}

void DuckHunt::Classic::drawIntro(sf::RenderWindow& window)
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

void DuckHunt::Classic::drawHUD(sf::RenderWindow& window)
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

void DuckHunt::Classic::draw(sf::RenderWindow& window)
{
    if (!_flyAway)
        window.draw(_assets->_backgroundSpr);
    else
        window.draw(_assets->_flyAwayBackgroundSpr);
    for (size_t i = 0; i < _ducks.size(); i++) {
        if (_ducks[i] && _ducks[i]->isFalling())
            window.draw(_assets->_hitScoreText);
        if (_ducks[i])
            _ducks[i]->draw(window);
    }
    if (_showFlash) {
        window.draw(_blackScreen);
        for (size_t i = 0; i < _hitboxRects.size(); i++)
            window.draw(_hitboxRects[i]);
        if (_flashClock.getElapsedTime().asSeconds() >= 0.075f)
            _showFlash = false;
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

void DuckHunt::Classic::drawAmmo(sf::RenderWindow &window, std::vector<sf::Sprite> bullets)
{
    for (int i = 0; i < _ammo; i++)
        window.draw(bullets[i]);
}

void DuckHunt::Classic::drawHits(sf::RenderWindow &window, std::vector<sf::Sprite> redDucks,
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

void DuckHunt::Classic::reload()
{
    _ammo = 3;
    _assets->setAmmo(_ammo);
}

void DuckHunt::Classic::handleHit(sf::Vector2i mousePos)
{
    _showFlash = true;
    _flashClock.restart();
    _lastMouseShot = sf::Vector2f(mousePos);
    _hitboxRects.clear();
    for (size_t i = 0; i < _ducks.size(); i++) {
        if (!_ducks[i] || _ducks[i]->isFalling() || _ducks[i]->isCaught())
            continue;
        sf::FloatRect bounds = _ducks[i]->getSprite().getGlobalBounds();
        sf::RectangleShape hitbox;
        hitbox.setPosition(bounds.left, bounds.top);
        hitbox.setSize({bounds.width, bounds.height});
        hitbox.setFillColor(sf::Color::White);
        _hitboxRects.push_back(hitbox);
    }
}

bool DuckHunt::Classic::isDuckHit(size_t duckIndex)
{
    if (!_ducks[duckIndex])
        return false;
    sf::FloatRect bounds = _ducks[duckIndex]->getSprite().getGlobalBounds();
    return bounds.contains(_lastMouseShot);
}

void DuckHunt::Classic::shoot(sf::Vector2i mousePos)
{
    bool duckWasHit = false;

    _audio.playSound(SHOT_SOUND);
    _ammo--;
    _assets->setAmmo(_ammo);
    handleHit(mousePos);
    for (size_t i = 0; i < _ducks.size(); i++) {
        if (!_ducks[i])
            continue;
        if (isDuckHit(i)) {
            duckHasBeenHit(i);
            sf::FloatRect duckBounds = _ducks[i]->getSprite().getGlobalBounds();
            float hitX = duckBounds.left + duckBounds.width / 2.f - 30.f;
            float hitY = duckBounds.top + duckBounds.height / 2.f - 20.f;
            _assets->setHitScore(_ducks[i]->getDuckScore(_round), {hitX, hitY});
            _score += _ducks[i]->getDuckScore(_round);
            _assets->setScore(_score);
            _duckHits[_duckIndex + i] = Hit;
            duckWasHit = true;
        }
    }
    if (!_ducks.empty() && _ammo == 0 && !duckWasHit)
        _flyAway = true;
}

bool DuckHunt::Classic::canShoot()
{
    size_t ducksInactive = 0;

    if (_dogHasAppeared)
        return false;
    if (_ducks.empty())
        return false;
    if (_flyAway)
        return false;
    for (size_t i = 0; i < _ducks.size(); i++)
        if (_ducks[i] && !_ducks[i]->isFlying())
            ducksInactive++;
    if (_ducks.size() == ducksInactive)
        return false;
    return true;
}

void DuckHunt::Classic::handleInput(input &in, sf::Vector2i mousePos)
{
    if (in == leftClick && !_dogHasAppeared && !_pause) {
        if (canShoot())
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

void DuckHunt::Classic::spawnDuck(float dt)
{
    _duckSpawnTimer += dt;

    if (_ducks.empty()) {
        if (_duckIndex >= MAX_DUCKS_PER_ROUND)
            return;
        _ducks.push_back(std::make_unique<Animation::Duck>(_assets->_spritesheet, _round));
        _audio.playMusic(FLAP_MUSIC, true);
        _duckHits[_duckIndex] = Blinking;
        _duckSpawnTimer = 0.f;
        return;
    }

    if (_maxDucksOnScreen == 2 && _ducks.size() == 1 && _duckSpawnTimer > 0.75f) {
        if (_duckIndex + 1 >= MAX_DUCKS_PER_ROUND)
            return;
        _ducks.push_back(std::make_unique<Animation::Duck>(_assets->_spritesheet, _round));
        _duckHits[_duckIndex + 1] = Blinking;
        _audio.playMusic(FLAP_MUSIC, true);
        _duckSpawnTimer = 0.f;
    }
}

void DuckHunt::Classic::duckHasBeenHit(int duckIndex)
{
    if (_caughtOrder.first == -1)
        _caughtOrder.first = duckIndex;
    else if (_caughtOrder.second == -1 && duckIndex != _caughtOrder.first)
        _caughtOrder.second = duckIndex;
    if (_ducks[duckIndex]) {
        _ducks[duckIndex]->Hit();
        _audio.playMusic(FALL_MUSIC, false);
    }
}

sf::Vector2f DuckHunt::Classic::getLastCaughtPos(int ducksCaught)
{
    if (_ducks.size() == 1 && _ducks[0])
        return _ducks[0]->getSprite().getPosition();

    if (_ducks.size() == 2) {
        int index = (ducksCaught == 1) ? _caughtOrder.first : _caughtOrder.second;
        if (index != -1 && _ducks[index])
            return _ducks[index]->getSprite().getPosition();
    }
    return {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT};
}

void DuckHunt::Classic::dogCatchingDuck(float dt)
{
    int ducksCaught = 0;

    for (size_t i = 0; i < _ducks.size(); i++)
        if (_ducks[i] && _ducks[i]->isCaught())
            ducksCaught++;
    if (ducksCaught == 0)
        return;
    if (_ducks.size() == 2 && _ducks[0] && _ducks[1]) {
        if (_ducks[0]->isCaught() && _ducks[1]->isOnScreen())
            return;
        if (_ducks[0]->isOnScreen() && _ducks[1]->isCaught())
            return;
    }
    if (!_dogHasAppeared) {
        sf::Vector2f duckPos = getLastCaughtPos(ducksCaught);
        _dog->initHappy({ duckPos.x - 125.f, 775.f }, ducksCaught);
        _audio.stopMusic(FALL_MUSIC);
        _audio.playMusic(CAUGHT_MUSIC, false);
        _dogCatching = true;
        _dogHasAppeared = true;
        _caughtOrder = {-1, -1};
        _flyAway = false;
    } else
        _dog->update(dt);
}

void DuckHunt::Classic::dogLaughing()
{
    _dog->initLaugh({WINDOW_WIDTH / 2.f - 40, 775.f});
    _audio.playMusic(LAUGH_MUSIC, false);
    _dogLaughing = true;
    _dogHasAppeared = true;
    if (_flyAway)
        _flyAway = false;
}

void DuckHunt::Classic::duckHasBeenCaught()
{
    reload();
    _dogCatching = false;
    _dogHasAppeared = false;
    _duckIndex += static_cast<int>(_ducks.size());
    _ducks.clear();
}

bool DuckHunt::Classic::ducksAllLeft()
{
    size_t ducksOutOfScreen = 0;

    if (_ducks.empty())
        return false;
    for (size_t i = 0; i < _ducks.size(); i++) {
        if (_ducks[i] && _ducks[i]->isOffScreen() && !_ducks[i]->isCaught()) {
            ducksOutOfScreen++;
            _duckHits[_duckIndex + i] = Missed;
        }
    }
    if (ducksOutOfScreen == _ducks.size())
        return true;
    return false;
}

void DuckHunt::Classic::pointsCountingSequence(float dt)
{
    _pointsCountingTimer += dt;

    if (isPerfectRound()) {
        _pointsCounting = true;
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
                _pointsCounting = true;
                _pointsCountingIndex = 0;
                _endGame = true;
                return;
            }
        }
        _pointsCounting = true;
        _pointsCountingIndex = 0;
        _endGame = true;
    }
}

void DuckHunt::Classic::gameOverLaugh(float dt)
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

void DuckHunt::Classic::newRoundOutro(float dt)
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

void DuckHunt::Classic::playLoop(float dt)
{
    if (_duckIndex < MAX_DUCKS_PER_ROUND && _ducks.size() < static_cast<size_t>(_maxDucksOnScreen))
        spawnDuck(dt);

    if (!_ducks.empty()) {
        for (size_t i = 0; i < _ducks.size(); i++) {
            if (!_ducks[i])
                continue;
            _ducks[i]->update(dt);
            _currentDuckBlinkTimer += dt;
            if (_currentDuckBlinkTimer >= 0.5f) {
                _currentDuckBlinking = !_currentDuckBlinking;
                _currentDuckBlinkTimer = 0.f;
            }
        }
    }

    if (_ducks.size() > 0)
        dogCatchingDuck(dt);

    if (_dog && _dogCatching && _dog->isAnimationDone())
        duckHasBeenCaught();

    if (_flyAway) {
        for (size_t i = 0; i < _ducks.size(); i++)
            if (_ducks[i])
                _ducks[i]->flyAway();
    }
    if (ducksAllLeft() && !_dogLaughing)
        dogLaughing();

    if (_dog && _dogLaughing && !_ducks.empty()) {
        _dog->update(dt);
        if (_dog->isAnimationDone()) {
            _dogHasAppeared = false;
            _dogLaughing = false;
            reload();
            _duckIndex += static_cast<int>(_ducks.size());
            _ducks.clear();
        }
    }
    if (_duckIndex > (MAX_DUCKS_PER_ROUND - _maxDucksOnScreen) && !_pointsCounting)
        if (_dog && _dog->isAnimationDone())
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

void DuckHunt::Classic::update()
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
