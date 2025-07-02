/*
** Paul Mourens project
** Duck Hunt cpp
** Play.cpp
*/

#include "game_states/Play.hpp"
#include "assets/PlayAssets.hpp"
#include <unistd.h>

DuckHunt::Play::Play(int &score, Audio& audio)
    : _audio(audio), _score(score)
{
    _assets = std::make_unique<Assets::PlayAssets>(std::vector<int>{1, 1, 3, 0, 0});
    _dog = std::make_unique<Animation::Dog>(_assets->_spritesheet);
    _blackScreen.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
    _blackScreen.setFillColor(sf::Color::Black);
    _hitboxRect.setFillColor(sf::Color::White);
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
    window.draw(_assets->_grassSpr);
    if (!_gameHasStarted) {
        window.draw(_assets->_roundBg);
        window.draw(_assets->_roundText);
        if (_dog && !_gameHasStarted)
            _dog->draw(window);
    }
    window.draw(_assets->_reloadBg);
    window.draw(_assets->_reloadText);
    window.draw(_assets->_ammoBg);
    window.draw(_assets->_ammoText);
    window.draw(_assets->_hitBg);
    window.draw(_assets->_hitText);
    window.draw(_assets->_scoreBg);
    window.draw(_assets->_scoreText);
    drawAmmo(window, _assets->_bullets);
    drawHits(window, _assets->_redDucks, _assets->_whiteDucks);
}

void DuckHunt::Play::drawAmmo(sf::RenderWindow &window, std::vector<sf::Sprite> bullets)
{
    for (int i = 0; i < _ammo; i++)
        window.draw(bullets[i]);
}

void DuckHunt::Play::drawHits(sf::RenderWindow &window, std::vector<sf::Sprite> redDucks,
    std::vector<sf::Sprite> whiteDucks)
{
    for (int i = 0; i < _duckCount && i < 10; i++)
        window.draw(redDucks[i]);
    for (int i = _duckCount; i < 10; i++)
        window.draw(whiteDucks[i]);
}

void DuckHunt::Play::reload()
{
    _ammo = 3;
    _assets->setAmmo(_ammo);
}

void DuckHunt::Play::reloadUsed()
{
    _reload--;
    _assets->setReload(_reload);
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
    _audio.play_sound(SHOT_SOUND);
    _ammo--;
    _assets->setAmmo(_ammo);
    if (_ammo == 0) {
        if (_reload > 0) {
            reload();
            reloadUsed();
        } else
            _gameOver = true;
    }
    handleHit(mousePos);
    //_duck++;
    //if _duck >= 10
    //next round
} 

void DuckHunt::Play::handleInput(input &in, sf::Vector2i mousePos)
{
    if (!_gameHasStarted)
        return;
    if (in == left_click) {
        shoot(mousePos);
        return;
    } else
        return;
}

void DuckHunt::Play::duckHasBeenHit()
{
    _audio.stop_music(FLAP_MUSIC);
    _duck->isHit();
    _audio.play_music(FALL_MUSIC, false);
}

void DuckHunt::Play::dogCatchingDuck(float dt)
{
    if (!_dogHasAppeared) {
        sf::Vector2f duckPos = _duck->getSprite().getPosition();
        _dog->happy1({ duckPos.x - 125.f, 775.f });
        _audio.stop_music(FALL_MUSIC);
        _audio.play_music(CAUGHT_MUSIC, false);
        _dogCatching = true;
        _dogHasAppeared = true;
    }
    _dog->update(dt);
}

void DuckHunt::Play::playLoop(float dt)
{
    if (!_duck) {
        spawnDuck();
        _audio.play_music(FLAP_MUSIC, true);
        return;
    }
    if (_duckActive && _duck) {
        _duck->update(dt);
    }
    if (_showFlash) {
        if (_flashClock.getElapsedTime().asSeconds() >= 0.05f) {
            _showFlash = false;
            if (_duckBounds.contains(_lastMouseShot)) {
                duckHasBeenHit();
            }
        }
    }
    if (_duck && _duck->isCaught())
        dogCatchingDuck(dt);
    if (_dogCatching && _dog->isAnimationDone()) {
        _duck.reset();
        _duckActive = false;
        _dogCatching = false;
    }
}

void DuckHunt::Play::update()
{
    float dt = _clock.restart().asSeconds();

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

void DuckHunt::Play::spawnDuck()
{
    if (!_duckActive) {
        _duck = std::make_unique<Animation::Duck>(_assets->_spritesheet);
        _duck->setDirection({ (rand() % 2 ? 1.f : -1.f), -1.f });
        _duckActive = true;
    }
}
